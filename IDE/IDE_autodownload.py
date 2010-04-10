import httplib
import wx
import thread
import xml.sax
import os
import sys, traceback
import zipfile
from xml.sax.handler import ContentHandler

class AutoDownloader(wx.Timer):
##    def __init__(self,parent,id,title):
    def __init__(self,parent,gauge,sb):
        wx.Timer.__init__(self,parent)
        self.IDE = parent
        self.statusbar = sb;

        if (os.access(self.IDE.appdir+"/update.version", os.W_OK)):
            self.SetStatusBarStatus("Unable to check for updates.  Re-run IDE as administrator to check for and install updates.")
        
        self.statusgauge = gauge
        self.mode = 'XMLFile'
        self.ufilecount = 0
        self.xmldata = ''
        self.updatenum = 0;
        result = True
        if os.path.exists(self.IDE.appdir + "/update.version"):
            file = open(self.IDE.appdir + "/update.version")
            self.updatenum = int(file.read())
            file.close()
        else:
            result = self.SetVersionNumber("0")

        # How much to divide the gauge value by
        self.gauge_adjust = 1

#        handler = DownloaderXMLInterpreter(self.XMLCallback)
#        parser = make_parser()
#        parser.setContentHandler(handler)
#        parser.parse("updates.xml")
        
        self.downloader = DownloaderStatus(self.statusgauge,"/updates.xml",self.DownloadCallback,self)
        self.downloader.Start()

        self.updates_installed = False

        self.Start(10)
##        wx.CallLater(10, self.WaitCheck)
##        self.Bind(wx.EVT_TIMER, self.WaitCheck)
##        self.timer = wx.Timer(self)
##        self.timer.Start(10)

    def SetStatusBarStatus(self,status):
        self.statusbar.SetStatusText(status, 0)

    def Notify(self):
        self.WaitCheck()
        
    def ReloadVersionNumber(self):
        file = open(self.IDE.appdir + "/update.version",'r')
        self.updatenum = int(file.read())
        file.close()

    def SetVersionNumber(self,version):
        try:
            file = open(self.IDE.appdir + "/update.version",'w')
            file.write(version)
            file.close()
            return True
        except:
            return False
    
    def DownloadCallback(self,data):
        if (self.mode == 'XMLFile'):
            handler = DownloaderXMLInterpreter(self.XMLCallback)
            xml.sax.parseString(data,handler)
        if (self.mode == 'UpdateFile'):
            if (self.ufilecount >= 0):
##                self.SetStatusBarStatus("Finished " + self.xmldata[self.ufilecount])
                # Save file
                try:
                    os.mkdir(self.IDE.appdir+"/todopatches")
                except WindowsError:
                    noonecares = True
                except IOError:
                    self.SetStatusBarStatus("Unable to make " + self.IDE.appdir+"/todopatches.")
                try:
                    file = open(self.IDE.appdir+"/todopatches/"+self.xmldata[self.ufilecount+2],'wb')
                    file.write(data)
                    file.close()
                except:
                    self.SetStatusBarStatus("Something went wrong while writing the ZIP.")

                destdir = self.IDE.appdir+"/todopatches/"+self.xmldata[self.ufilecount+2][0:-4]

                try:
                    zf = zipfile.ZipFile(self.IDE.appdir+"/todopatches/"+self.xmldata[self.ufilecount+2])
                except:
                    traceback.print_exc()
                
                try:
                    os.mkdir(destdir)
                except WindowsError:
                    noonecares = True
                except IOError:
                    traceback.print_exc()
                try:
                    self.SetStatusBarStatus("Installing update...")
                    filelist = zf.namelist()
                    i = 0
                    for f in filelist:
                        self.downloader.isDownloading = True
                        self.downloader.notDownloadingCount = 0
                        self.statusgauge.SetValue((100/float(len(filelist))*i)/self.gauge_adjust)
##                        self.SetStatusBarStatus("Extracting " + f)
                        file = open(destdir+"/"+f,'wb')
                        file.write(zf.read(f))
                        file.close()
                        i += 1
                except:
                    traceback.print_exc()
                try:
                    zf.close()
                except:
                    traceback.print_exc()
                os.remove(self.IDE.appdir+"/todopatches/"+self.xmldata[self.ufilecount+2])
                self.SetVersionNumber(self.xmldata[self.ufilecount+2][0:-4])
                self.ReloadVersionNumber()
##                self.SetStatusBarStatus("Extracted patch.")
                
            self.ufilecount += 3
            if (self.ufilecount < len(self.xmldata)):
                self.UpdateDownloader()
            elif (self.updates_installed == False):
                self.Stop()
                self.SetStatusBarStatus("No updates available.")
                self.statusgauge.SetValue(0)
            elif (self.updates_installed == True):
                self.Stop()
                self.SetStatusBarStatus("Updates installed.  Please restart to update IDE.")
                self.statusgauge.SetValue(0)
                self.IDE.menumanager.AddRestart()
                dlg = wx.MessageDialog(self.IDE, "Updates have been installed.  Restart the IDE to apply the updates.","Updates Installed")
                dlg.ShowModal()
##            else:
##                self.SetStatusBarStatus("Finished patches.")
            
    def WaitCheck(self):
        self.downloader.notDownloadingCount += 1
        if (self.downloader.notDownloadingCount == 1000):
            self.downloader.isDownloading = False
            self.downloader.notDownloadingCount = 0
        if not (self.downloader.isDownloading):
            self.statusgauge.Pulse()

    def XMLCallback(self,data):
        if (self.mode == 'XMLFile'):
            # Now we have our data as list, parse it and start downloading other files.
            self.mode = 'UpdateFile'
            self.ufilecount = -3
            self.xmldata = data

    def UpdateDownloader(self):
        patchid = self.xmldata[self.ufilecount+2][0:-4]
        if (int(patchid)>self.updatenum):
            self.updates_installed = True
            self.SetStatusBarStatus("Downloading " + self.xmldata[self.ufilecount] + "...")
            self.downloader = DownloaderStatus(self.statusgauge,"/updates/" + self.xmldata[self.ufilecount+2],self.DownloadCallback,self)
            self.downloader.Start()
        else:
            self.SetStatusBarStatus("Already installed " + self.xmldata[self.ufilecount] + ".")
            if (self.ufilecount + 3 < len(self.xmldata)):
                self.ufilecount += 3
                self.UpdateDownloader()
            elif (self.updates_installed == False):
                self.Stop()
                self.SetStatusBarStatus("No updates available.")
                self.statusgauge.SetValue(0)
            elif (self.updates_installed == True):
                self.Stop()
                self.SetStatusBarStatus("Updates installed.  Please restart to update IDE.")
                self.statusgauge.SetValue(0)
                self.IDE.menumanager.AddRestart()
                dlg = wx.MessageDialog(self.IDE, "Updates have been installed.  Restart the IDE to apply the updates.","Updates Installed",wx.OK)
                dlg.ShowModal()

class DownloaderStatus:
    def __init__(self,gauge,url,callback,parent):
        self.domain = "www.roket3d.com"
        self.targetURL = url
        self.keepGoing = False
        self.conn = 0
        self.gauge = gauge
        self.callback = callback
        self.notDownloadingCount = 0
        self.isDownloading = True
        self.gauge_adjust = parent.gauge_adjust
        self.parent = parent

    def Start(self):
        self.keepGoing = self.running = True
        thread.start_new_thread(self.Run, ())

    def Stop(self):
        self.keepGoing = False

    def IsRunning(self):
        return self.running

    def Run(self):
        conn = httplib.HTTPConnection(self.domain)
        conn.request("GET", self.targetURL)
        r1 = conn.getresponse()
        if (r1.status == 404):
            self.parent.SetStatusBarStatus("Unable to get updates.")
##        else:
##            self.parent.SetStatusBarStatus("Downloading data...")

        totalbytes = int(r1.getheader("content-length"))+10
        currentbytes = 0
        data1 = ''

        while self.keepGoing and currentbytes < totalbytes:
            self.isDownloading = True
            self.notDownloadingCount = 0
            # Get 10 bytes and update progress
            data1 += r1.read(1)
            currentbytes += 1
            if (currentbytes > totalbytes):
                self.keepGoing = False
            perc = int(100/float(totalbytes)*currentbytes)
            self.gauge.SetValue(perc/self.gauge_adjust)
##            self.parent.SetStatusBarStatus("Downloaded: " + str(perc) + "% (" + str(currentbytes) + "/" + str(totalbytes) + ")")
        
        conn.close()

        self.running = False

        self.callback(data1)

# handler = DownloaderXMLInterpreter()
# parser = make_parser()
# parser.setContentHandler(handler)
# parser.parse(string)

class DownloaderXMLInterpreter(ContentHandler):
    def __init__(self,callback):
        self.updatelist = list()
        self.section_type = ''
        self.element_type = ''
        self.buffer = ''
        self.callback = callback

    def startElement(self, name, attrs):
        self.buffer = ''
        if (name == 'updates'):
            self.section_type = 'updates'
        if (self.section_type == 'updates' and name == 'update'):
            self.section_type = 'update'
            
    def endElement(self,name):
        if (self.section_type == 'update' and name != 'update'):
            # Handle update data
            if (name == 'type'):
                self.utype = self.buffer
            elif (name == 'name'):
                self.uname = self.buffer
            elif (name == 'url'):
                self.uurl = self.buffer
            self.buffer = ''
        elif (self.section_type == 'update' and name == 'update'):
            self.updatelist.append(self.uname)
            self.updatelist.append(self.utype)
            self.updatelist.append(self.uurl)
            self.section_type = 'updates'
        elif (self.section_type == 'updates' and name == 'updates'):
            self.callback(self.updatelist)
            
    def characters(self, chars):
        self.buffer += chars
            
##class MyApp(wx.App):
##    def OnInit(self):
##        frame = MyWindow(None, -1, 'Download')
##        frame.Show(True)
##        return True

##var = MyApp(0)
##var.MainLoop()

#var = DownloaderStatus()
#var.Start()

#while var.running:
#    test = False
    # no

#var.Stop()
