import wx
import wx.stc as stc
import wx.aui as aui
import os
import webbrowser

class MenuManager:

################################# INITALIZATION #########################################

    def __init__(self,controller,handler):
        self.IDE = controller
        self.ProjectManager = handler
        self.appname = self.IDE.appname
        
        menubar = wx.MenuBar()
        self.file = wx.Menu()
        self.edit = wx.Menu()
        self.project = wx.Menu()
        self.build = wx.Menu()
        self.options = wx.Menu()
        self.help = wx.Menu()
        self.view = wx.Menu()
        self.tasks = wx.Menu()

        self.file.Append(90, 'N&ew Project', 'Create a new project')
        wx.EVT_MENU(self.IDE, 90, self.Menu_File_NewProject)
        self.file.Append(91, 'Open Project', 'Load an existing project')
        wx.EVT_MENU(self.IDE, 91, self.Menu_File_LoadProject)
        self.file.Append(92, 'Save Project', "Saves the current project")
        wx.EVT_MENU(self.IDE, 92, self.Menu_File_SaveProject)
        self.file.Append(93, 'Close Project', "Closes the current project")
        wx.EVT_MENU(self.IDE, 93, self.Menu_File_CloseProject)
        self.file.AppendSeparator()
        self.file.Append(94, 'Save File\tCtrl-S', "Saves the current file")
        wx.EVT_MENU(self.IDE, 94, self.Menu_File_SaveFile)
        self.file.Append(99, 'Save File As', "Saves the current file")
        wx.EVT_MENU(self.IDE, 99, self.Menu_File_SaveFileAs)
        self.file.Append(95, 'Save All Files\tCtrl-D', "Saves all the current files")
        wx.EVT_MENU(self.IDE, 95, self.Menu_File_SaveAllFiles)
        self.file.AppendSeparator()
        self.file.Append(96, 'Close File', "Closes the current file")
        wx.EVT_MENU(self.IDE, 96, self.Menu_File_CloseFile)
        self.file.Append(97, 'Close All Files', "Closes all open files")
        wx.EVT_MENU(self.IDE, 97, self.Menu_File_CloseAllFiles)
        self.file.AppendSeparator()
        self.quitmenuitem = self.file.Append(98, 'Quit', "Exit Roket3D IDE")
        wx.EVT_MENU(self.IDE, 98, self.Menu_File_Quit)
        
        self.file.Enable(92,False)
        self.file.Enable(93,False)
        
        self.file.Enable(94,False)
        self.file.Enable(99,False)
        self.file.Enable(95,False)

        self.file.Enable(96,False)
        self.file.Enable(97,False)

        self.build.Append(102, 'Build Project\tF5', 'Builds the project')
        wx.EVT_MENU(self.IDE, 102, self.Menu_Project_BuildGame)
        self.build.Append(106, 'Publish Project\tCtrl-F5', 'Exports the project to multiple platforms')
        wx.EVT_MENU(self.IDE, 106, self.Menu_Project_PublishGame)
        self.build.Append(104, 'Clean Project\tF6', 'Cleans the project')
        wx.EVT_MENU(self.IDE, 104, self.Menu_Project_CleanGame)
        self.build.AppendSeparator()
        self.build.Append(103, 'Test Project\tF7', 'Tests the project')
        wx.EVT_MENU(self.IDE, 103, self.Menu_Project_TestGame)
        self.project.Append(100, 'Add Script\tCtrl+Shift+A', 'Add a new script')
        wx.EVT_MENU(self.IDE, 100, self.Menu_Project_NewScript)
        self.project.AppendSeparator()
        self.project.Append(101, 'Add Existing\tCtrl+Shift+E', 'Add an existing file to the project')
        wx.EVT_MENU(self.IDE, 101, self.Menu_Project_Existing)
##        self.project.Append(105, 'Add Extension\tCtrl+Shift+X', 'Add an extension to the project')
##        wx.EVT_MENU(self.IDE, 105, self.Menu_Project_Extension)
        self.project.Enable(100,False)
        self.project.Enable(101,False)
        self.build.Enable(102,False)
        self.build.Enable(103,False)
        self.build.Enable(104,False)
##        self.project.Enable(105,False)
        self.build.Enable(106,False)

        self.edit.Append(201, '&Copy\tCtrl+C')
        self.edit.Append(202, 'C&ut\tCtrl+X')
        self.edit.Append(203, '&Paste\tCtrl+V')
        self.edit.AppendSeparator()
        self.edit.Append(204, '&Undo\tCtrl+Z')
        self.edit.Append(205, '&Redo\tCtrl+Y')
        wx.EVT_MENU(self.IDE, 201, self.Menu_Edit_Copy)
        wx.EVT_MENU(self.IDE, 202, self.Menu_Edit_Cut)
        wx.EVT_MENU(self.IDE, 203, self.Menu_Edit_Paste)
        wx.EVT_MENU(self.IDE, 204, self.Menu_Edit_Undo)
        wx.EVT_MENU(self.IDE, 205, self.Menu_Edit_Redo)
        self.edit.Enable(201,False)
        self.edit.Enable(202,False)
        self.edit.Enable(203,False)
        self.edit.Enable(204,False)
        self.edit.Enable(205,False)

        self.help.Append(300, 'Online Help', 'View the online help documentation')
        wx.EVT_MENU(self.IDE, 300, self.Menu_Help_OpenDocs)
        self.help.Append(301, 'Submit Bug Report', 'Roket3D is still in beta.  If you encounter a bug, please submit it to the bugtracker.')
        wx.EVT_MENU(self.IDE, 301, self.Menu_Help_SubmitBug)

        menubar.Append(self.file, '&File')
        menubar.Append(self.edit, '&Edit')
##        menubar.Append(self.view, '&View')
        menubar.Append(self.project, '&Project')
        menubar.Append(self.build, '&Build')
##        menubar.Append(self.options, '&Options')
        menubar.Append(self.help, '&Help')
        self.IDE.SetMenuBar(menubar)
        projectIsLoaded = False;

##        self.IDE.StatusBar()        
        self.IDE.Centre()
        self.IDE._mgr.Update()

################################# TOGGLE FUNCTIONS ############################################

    def AddRestart(self):
        print("Adding restart option.")
        self.file.Append(89, 'Restart', "Restart Roket3D IDE")
        wx.EVT_MENU(self.IDE, 89, self.Menu_File_Restart)
        print("Removing quit option.")
        self.file.Delete(98)

    def ProjectClosed(self):
        self.file.Enable(90,True)
        self.file.Enable(91,True)
        self.file.Enable(92,False)
        self.file.Enable(93,False)
        self.file.Enable(94,False)
        self.file.Enable(99,False)
        self.file.Enable(95,False)
        self.file.Enable(96,False)
        self.file.Enable(97,False)

        self.project.Enable(100,False)
        self.project.Enable(101,False)
        self.build.Enable(102,False)
        self.build.Enable(103,False)
        self.build.Enable(104,False)
##        self.project.Enable(105,False)
        self.build.Enable(106,False)

        self.edit.Enable(201,False)
        self.edit.Enable(202,False)
        self.edit.Enable(203,False)
        self.edit.Enable(204,False)
        self.edit.Enable(205,False)
        
    def ProjectOpened(self):
        self.file.Enable(90,False)
        self.file.Enable(91,False)
        self.file.Enable(92,True)
        self.file.Enable(93,True)
        self.file.Enable(94,False)
        self.file.Enable(99,False)
        self.file.Enable(95,False)
        self.file.Enable(96,False)
        self.file.Enable(97,False)

        self.project.Enable(100,True)
        self.project.Enable(101,True)
        self.build.Enable(102,True)
        self.build.Enable(103,True)
        self.build.Enable(104,True)
##        self.project.Enable(105,True)
        self.build.Enable(106,True)

    def FileOpened(self):
        self.file.Enable(94,True)
        self.file.Enable(99,True)
        self.file.Enable(95,True)
        self.file.Enable(96,True)
        self.file.Enable(97,True)

        self.edit.Enable(201,True)
        self.edit.Enable(202,True)
        self.edit.Enable(203,True)
        self.edit.Enable(204,True)
        self.edit.Enable(205,True)

    def FileClosed(self):
        if (self.IDE.notebook.GetPageCount() == 1):
            self.file.Enable(94,False)
            self.file.Enable(99,False)
            self.file.Enable(95,False)
            self.file.Enable(96,False)
            self.file.Enable(97,False)

            self.edit.Enable(201,False)
            self.edit.Enable(202,False)
            self.edit.Enable(203,False)
            self.edit.Enable(204,False)
            self.edit.Enable(205,False)

################################# MENU ITEM FUNCTIONS #########################################


    def Menu_File_NewProject(self, event):
        self.ProjectManager.NewProject()
        if (self.ProjectManager.ProjectLoaded):
            self.ProjectOpened()
        return 0

    def Menu_File_LoadProject(self, event):
        self.ProjectManager.LoadProject()
        if (self.ProjectManager.ProjectLoaded):
            self.ProjectOpened()
        return 0

    def Menu_File_SaveProject(self, event):
        self.ProjectManager.SaveProject()
        return 0

    def Menu_File_CloseProject(self, event):
        self.ProjectManager.CloseProject()
        if not (self.ProjectManager.ProjectLoaded):
            self.ProjectClosed()
        return 0

    def Menu_File_SaveFile(self, event):
        self.ProjectManager.SaveFile()
        return 0

    def Menu_File_SaveFileAs(self, event):
        self.ProjectManager.SaveFileAs()
        return 0

    def Menu_File_SaveAllFiles(self, event):
        self.ProjectManager.SaveAllFiles()
        return 0

    def Menu_File_CloseFile(self, event):
        self.ProjectManager.CloseFile()
        return 0

    def Menu_File_CloseAllFiles(self, event):
        self.ProjectManager.CloseAllFiles()
        return 0

    def Menu_File_Quit(self, event):
        if (self.ProjectManager.ProjectLoaded):
            self.ProjectManager.CloseProject()
        if not (self.ProjectManager.ProjectLoaded):
            self.IDE.Close()
        return 0

    def Menu_File_Restart(self,event):
        if (self.ProjectManager.ProjectLoaded):
            self.ProjectManager.CloseProject()
        if not (self.ProjectManager.ProjectLoaded):
            self.IDE.Close()
            import sys
            import getopt
            wx.CallLater(100,os.execl(sys.executable, sys.executable, self.appname))

    def Menu_Edit_Copy(self, event):
        self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection()).Copy()
        return 0

    def Menu_Edit_Cut(self, event):
        self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection()).Cut()
        return 0

    def Menu_Edit_Paste(self, event):
        self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection()).Paste()
        return 0

    def Menu_Edit_Undo(self, event):
        self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection()).Undo()
        return 0

    def Menu_Edit_Redo(self, event):
        self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection()).Redo()
        return 0



    def Menu_Project_BuildGame(self, event):
        self.ProjectManager.BuildProject()
        return 0

    def Menu_Project_TestGame(self, event):
        self.ProjectManager.TestProject()
        return 0

    def Menu_Project_PublishGame(self, event):
        self.ProjectManager.PublishProject()
        return 0

    def Menu_Project_CleanGame(self, event):
        self.ProjectManager.CleanProject()
        return 0

    def Menu_Project_NewScript(self, event):
        self.ProjectManager.AddScript()
        return 0

    def Menu_Project_Existing(self, event):
        self.ProjectManager.AddExisting()
        return 0

    def Menu_Project_Extension(self, event):
        self.ProjectManager.AddExtension()
        return 0



    def Menu_Help_OpenDocs(self, event):
        webbrowser.open("http://www.roket-productions.com/docs/roket3d/",2)
        return 0

    def Menu_Help_SubmitBug(self, event):
        webbrowser.open("http://www.roket3d.com/bugtracker/",2)
        return 0

