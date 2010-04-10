import wx
import os
import wx.wizard as wiz
import thread
import shutil
import sys
import subprocess

def makePageTitle(wizPg, title):
    sizer = wx.BoxSizer(wx.VERTICAL)
    wizPg.SetSizer(sizer)
    title = wx.StaticText(wizPg, -1, title)
    title.SetFont(wx.Font(18, wx.SWISS, wx.NORMAL, wx.BOLD))
    sizer.Add(title, 0, wx.ALIGN_CENTRE|wx.ALL, 5)
    sizer.Add(wx.StaticLine(wizPg, -1), 0, wx.EXPAND|wx.ALL, 5)
    return sizer

class Page_Title(wiz.PyWizardPage):
    def __init__(self, parent, title, description):
        wiz.PyWizardPage.__init__(self, parent)
        self.sizer = makePageTitle(self, title)
        self.parent = parent

        self.content_description = wx.StaticText(self, -1,
                   description,
                   style=wx.EXPAND)

        # Content Arrangement
        self.content_sizer = wx.BoxSizer(wx.VERTICAL)
        self.content_sizer.Add(self.content_description, proportion=0)
        self.content_sizer.AddStretchSpacer()

        self.sizer.Add(self.content_sizer, 0, wx.ALL, 5)

    def SetNext(self, next):
        self.next = next

    def SetPrev(self, prev):
        self.prev = prev

    def GetNext(self):
        return self.next
        
    def GetPrev(self):
        return self.prev

class Page_ChooseDirectory(wiz.PyWizardPage):
    def __init__(self, parent, title):
        wiz.PyWizardPage.__init__(self, parent)
        self.next = self.prev = None
        self.parent = parent
        self.sizer = makePageTitle(self, title)

        # Static Text
        self.content_description = wx.StaticText(self, -1,
                   "During the publishing step, Roket3D will export your project to a directory.  Roket3D needs to know where you\nwould like your project to be placed.  It is recommended that you select an empty directory.\n\nPlease select a directory:\n",
                   style=wx.EXPAND)

        # Directory Selector
        self.directory_text = wx.TextCtrl(self, -1, "", size=(125, -1))
        self.directory_button = wx.Button(self, 20, "Browse", (20, 80)) ##, (120, 45))
        self.Bind(wx.EVT_BUTTON, self.OnSelectDirectory, self.directory_button)
        self.directory_button.SetToolTipString("Select directory.")
        
        self.directory_sizer = wx.BoxSizer(wx.HORIZONTAL)
        self.directory_sizer.Add(self.directory_text, proportion=1)
        self.directory_sizer.Add(self.directory_button, proportion=0)
        self.directory_text.Disable()
        self.directory_text.AppendText(self.parent.publish_directory)

        # Content Arrangement
        self.content_sizer = wx.BoxSizer(wx.VERTICAL)
        self.content_sizer.Add(self.content_description, proportion=0)
        self.content_sizer.Add(self.directory_sizer, proportion=0, flag=wx.EXPAND)
        self.content_sizer.AddStretchSpacer()

        self.sizer.Add(self.content_sizer, 0, wx.ALL, 5)

    def OnSelectDirectory(self,event):
        dlg = wx.DirDialog(self, "Select publish directory:",
                          style=wx.DD_DEFAULT_STYLE
                           #| wx.DD_DIR_MUST_EXIST
                           #| wx.DD_CHANGE_DIR
                           ,size = wx.Size(550,350))

        dlg.CenterOnParent(wx.BOTH)
        dlg.CenterOnScreen(wx.BOTH)

        # If the user selects OK, then we process the dialog's data.
        # This is done by getting the path data from the dialog - BEFORE
        # we destroy it. 
        if dlg.ShowModal() == wx.ID_OK:
            self.directory_text.Clear()
            self.directory_text.AppendText(dlg.GetPath())
            self.parent.publish_directory = dlg.GetPath()
            wx.FindWindowById(wx.ID_FORWARD).Enable()

        # Only destroy a dialog after you're done with it.
        dlg.Destroy()

    def SetNext(self, next):
        self.next = next

    def SetPrev(self, prev):
        self.prev = prev

    def GetNext(self):
        if (self.parent.GetCurrentPage() == self and self.directory_text.GetLineText(0) == ""):
            wx.FindWindowById(wx.ID_FORWARD).Disable()
        return self.next
        
    def GetPrev(self):
        return self.prev

class Page_PerformPublish(wiz.PyWizardPage):
    def __init__(self, parent, title, ide):
        wiz.PyWizardPage.__init__(self, parent)
        self.next = self.prev = None
        self.sizer = makePageTitle(self, title)
        self.parent = parent
        self.ide = ide

        # Static Text
        self.content_description = wx.StaticText(self, -1,
                   "Roket3D is now exporting your project...\n",
                   style=wx.EXPAND)

        # Gauge
        self.publish_progress = wx.Gauge(self,range=100,style=wx.GA_SMOOTH)
        self.publish_progress.SetValue(0)

        # Content Arrangement
        self.content_sizer = wx.BoxSizer(wx.VERTICAL)
        self.content_sizer.Add(self.content_description, proportion=0)
        self.content_sizer.Add(self.publish_progress, proportion=0, flag=wx.EXPAND)
        self.content_sizer.AddStretchSpacer()

        self.sizer.Add(self.content_sizer, 0, wx.ALL, 5)

    def SetNext(self, next):
        self.next = next

    def SetPrev(self, prev):
        self.prev = prev

    def GetNext(self):
        if (self.parent.GetCurrentPage() == self and self.publish_progress.GetValue() == 0):
            wx.FindWindowById(wx.ID_FORWARD).Disable()
            self.publish_progress.SetValue(1)
            self.publisher = PublisherThread(self.publish_progress,self,self.ide)
            self.publisher.SetGroup1(self.parent.publish_1_windows,self.parent.publish_1_macos,self.parent.publish_1_linux86,self.parent.publish_1_linux64)
            self.publisher.SetGroup2(self.parent.publish_2_windows,self.parent.publish_2_macos,self.parent.publish_2_linux)
            self.publisher.SetGroup3(self.parent.publish_3_windowsi,self.parent.publish_3_windowsc,self.parent.publish_3_macosi,self.parent.publish_3_linuxi,self.parent.publish_3_livecd)
            self.publisher.SetGroup4(self.parent.publish_4_rg)
            self.publisher.SetDirectory(self.parent.publish_directory)
            self.publisher.Publish()
        elif (self.parent.GetCurrentPage() == self and self.publish_progress.GetValue() != 100):
            wx.FindWindowById(wx.ID_FORWARD).Disable()
        elif (self.parent.GetCurrentPage() == self and self.publish_progress.GetValue() == 100):
            wx.FindWindowById(wx.ID_FORWARD).Enable()            
        return self.next
        
    def GetPrev(self):
        return self.prev

class Page_ChoosePublishOptions(wiz.PyWizardPage):
    def __init__(self, parent, title):
        wiz.PyWizardPage.__init__(self, parent)
        self.next = self.prev = None
        self.sizer = makePageTitle(self, title)
        self.parent = parent

        # Static Text
        self.content_description = wx.StaticText(self, -1,
                   "Select the platforms and distribution types you would like to publish your project to:\n",
                   style=wx.EXPAND)

        # OPTION GROUP 1
        self.grp_1 = wx.StaticBox(self, -1, "Plain Files")
        self.grp_1_windows  = wx.CheckBox(self, -1, "Windows")
        self.grp_1_windows.SetValue(True)
        self.grp_1_macos    = wx.CheckBox(self, -1, "Mac OS")
        self.grp_1_linux86  = wx.CheckBox(self, -1, "Linux (x86)")
        self.grp_1_linux64  = wx.CheckBox(self, -1, "Linux (x64)")
        self.grp_1_macos.Disable()
        self.grp_1_linux86.Disable()
        self.grp_1_linux64.Disable()

        self.box_1 = wx.StaticBoxSizer(self.grp_1, wx.VERTICAL)
        self.box_1.Add(self.grp_1_windows  , 0, wx.ALL, 5)
        self.box_1.Add(self.grp_1_macos    , 0, wx.ALL, 5)
        self.box_1.Add(self.grp_1_linux86  , 0, wx.ALL, 5)
        self.box_1.Add(self.grp_1_linux64  , 0, wx.ALL, 5)

        # OPTION GROUP 2
        self.grp_2 = wx.StaticBox(self, -1, "Installations")
        self.grp_2_windows  = wx.CheckBox(self, -1, "Windows")
        self.grp_2_macos    = wx.CheckBox(self, -1, "Mac OS")
        self.grp_2_linux    = wx.CheckBox(self, -1, "Linux (x86 and x64, multi-installer)")
        self.grp_2_windows.Disable()
        self.grp_2_macos.Disable()
        self.grp_2_linux.Disable()

        self.box_2 = wx.StaticBoxSizer(self.grp_2, wx.VERTICAL)
        self.box_2.Add(self.grp_2_windows  , 0, wx.ALL, 5)
        self.box_2.Add(self.grp_2_macos    , 0, wx.ALL, 5)
        self.box_2.Add(self.grp_2_linux    , 0, wx.ALL, 5)

        # OPTION GROUP 3
        self.grp_3 = wx.StaticBox(self, -1, "CD/DVD Images")
        self.grp_3_windowsi = wx.CheckBox(self, -1, "Windows (Installer)")
        self.grp_3_windowsc = wx.CheckBox(self, -1, "Windows (Run From CD)")
        self.grp_3_macosi   = wx.CheckBox(self, -1, "Mac OS (Installer)")
        self.grp_3_linuxi   = wx.CheckBox(self, -1, "Linux (Installer)")
        self.grp_3_livecd   = wx.CheckBox(self, -1, "Live CD (OS Independent)")
        self.grp_3_windowsi.Disable()
        self.grp_3_windowsc.Disable()
        self.grp_3_macosi.Disable()
        self.grp_3_linuxi.Disable()
        self.grp_3_livecd.Disable()

        self.box_3 = wx.StaticBoxSizer(self.grp_3, wx.VERTICAL)
        self.box_3.Add(self.grp_3_windowsi , 0, wx.ALL, 5)
        self.box_3.Add(self.grp_3_windowsc , 0, wx.ALL, 5)
        self.box_3.Add(self.grp_3_macosi   , 0, wx.ALL, 5)
        self.box_3.Add(self.grp_3_linuxi   , 0, wx.ALL, 5)
        self.box_3.Add(self.grp_3_livecd   , 0, wx.ALL, 5)

        # OPTION GROUP 4
        self.grp_4 = wx.StaticBox(self, -1, "Web Publish")
        self.grp_4_rg       = wx.CheckBox(self, -1, "RoketGames")
        self.grp_4_rg.Disable()

        self.box_4 = wx.StaticBoxSizer(self.grp_4, wx.VERTICAL)
        self.box_4.Add(self.grp_4_rg       , 0, wx.ALL, 5)

        # ADD OPTIONS
        self.grid_sizer = wx.GridBagSizer(2, 2)
        self.grid_sizer.Add(self.box_1,(0,0), flag=wx.EXPAND)
        self.grid_sizer.Add(self.box_2,(0,1), flag=wx.EXPAND)
        self.grid_sizer.Add(self.box_3,(1,0), flag=wx.EXPAND)
        self.grid_sizer.Add(self.box_4,(1,1), flag=wx.EXPAND)

        # Content Arrangement
        self.content_sizer = wx.BoxSizer(wx.VERTICAL)
        self.content_sizer.Add(self.content_description, proportion=0)
        self.content_sizer.Add(self.grid_sizer, proportion=1, flag=wx.EXPAND)

        self.sizer.Add(self.content_sizer, wx.EXPAND, wx.ALL, 5)

    def SetNext(self, next):
        self.next = next

    def SetPrev(self, prev):
        self.prev = prev

    def GetNext(self):
        if (self.parent.GetCurrentPage() == self):
            self.parent.publish_1_windows  = self.grp_1_windows.GetValue()
            self.parent.publish_1_macos    = self.grp_1_macos.GetValue()
            self.parent.publish_1_linux86  = self.grp_1_linux86.GetValue()
            self.parent.publish_1_linux64  = self.grp_1_linux64.GetValue()

            self.parent.publish_2_windows  = self.grp_2_windows.GetValue()
            self.parent.publish_2_macos    = self.grp_2_macos.GetValue()
            self.parent.publish_2_linux    = self.grp_2_linux.GetValue()

            self.parent.publish_3_windowsi = self.grp_3_windowsi.GetValue()
            self.parent.publish_3_windowsc = self.grp_3_windowsc.GetValue()
            self.parent.publish_3_macosi   = self.grp_3_macosi.GetValue()
            self.parent.publish_3_linuxi   = self.grp_3_linuxi.GetValue()
            self.parent.publish_3_livecd   = self.grp_3_livecd.GetValue()

            self.parent.publish_4_rg       = self.grp_4_rg.GetValue()
        return self.next
        
    def GetPrev(self):
        return self.prev

def StartWizard(ide):
    wizard = wiz.Wizard(ide, -1, "Publish Game")

    # set settings
    wizard.publish_directory = ""
    wizard.publish_1_windows  = True
    wizard.publish_1_macos    = False
    wizard.publish_1_linux86  = False
    wizard.publish_1_linux64  = False

    wizard.publish_2_windows  = False
    wizard.publish_2_macos    = False
    wizard.publish_2_linux    = False

    wizard.publish_3_windowsi = False
    wizard.publish_3_windowsc = False
    wizard.publish_3_macosi   = False
    wizard.publish_3_linuxi   = False
    wizard.publish_3_livecd   = False

    wizard.publish_4_rg       = False

    page1 = Page_Title(wizard, "Publish Game","Once you have finished creating your game, you can export the game to multiple\nplatforms.  This wizard lets you export and publish your game to multiple platforms\nand websites.\n\nClick next to continue.")
    page2 = Page_ChooseDirectory(wizard, "Choose Directory")
    page3 = Page_ChoosePublishOptions(wizard, "Choose Platform")
    page4 = Page_PerformPublish(wizard, "Perform Publish", ide)
    page5 = Page_Title(wizard, "Publish Complete","Your game has been published to the directory you selected in step 2.  Press finish to open the directory.")

    page1.sizer.SetMinSize(wx.Size(600,400))
    wizard.FitToPage(page1)

    #page1.sizer.Add(wx.StaticText(page1, -1, """
#This wizard shows the ability to choose at runtime the order of the pages and also which bitmap is shown.
#"""))

    # Set the initial order of the pages
    page1.SetNext(page2)
    page2.SetPrev(page1)
    page2.SetNext(page3)
    page3.SetPrev(page2)
    page3.SetNext(page4)
    page4.SetPrev(page3)
    page4.SetNext(page5)
    page5.SetPrev(page4)

    if wizard.RunWizard(page1):
        # TODO: Make this cross-platform (linux could be trouble :/)
        p = subprocess.Popen("C:/Windows/explorer.exe \"" + wizard.publish_directory + "\"",cwd=wizard.publish_directory,shell=True, stdout=subprocess.PIPE).stdout
        

################################## PUBLISHER THREAD #############################

class PublisherThread:
    def __init__(self,gauge,parent,ide):
        self.gauge = gauge
        self.parent = parent
        self.keepGoing = False
        self.running = False
        self.ide = ide

        self.publish_directory = ""
        self.publish_1_windows  = True
        self.publish_1_macos    = False
        self.publish_1_linux86  = False
        self.publish_1_linux64  = False

        self.publish_2_windows  = False
        self.publish_2_macos    = False
        self.publish_2_linux    = False

        self.publish_3_windowsi = False
        self.publish_3_windowsc = False
        self.publish_3_macosi   = False
        self.publish_3_linuxi   = False
        self.publish_3_livecd   = False

        self.publish_4_rg       = False

    def SetGroup1(self,w,m,l86,l64):
        self.publish_1_windows = w
        self.publish_1_macos = m
        self.publish_1_linux86 = l86
        self.publish_1_linux64 = l64

    def SetDirectory(self,d):
        self.publish_directory = d

    def SetGroup2(self,w,m,l):
        self.publish_2_windows = w
        self.publish_2_macos = m
        self.publish_2_linux = l

    def SetGroup3(self,wi,wc,mi,li,lcd):
        self.publish_3_windowsi = wi
        self.publish_3_windowsc = wc
        self.publish_3_macosi = mi
        self.publish_3_linuxi = li
        self.publish_3_livecd = lcd

    def SetGroup4(self,r):
        self.publish_4_rg = r

    def Publish(self):
        self.keepGoing = self.running = True
##        self.__Publish()
        thread.start_new_thread(self.__Publish, ())

    def Stop(self):
        self.keepGoing = False

    def IsRunning(self):
        return self.running

    def __Publish(self):
        ####### PUBLISH -> WINDOWS -> PLAIN FILES
        if (self.publish_1_windows):
            output_path = os.path.join(self.publish_directory,"Windows_PlainFiles")
            try:
                os.mkdir(output_path)
            except WindowsError:
                noonecares = True
            data_path = os.path.join(self.ide.projectman.base_location,self.ide.projectman.data_location)
            engine_path = os.path.join(self.ide.appdir,"Data/publishengine")
            shader_path = os.path.join(self.ide.appdir,"Data/shaders")
            for root, dirs, files in os.walk(output_path,topdown=False):
                for name in files:
                    os.remove(os.path.join(root, name))
                for name in dirs:
                    try:
                        os.rmdir(os.path.join(root, name))
                    except OSError:
                        os.remove(os.path.join(root, name))

            total_files = os.listdir(data_path)
            i = 0
            while (i < len(total_files)):
                self.gauge.SetValue(33/float(len(total_files))*i)
                shutil.copy(os.path.join(data_path,total_files[i]),os.path.join(output_path,total_files[i]))
                i += 1

            total_files = os.listdir(engine_path)
            i = 0
            while (i < len(total_files)):
                self.gauge.SetValue(33+(33/float(len(total_files))*i))
                shutil.copy(os.path.join(engine_path,total_files[i]),os.path.join(output_path,total_files[i]))
                i += 1

            try:
                os.mkdir(os.path.join(output_path,"shaders"))
            except WindowsError:
                noonecares = True

            total_files = os.listdir(shader_path)
            i = 0
            while (i < len(total_files)):
                self.gauge.SetValue(66+(34/float(len(total_files))*i))
                shutil.copy(os.path.join(shader_path,total_files[i]),os.path.join(os.path.join(output_path,"shaders"),total_files[i]))
                i += 1

            self.gauge.SetValue(100)
            wx.FindWindowById(wx.ID_FORWARD).Enable()
