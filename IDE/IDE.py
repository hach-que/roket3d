#Roket 3D Test 2

import wx
import wx.stc as stc
import wx.aui as aui
import os
import array
import shutil
import keyword
import sys
from xml.sax import make_parser, SAXException
from xml.sax.handler import ContentHandler        
            
import IDE_texteditor
import IDE_projectmanager
import IDE_projectreader
import IDE_autodownload
import IDE_menumanager
import IDE_dialog_gettingstarted
import IDE_customstatusbar
##import IDE_worldeditor
import IDE_taskmanager

class MainWindow(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title, size=wx.Size(800,600))

        self.Maximize()
        self.appname = sys.argv[0]
        self.appdir = os.path.dirname(sys.argv[0])
        
        # Attempt to calculate the project directory on the computer
        try:
            from win32com.shell import shell
            df = shell.SHGetDesktopFolder()
            pidl = df.ParseDisplayName(0, None,"::{450d8fba-ad25-11d0-98a8-0800361b1103}")[1]
            mydocs = shell.SHGetPathFromIDList(pidl)
        except:
            mydocs = os.path.expanduser('~')

        self.projectdir = mydocs + "/Roket3D Projects"
        if not (os.path.exists(self.projectdir)):
            try:
                os.mkdir(self.projectdir)
            except:
                dlg = wx.MessageDialog(self, "The IDE was unable to create the following directory:\n\n" + self.projectdir + "\n\nThe IDE may not operate as expected.  Please create the directory and set the permissions manually.",
                               'Error!',
                               wx.OK | wx.ICON_ERROR 
                               #wx.YES_NO | wx.ERROR  | wx.CANCEL | wx.ICON_INFORMATION
                               )
                dlg.ShowModal()
                dlg.Destroy()

        ##Init
        self.last_name_saved = 'Empty'
        self.modified = False
        self.currentpagesopen = 0

        # Create all our objects
        self._mgr = wx.aui.AuiManager()
        self._mgr.SetManagedWindow(self)
        self.projectman = IDE_projectmanager.ProjectManager(self, -1, self)
        self.handler = IDE_projectreader.ProjectXMLHandler(self,self.projectman)
        self.taskman = IDE_taskmanager.TaskManager(self, -1, self)
        self.notebook = aui.AuiNotebook(self, -1)

        # Setup the working environment
        self._mgr.AddPane(self.projectman, wx.aui.AuiPaneInfo().BestSize(wx.Size(200,600)).Name("Project Manager").Caption("Project Manager").Left().Layer(1).Position(1).CloseButton(False).MaximizeButton(True))
        self._mgr.AddPane(self.notebook, wx.aui.AuiPaneInfo().Name("Notebook").CenterPane())
        self._mgr.AddPane(self.taskman, wx.aui.AuiPaneInfo().BestSize(wx.Size(200,150)).Name("Task Manager").Caption("Task Manager").Bottom().Layer(1).Position(1).CloseButton(False).MaximizeButton(True))

        # Bind events
        self.Bind(wx.EVT_CLOSE, self.OnQuitX, self)
        self.Bind(wx.EVT_TREE_ITEM_ACTIVATED, self.projectman.OnActivate, self.projectman)
        self.Bind(wx.aui.EVT_AUINOTEBOOK_PAGE_CLOSE, self.projectman.OnPageClose, self.notebook)

        # Add our menu manager (handles all the menu items)
        self.menumanager = IDE_menumanager.MenuManager(self, self.projectman)
        
        # Getting started
        self.gettingstarteddlg = IDE_dialog_gettingstarted.GettingStartingDialog(self)
        wx.CallLater(100, self.gettingstarteddlg.ShowModal)

        # Download check
        self.statusbar = IDE_customstatusbar.IDEStatusBar(self)
        self.SetStatusBar(self.statusbar)
        self.autoupdater = IDE_autodownload.AutoDownloader(self,self.statusbar.GetGauge(),self.statusbar)

        # Keyboard shortcuts
        self.Bind(wx.EVT_KEY_UP, self.OnKeyUp)


################################# PROJECT FUNCTIONS ###########################################

    def OnAddNewScript(self, event):
        print("New Script")
        return 1

    def OnAddExistingScript(self, event):
        print("Existing Script")
        return 1

    def SaveProject(self, event):
        return 0

    def CloseProject(self, event):
        return 0

    def OnQuit(self, event):
        self.Close()

    def OnQuitX(self, event):
        if self.projectman.ProjectLoaded:
            self.projectman.CloseProject()
            if self.projectman.ProjectLoaded:
                event.Veto(True)
            else:
                self.Destroy()
        else:
            self.Destroy()

    def OnKeyUp(self, event):
        if (event.GetKeyCode() > 256):
            return
        if chr(event.GetKeyCode()) == "S" and event.ControlDown():
            if self.notebook.GetPageCount() > 0:
                self.projectman.SaveFile()
##
##    def OnImportSource(self, event):
##        if self.notebook.GetPage(self.notebook.GetSelection()).text_changed:
##            dlg = ImportCheck(self, -1, 'Save Changes')
##            val = dlg.ShowModal()
##        else:
##            self.DoImportSource()
##
##    def DoImportSource(self):
##        dir = self.IDE.appdir
##        wcd = 'R3D Source Files (*.rks,*.rka,*.rkt,*.rkw)|*.rks;*.rka;*.rkt;*.rkw|Plain Text (*.txt)|*.txt|All Files(*)|*'
##        open_dlg = wx.FileDialog(self, message='Import file...', defaultDir=dir, defaultFile='', wildcard=wcd, style=wx.OPEN | wx.CHANGE_DIR)
##        if open_dlg.ShowModal() == wx.ID_OK:
##            path = open_dlg.GetPath()
##
##            try:
##                file = open(path, 'r')
##                text = file.read()
##                file.close()
##                # Get the wxWindow for the current tab, get the first child (textarea)
##                self.notebook.AddPage(MySTC(self.notebook, 1000, '', 1), os.path.basename(self.notebook.GetPage(self.notebook.GetSelection()).path), select = 1)
##                c = self.notebook.GetPage(self.notebook.GetSelection())
##                c.ClearAll()
##                c.AddText(text)
##                c.path = path                
##                self.notebook.SetPageText(self.notebook.GetSelection(),os.path.basename(path))
##                if str(os.path.basename(path)).endswith('.rks'):
##                    print 'File is a .rks'
##                    self.handler.scriptlist.append(self.projectman.AppendItem(self.handler.scripts, str(os.path.basename(path)), 51))
##                elif str(os.path.basename(path)).endswith('.rkt'):
##                    print 'File is a .rkt'
##                    self.handler.templatelist.append(self.projectman.AppendItem(self.handler.templates, str(os.path.basename(path)), 51))
##                elif str(os.path.basename(path)).endswith('.rka'):
##                    print 'File is a .rka'
##                    self.handler.arealist.append(self.projectman.AppendItem(self.handler.areas, str(os.path.basename(path)), 51))
##                elif str(os.path.basename(path)).endswith('.rkw'):
##                    print 'File is a .rkw'
##                    self.handler.worldlist.append(self.projectman.AppendItem(self.handler.worlds, str(os.path.basename(path)), 51))
##
##                try:
##                    p = self.projectman.GetItemText(self.handler.dir) + '\\' + os.path.basename(path)
##                    file = open( p, 'w')
##                    print self.projectman.GetItemText(self.handler.dir)
##                    print p
##                    # Get the wxWindow for the current tab, get the first child (textarea), and get it's text.
##                    text = self.notebook.GetPage(self.notebook.GetSelection()).GetText()
##                    file.write(text)
##                    file.close()
##                    self.statusbar.SetStatusText(p + ' successfully saved.', 0)
##                    self.notebook.SetPageText(self.notebook.GetSelection(),os.path.basename(path))
##                    self.modified = False
##                    self.statusbar.SetStatusText('', 1)
##
##                except IOError, error:
##                    dlg = wx.MessageDialog(self, 'Error saving file\n' + str(error))
##                    dlg.ShowModal()
##
##                #self.last_name_saved = path
##                self.statusbar.SetStatusText('Successfully opened ' + path, 0)
##                self.modified = False
##                self.statusbar.SetStatusText('', 1)
##            except IOError, error:
##                dlg = wx.MessageDialog(self, 'Error opening file ' + str(error))
##                dlg.ShowModal()
##            except UnicodeDecodeError, error:
##                dlg = wx.MessageDialog(self, 'Error opening file ' + str(error))
##                dlg.ShowModal()
##
##        open_dlg.Destroy()
##
##    def OnTextChanged(self, event):
##        self.modified = True
##        self.statusbar.SetStatusText('Modified', 1)
##        event.Skip()
##        
##class ImportCheck(wx.Dialog):
##    def __init__(self, parent, id, title):
##        wx.Dialog.__init__(self, parent, id, title, size=wx.Size(320,120))
##
##        vbox = wx.BoxSizer(wx.VERTICAL)
##        stline = wx.StaticText(self, 1234, 'Save changes?')
##        vbox.Add(stline, 1, wx.ALIGN_CENTER|wx.TOP, 10)
##        hbox = wx.BoxSizer(wx.HORIZONTAL)
##        hbox.Add(wx.Button(self, 102, 'Save File'), 1, wx.ALL, 2)
##        hbox.Add(wx.Button(self, 103, 'Save File As...'), 1, wx.ALL, 2)
##        hbox.Add(wx.Button(self, 123, 'Discard changes'), 1, wx.ALL, 2)
##        vbox.Add(hbox, 1, wx.ALIGN_CENTER)
##        vbox.Add(wx.Button(self, 321, 'Cancel'), 1, wx.ALIGN_CENTER) 
##        self.SetSizer(vbox)
##        self.Bind(wx.EVT_BUTTON, self.Discard, id=123)
##        self.Bind(wx.EVT_BUTTON, self.Save, id=102)
##        self.Bind(wx.EVT_BUTTON, self.SaveAs, id=103)
##        self.Bind(wx.EVT_BUTTON, self.Cancel, id=321)
##
##    def Cancel(self, event):
##        self.Destroy()
##
##    def Discard(self, event):
##        self.Destroy()
##        frame.DoOpen()
##
##    def Save(self, event):
##        frame.OnSave(event)
##        self.Destroy()
##        frame.DoOpen()
##
##    def SaveAs(self, event):
##        frame.OnSaveAs(event)
##        self.Destroy()
##        frame.DoOpen()
##
##class SaveCheck(wx.Dialog):
##    def __init__(self, parent, id, title):
##        wx.Dialog.__init__(self, parent, id, title, size=wx.Size(320,120))
##
##        vbox = wx.BoxSizer(wx.VERTICAL)
##        stline = wx.StaticText(self, 1234, 'Save changes?')
##        vbox.Add(stline, 1, wx.ALIGN_CENTER|wx.TOP, 10)
##        hbox = wx.BoxSizer(wx.HORIZONTAL)
##        hbox.Add(wx.Button(self, 102, 'Save File'), 1, wx.ALL, 2)
##        hbox.Add(wx.Button(self, 103, 'Save File As...'), 1, wx.ALL, 2)
##        hbox.Add(wx.Button(self, 123, 'Discard changes'), 1, wx.ALL, 2)
##        vbox.Add(hbox, 1, wx.ALIGN_CENTER)
##        vbox.Add(wx.Button(self, 321, 'Cancel'), 1, wx.ALIGN_CENTER_HORIZONTAL) 
##        self.SetSizer(vbox)
##        self.Bind(wx.EVT_BUTTON, self.Discard, id=123)
##        self.Bind(wx.EVT_BUTTON, self.Save, id=102)
##        self.Bind(wx.EVT_BUTTON, self.SaveAs, id=103)
##        self.Bind(wx.EVT_BUTTON, self.Cancel, id=321)
##
##    def Cancel(self, event):
##        self.Destroy()
##
##    def Discard(self, event):
##        self.Destroy()
##        frame.Close()
##
##    def Save(self, event):
##        frame.OnSave(event)
##        self.Destroy()
##        frame.Close()
##
##    def SaveAs(self, event):
##        frame.OnSaveAs(event)
##        self.Destroy()
##        frame.Close()

class MyApp(wx.App):
    def OnInit(self):
        global frame
        frame = MainWindow(None, -1, 'Roket3D Development Environment')
        frame.Show(True)
        return True

# First check to see if there are any patches into todopatches
__appdir = os.path.dirname(sys.argv[0])
if (os.path.exists(__appdir + "/todopatches")):
    for patch in os.listdir(__appdir + "/todopatches"):
        # Copy the new files
        print("Installing patch " + patch + "...");
        for basedir, createdirs, copyfiles in os.walk(__appdir + "/todopatches/" + patch):
            extdir = basedir[len(__appdir + "/todopatches/" + patch):]
            
            for d in createdirs:
                tofile = os.path.join(__appdir + extdir, d)
                try:
                    os.mkdir(tofile)
                except WindowsError:
                    noonecares = True
                
            for f in copyfiles:
                fromfile = os.path.join(basedir, f)
                tofile = os.path.join(__appdir + extdir, f)
                shutil.copy(fromfile,tofile)

        # Delete the patch
        print("Cleaning up patch " + patch + "...");
        for root, dirs, files in os.walk(__appdir + "/todopatches/" + patch,topdown=False):
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                try:
                    os.rmdir(os.path.join(root, name))
                except OSError:
                    os.remove(os.path.join(root, name))
        os.rmdir(__appdir + "/todopatches/" + patch)
    os.rmdir(__appdir + "/todopatches")

    print("Restarting... (" + sys.argv[0] + ")")
    os.execl(sys.argv[0], sys.argv[0])
else:
    app = MyApp(0)
    app.MainLoop()
