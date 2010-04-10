import wx
import os
import shutil
import subprocess
import thread
import sys
from xml.sax import make_parser, SAXException

import IDE_dialog_newproject
import IDE_dialog_newfile
import IDE_dialog_installextensions
import IDE_dialog_publishwizard
import IDE_texteditor

class ProjectManager(wx.TreeCtrl):
    def __init__(self, parent, id, controller):
        wx.TreeCtrl.__init__(self, parent, id)

        self.IDE = controller
        self.ProjectLoaded = False
        self.ProjectFile = ''
        self.ProjectRunning = False

        isz = (16,16)
        il = wx.ImageList(isz[0], isz[1])
        
        self.fldridx     = il.Add(wx.ArtProvider_GetBitmap(wx.ART_FOLDER,      wx.ART_OTHER, isz))
        self.fldropenidx = il.Add(wx.ArtProvider_GetBitmap(wx.ART_FILE_OPEN,   wx.ART_OTHER, isz))
        self.filegen     = il.Add(wx.ArtProvider_GetBitmap(wx.ART_NORMAL_FILE, wx.ART_OTHER, isz))
        
        self.filerks = il.AddIcon(wx.Icon(self.IDE.appdir+"/Data/images/ico_rks.ico", wx.BITMAP_TYPE_ICO)) #, wx.BITMAP_TYPE_BMP
        self.filer3d = il.AddIcon(wx.Icon(self.IDE.appdir+"/Data/images/ico_r3d.ico", wx.BITMAP_TYPE_ICO))
        self.filerkt = il.AddIcon(wx.Icon(self.IDE.appdir+"/Data/images/ico_rkt.ico", wx.BITMAP_TYPE_ICO))
        self.filerka = il.AddIcon(wx.Icon(self.IDE.appdir+"/Data/images/ico_rka.ico", wx.BITMAP_TYPE_ICO))
        self.filerkw = il.AddIcon(wx.Icon(self.IDE.appdir+"/Data/images/ico_rkw.ico", wx.BITMAP_TYPE_ICO))
        self.fileextra = il.AddIcon(wx.Icon(self.IDE.appdir+"/Data/images/ico_extra.ico", wx.BITMAP_TYPE_ICO))

        self.SetImageList(il)
        self.il = il
        self.Bind(wx.EVT_TREE_ITEM_RIGHT_CLICK, self.__ShowContextMenu)

########################################### PROJECT FUNCTIONS ######################################

    def OnActivate(self, event):
        if not (self.GetItemText(self.GetSelection()).endswith(".rks")):
            return;
        path = self.base_location + "/" + self.data_location + "/" + self.GetItemText(self.GetSelection())

        if (self.__CheckIsFileOpen(path) != -1):
            return;

##        edit = IDE_worldeditor.WorldEditor_Editor(self.notebook)
##        self.notebook.AddPage(edit, "Test World", select = 1)
##        return

        try:
            file = open(path, 'r')
            text = file.read()
            file.close()
            # Get the wxWindow for the current tab, get the first child (textarea)
            self.IDE.notebook.AddPage(IDE_texteditor.TextEditor(self.IDE.notebook, 1000, '', 1), os.path.basename(path), select = 1)
            c = self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection())
            c.ClearAll()
            c.AddText(text)
            c.StyleAllLines(self.IDE.taskman, path)
            c.path = path
            self.IDE.notebook.SetPageText(self.IDE.notebook.GetSelection(),os.path.basename(path))
            c.modified = False
            wx.CallLater(100,c.AllowSaveMark)
            self.IDE.menumanager.FileOpened()
        except IOError, error:
            dlg = wx.MessageDialog(self, 'Error opening file ' + str(error))
            dlg.ShowModal()
        except UnicodeDecodeError, error:
            dlg = wx.MessageDialog(self, 'Error opening file ' + str(error))
            dlg.ShowModal()

    def OnPageClose(self, event):
        self.IDE.menumanager.FileClosed()
        if not self.AskSaveFile():
            event.Skip()

    def NewProject(self):
        dlg = IDE_dialog_newproject.NewProjectDialog(self.__NewProject_CallBack_Okay, self.__NewProject_CallBack_Cancel, self.IDE)
        dlg.ShowModal()
        return 0

    def LoadProject(self):
        dir = self.IDE.projectdir+"/"
        wcd = 'R3D Project Files (*.r3d)|*.r3d|All Files(*)|*'
        open_dlg = wx.FileDialog(self, message='Load Project', defaultDir=dir, defaultFile='', wildcard=wcd, style=wx.OPEN | wx.CHANGE_DIR)
        open_dlg.CenterOnParent(wx.BOTH)
        open_dlg.CenterOnScreen(wx.BOTH)
        if open_dlg.ShowModal() == wx.ID_OK:
            path = open_dlg.GetPath()
            self.__LoadProject(path)
        return 0

    def SaveProject(self):
        self.__SaveProject()
        return 0

    def CloseProject(self):
        self.__CloseProject()
        return 0

########################################### ADD XXXXXX FUNCTIONS ######################################

    def AddScript(self):
        dlg = IDE_dialog_newfile.NewFileDialog(self.__AddFile_CallBack_Okay, self.__AddFile_CallBack_Cancel, self.IDE)
        dlg.ShowModal()
        return 0

    def AddExisting(self):
        dir = self.IDE.projectdir+"/"
        wcd =  'All Valid Files (*.rks;*.3ds;*.b3d;*.csm;*.dae;*.dmf;*.x;*.obj;*.my3d;*.oct;*.mesh;*.lmts;*.bsp;*.md2;*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.pcx;*.psd)|*.rks;*.3ds;*.b3d;*.csm;*.dae;*.dmf;*.x;*.obj;*.my3d;*.oct;*.mesh;*.lmts;*.bsp;*.md2;*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.pcx;*.psd|';
        wcd += '3D Models (*.3ds;*.b3d;*.csm;*.dae;*.dmf;*.x;*.obj;*.my3d;*.oct;*.mesh;*.lmts;*.bsp;*.md2)|*.3ds;*.b3d;*.csm;*.dae;*.dmf;*.x;*.obj;*.my3d;*.oct;*.mesh;*.lmts;*.bsp;*.md2|';
        wcd += 'Images (*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.pcx;*.psd)|*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.pcx;*.psd|'
        wcd += 'Roket3D Script (*.rks)|*.rks|';
        wcd += 'All Files (*.*)|*.*'
        open_dlg = wx.FileDialog(self, message='Add Existing File', defaultDir=dir, defaultFile='', wildcard=wcd, style=wx.OPEN | wx.CHANGE_DIR)
        open_dlg.CenterOnParent(wx.BOTH)
        open_dlg.CenterOnScreen(wx.BOTH)
        if open_dlg.ShowModal() == wx.ID_OK:
            path = open_dlg.GetPath()
            print("comparing " + os.path.abspath(self.base_location + "/" + self.data_location + "/" + os.path.basename(path)) + " with " + os.path.abspath(path))
            if (os.path.abspath(self.base_location + "/" + self.data_location + "/" + os.path.basename(path)) == os.path.abspath(path)):
                self.__AddFile(os.path.basename(path))
            elif not (os.path.exists(self.base_location + "/" + self.data_location + "/" + os.path.basename(path))):
                shutil.copy(path,self.base_location + "/" + self.data_location + "/" + os.path.basename(path))
                self.__AddFile(os.path.basename(path))
            else:
                dlg = wx.MessageDialog(self, "File already exists!  A file with the same name as the one you selected already exists in the project.  Rename the file in the source directory, and try again.","Error",wx.OK)
                dlg.ShowModal()
        return 0

    def AddExtension(self):
        dlg = IDE_dialog_installextensions.InstallExtensionsDialog(self.__AddExtension_CallBack_Install, self.__AddExtension_CallBack_Cancel, self.IDE)
        dlg.ShowModal()
        return 0

########################################### RUN ROKET3D FUNCTIONS ##########################################

    def BuildProject(self):
        if (self.ProjectRunning):
            return 0
        gauge = self.IDE.statusbar.GetGauge()
        statusbar = self.IDE.statusbar
        self.__BuildProject(gauge,statusbar)

    def CleanProject(self):
        if (self.ProjectRunning):
            return 0
        gauge = self.IDE.statusbar.GetGauge()
        statusbar = self.IDE.statusbar
        self.__CleanProject(gauge,statusbar)

    def PublishProject(self):
        if (self.ProjectRunning):
            return 0
        IDE_dialog_publishwizard.StartWizard(self.IDE)
        return 0

    def TestProject(self):
        self.ProjectRunning = True
        # build
        gauge = self.IDE.statusbar.GetGauge()
        statusbar = self.IDE.statusbar
        self.__BuildProject(gauge,statusbar)

        # test
        self.IDE.statusbar.SetStatusText("Game Started.", 0)
        self.IDE.statusbar.GetGauge().SetValue(0)
        thread.start_new_thread(self.__RunGame, ())

########################################### FILE MANAGEMENT FUNCTIONS ######################################

    def SaveFile(self):
        self.__SaveFile()

    def SaveFileAs(self):
        self.__SaveFileAs()

    def SaveAllFiles(self):
        self.__SaveAllFiles(False)

    def NewFile(self):
        self.IDE.notebook.AddPage(MySTC(self.notebook, 1000, '', 1), os.path.basename(self.notebook.GetPage(self.notebook.GetSelection()).path), select = 1)

    def AskSaveFile(self):
        return self.__SaveAskFile()

    def CloseFile(self):
        self.IDE.menumanager.FileClosed()
        if self.IDE.projectman.AskSaveFile() != 1:
            self.IDE.notebook.DeletePage(self.IDE.notebook.GetSelection())

    def CloseAllFiles(self):
        for i in range(self.IDE.notebook.GetPageCount()):
            if self.__SaveAskFile() == 1:
                # Cancel
                return 0
            else:
                self.IDE.notebook.DeletePage(self.IDE.notebook.GetSelection())

    def DeleteFile(self, event):
        # Prompt for deletion from project
        currentMenuItem = self.GetSelection()
        if (not currentMenuItem.IsOk()):
            return 0
        if (currentMenuItem == self.root or self.GetItemParent(currentMenuItem) == self.root):
            return 0

        dlg = wx.MessageDialog(self, 'Are you sure you wish to delete ' + self.GetItemText(currentMenuItem) + '?\nThe file will be permenantly deleted from your harddisk.\nThis action cannot be undone!',
                               'Confirm Delete',
                               wx.YES_NO | wx.ICON_EXCLAMATION
                               #wx.YES_NO | wx.NO_DEFAULT | wx.CANCEL | wx.ICON_INFORMATION
                               )
        if (self.GetItemText(currentMenuItem) == ""):
            self.__DeleteFile(currentMenuItem)
        else:
            if (dlg.ShowModal() == wx.ID_YES):
                self.__DeleteFile(currentMenuItem)
        dlg.Destroy()

########################################### INTERNAL FUNCTIONS ######################################

    def __NewProject(self,name):
        if (name == ""):
            dlg = wx.MessageDialog(self, "Invalid name for project!","Error",wx.OK)
            dlg.ShowModal()
            self.NewProject()
            return 0
            
        path = self.IDE.projectdir+"/"+name
        if (os.path.exists(path)):
            dlg = wx.MessageDialog(self, "Project already exists!  Please choose another name.","Error",wx.OK)
            dlg.ShowModal()
            self.NewProject()
            return 0
        
        try:
            os.mkdir(path)
        except IOError:
            noonecares = True
        except:
            dlg = wx.MessageDialog(self, "Unable to create project directory!","Error",wx.OK)
            dlg.ShowModal()
            return 0

        try:
            os.mkdir(path+"/Output")
        except IOError:
            noonecares = True
        except:
            dlg = wx.MessageDialog(self, "Unable to create project output directory!","Error",wx.OK)
            dlg.ShowModal()
            return 0

        try:
            os.mkdir(path+"/Data")
        except IOError:
            noonecares = True
        except:
            dlg = wx.MessageDialog(self, "Unable to create project data directory!","Error",wx.OK)
            dlg.ShowModal()
            return 0

        r3dpath = path+"/"+name+".r3d"
        file = open(r3dpath,'w')
        file.write('<?xml version="1.0" encoding="utf-8"?>')
        file.write('<project>')
        file.write('<settings>')
        file.write('<general>')
        file.write('<name>' + name.replace("<","&lt;").replace(">","&gt;") + '</name>')
        file.write('<dir>' + path.replace("<","&lt;").replace(">","&gt;") + '</dir>')
        file.write('<output>Output</output>')
        file.write('<data>Data</data>')
        file.write('</general>')
        file.write('</settings>')
        file.write('<scripts>')
        file.write('<script>main.rks</script>')
        file.write('</scripts>')
        file.write('</project>')
        file.close()

        shutil.copy(self.IDE.appdir+"/Data/defaults/main.rks",path+"/Data/main.rks")

        # Finally, load the project
        self.__LoadProject(r3dpath)
        
        return 0

    def __NewProject_CallBack_Okay(self,name):
        self.__NewProject(name)
        return 0

    def __NewProject_CallBack_Cancel(self):
        return 0

    def __LoadProject(self,uri):
        self.DeleteAllItems()
        self.root = self.AddRoot(os.path.basename(uri)[0:-4])
        self.SetItemImage(self.root, self.fldridx, wx.TreeItemIcon_Normal)
        self.SetItemImage(self.root, self.fldropenidx, wx.TreeItemIcon_Expanded)
        self.IDE.handler.Reset()
        parser = make_parser()
        parser.setContentHandler(self.IDE.handler)
        try:
            parser.parse(open(uri))
            if not self.IDE.handler.ProjectOKAY:
                dlg = wx.MessageDialog(self, "Error loading project.  The project file is corrupt.  The project data is still intact.","Error",wx.OK | wx.ICON_ERROR)
                dlg.ShowModal()
                self.DeleteAllItems()
                self.ProjectLoaded = False
            else:
                self.ProjectLoaded = True
                self.ProjectFile = uri
                self.Data_ScriptList = self.IDE.handler.scriptlist
                self.Data_TemplateList = self.IDE.handler.templatelist
                self.Data_AreaList = self.IDE.handler.arealist
                self.Data_WorldList = self.IDE.handler.worldlist
                self.Data_ExtraList = self.IDE.handler.extralist
                self.Tree_ScriptList = self.IDE.handler.scripts
                self.Tree_TemplateList = self.IDE.handler.templates
                self.Tree_AreaList = self.IDE.handler.areas
                self.Tree_WorldList = self.IDE.handler.worlds
                self.Tree_ExtraList = self.IDE.handler.extras
                self.IDE.menumanager.ProjectOpened()
                self.ExpandAll()
        except SAXException:
            dlg = wx.MessageDialog(self, "Error loading project.  The project file is corrupt.  The project data is still intact.","Error",wx.OK | wx.ICON_ERROR)
            dlg.ShowModal()
            self.DeleteAllItems()
            self.ProjectLoaded = False
            return None
        return None

    def __CloseProject(self):
        if (self.__SaveAllFiles() == 1):
            # They pressed Cancel
            return 0
        dlg = wx.MessageDialog(self, 'Save changes to project?',
                               'Save Changes',
                               wx.YES_NO | wx.NO_DEFAULT | wx.CANCEL | wx.ICON_INFORMATION
                               )
        result = dlg.ShowModal()
        if (result == wx.ID_YES):
            # Save Changes
            self.__SaveProject()
        if (result == wx.ID_CANCEL):
            # Cancel
            return 0
        self.DeleteAllItems()
        self.IDE.menumanager.ProjectClosed()
        for i in range(self.IDE.notebook.GetPageCount()):
            self.IDE.notebook.DeletePage(self.IDE.notebook.GetSelection())
        self.ProjectLoaded = False

    def __SaveAllFiles(self,prompt=True):
        dir = self.IDE.projectdir+"/"
        count = self.IDE.notebook.GetPageCount()
        for i in range(count):
            # Loop through each page, check if it's modified, and ask to save changes.  Cancel should
            # make us return 1.
            path = self.IDE.notebook.GetPage(i).path
            modified = self.IDE.notebook.GetPage(i).modified
            if modified:
                if prompt:
                    dlg = wx.MessageDialog(self, 'Save changes to ' + os.path.basename(path) + '?',
                               'Save Changes',
                               wx.YES_NO | wx.NO_DEFAULT | wx.CANCEL | wx.ICON_INFORMATION
                               )
                    result = dlg.ShowModal()
                else:
                    result = wx.ID_YES
                if (result == wx.ID_YES):
                    # Save Changes
                    try:
                        if not path:
                            # Check to see if path is set.  If not, prompt for filename.
                            wcd = 'Roket3D Script (*.rks)|*.rks|All Files (*.*)|*.*'
                            save_dlg = wx.FileDialog(self, message='Save file as...', defaultDir=dir, defaultFile='', wildcard=wcd, style=wx.SAVE | wx.OVERWRITE_PROMPT)
                            if save_dlg.ShowModal() == wx.ID_OK:
                                path = save_dlg.GetPath()
                            else:
                                # Cancel
                                return 1
                        file = open(path, 'w')
                        text = self.IDE.notebook.GetPage(i).GetText()
                        file.write(text)
                        file.close()
                        self.IDE.notebook.GetPage(i).modified = False
                        self.IDE.notebook.SetPageText(i,os.path.basename(path))
                    except IOError, error:
                        dlg = wx.MessageDialog(self, "Error saving " + os.path.basename(path) + ".")
                        dlg.ShowModal()
                if (result == wx.ID_CANCEL):
                    # Cancel
                    return 1
        return 0

    def __SaveFile(self):
        i = self.IDE.notebook.GetSelection()
        path = self.IDE.notebook.GetPage(i).path
        if not path:
            self.__SaveFileAs()
        else:
            try:
                modified = self.IDE.notebook.GetPage(i).modified
                file = open(path, 'w')
                text = self.IDE.notebook.GetPage(i).GetText()
                file.write(text)
                file.close()
                self.IDE.notebook.GetPage(i).modified = False
                self.IDE.notebook.SetPageText(i,os.path.basename(path))
            except IOError, error:
                dlg = wx.MessageDialog(self, "Error saving " + os.path.basename(path) + ".","Error",wx.OK)
                dlg.ShowModal()

    def __SaveFileAs(self):
        i = self.IDE.notebook.GetSelection()
        dir = self.IDE.projectdir+"/"
        wcd = 'Roket3D Script (*.rks)|*.rks|All Files (*.*)|*.*'
        save_dlg = wx.FileDialog(self, message='Save file as...', defaultDir=dir, defaultFile='', wildcard=wcd, style=wx.SAVE | wx.OVERWRITE_PROMPT)
        if save_dlg.ShowModal() == wx.ID_OK:
            path = save_dlg.GetPath()
        else:
            # Cancel
            return 1
        try:
            modified = self.IDE.notebook.GetPage(i).modified
            file = open(path, 'w')
            text = self.IDE.notebook.GetPage(i).GetText()
            file.write(text)
            file.close()
            self.IDE.notebook.GetPage(i).modified = False
            self.IDE.notebook.SetPageText(i,os.path.basename(path))
        except IOError, error:
            dlg = wx.MessageDialog(self, "Error saving " + os.path.basename(path) + ".","Error",wx.OK)
            dlg.ShowModal()

    def __SaveAskFile(self):
        i = self.IDE.notebook.GetSelection()
        # Ask to save the current file
        path = self.IDE.notebook.GetPage(i).path
        modified = self.IDE.notebook.GetPage(i).modified
        if modified:
            dlg = wx.MessageDialog(self, 'Save changes to ' + os.path.basename(path) + '?',
                           'Save Changes',
                           wx.YES_NO | wx.NO_DEFAULT | wx.CANCEL | wx.ICON_INFORMATION
                           )
            result = dlg.ShowModal()
            if (result == wx.ID_YES):
                # Save Changes
                try:
                    if not path:
                        # Check to see if path is set.  If not, prompt for filename.
                        save_dlg = wx.FileDialog(self, message='Save file as...', defaultDir=dir, defaultFile='', wildcard=wcd, style=wx.SAVE | wx.OVERWRITE_PROMPT)
                        if save_dlg.ShowModal() == wx.ID_OK:
                            path = save_dlg.GetPath()
                        else:
                            # Cancel
                            return 1
                    file = open(path, 'w')
                    text = self.IDE.notebook.GetPage(i).GetText()
                    file.write(text)
                    file.close()
                    self.IDE.notebook.GetPage(i).modified = False
                    self.IDE.notebook.SetPageText(i,os.path.basename(path))
                except IOError, error:
                    dlg = wx.MessageDialog(self, "Error saving " + os.path.basename(path) + ".")
                    dlg.ShowModal()
            if (result == wx.ID_CANCEL):
                # Cancel
                return 1
        return 0

    def __SaveProject(self):
        # Save the project settings
        file = open(self.ProjectFile,'w')
        file.write('<?xml version="1.0" encoding="utf-8"?>')
        file.write('<project>')
        file.write('<settings>')
        file.write('<general>')
        file.write('<name>' + self.project_name.replace("<","&lt;").replace(">","&gt;") + '</name>')
        file.write('<dir>' + self.base_location.replace("<","&lt;").replace(">","&gt;") + '</dir>')
        file.write('<output>' + self.output_location.replace("<","&lt;").replace(">","&gt;") + '</output>')
        file.write('<data>' + self.data_location.replace("<","&lt;").replace(">","&gt;") + '</data>')
        file.write('</general>')
        file.write('</settings>')
        file.write('<scripts>')
        # Get all the scripts
        for i in range(len(self.Data_ScriptList)):
            file.write('<script>' + self.GetItemText(self.Data_ScriptList[i]) + '</script>')
        file.write('</scripts>')
        file.write('<extras>')
        # Get all the scripts
        for i in range(len(self.Data_ExtraList)):
            file.write('<extra>' + self.GetItemText(self.Data_ExtraList[i]) + '</extra>')
        file.write('</extras>')
        file.write('</project>')
        file.close()

    def __AddFile(self,filename):
        # filename is relative to project data_location
        if (filename.endswith(".rks")):
            item = self.AppendItem(self.Tree_ScriptList, filename, 21)
            self.SetItemImage(item, self.filerks, wx.TreeItemIcon_Normal)
            self.SetItemImage(item, self.filerks, wx.TreeItemIcon_Expanded)
            self.Data_ScriptList.append(item)
        else:
            item = self.AppendItem(self.Tree_ExtraList, filename, 21)
            self.SetItemImage(item, self.fileextra, wx.TreeItemIcon_Normal)
            self.SetItemImage(item, self.fileextra, wx.TreeItemIcon_Expanded)
            self.Data_ExtraList.append(item)
        self.__SaveProject()

    def __AddFile_CallBack_Okay(self,name):
        path = os.path.join(self.data_location,name)
        file = open(path,'w')
        file.write('-- New File')
        file.close()
        self.__AddFile(name)
        return 0

    def __AddFile_CallBack_Cancel(self):
        return 0

    def __CleanProject(self,gauge,sb):
        gauge.SetValue(0)
        sb.SetStatusText("Cleaning output directory...", 0)
        output_path = os.path.join(self.base_location,self.output_location)
        data_path = os.path.join(self.base_location,self.data_location)
        engine_path = os.path.join(self.IDE.appdir,"Data/engine")
        shader_path = os.path.join(self.IDE.appdir,"Data/shaders")
        for root, dirs, files in os.walk(output_path,topdown=False):
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                try:
                    os.rmdir(os.path.join(root, name))
                except OSError:
                    os.remove(os.path.join(root, name))
        gauge.SetValue(0)
        sb.SetStatusText("Clean complete.", 0)

    def __BuildProject(self,gauge,sb):
        self.__SaveAllFiles(False)
        # copy all the data files to the output directory, and copy the Roket3D files to the output directory
        gauge.SetValue(0)
        sb.SetStatusText("Cleaning output directory...", 0)
        output_path = os.path.join(self.base_location,self.output_location)
        data_path = os.path.join(self.base_location,self.data_location)
        engine_path = os.path.join(self.IDE.appdir,"Data/debugengine")
        shader_path = os.path.join(self.IDE.appdir,"Data/shaders")
        for root, dirs, files in os.walk(output_path,topdown=False):
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                try:
                    os.rmdir(os.path.join(root, name))
                except OSError:
                    os.remove(os.path.join(root, name))

        sb.SetStatusText("Copying game files...", 0)
        total_files = os.listdir(data_path)
        i = 0
        while (i < len(total_files)):
            gauge.SetValue(100/float(len(total_files))*i)
            shutil.copy(os.path.join(data_path,total_files[i]),os.path.join(output_path,total_files[i]))
            i += 1

        sb.SetStatusText("Copying game engine...", 0)
        total_files = os.listdir(engine_path)
        i = 0
        while (i < len(total_files)):
            gauge.SetValue(100/float(len(total_files))*i)
            shutil.copy(os.path.join(engine_path,total_files[i]),os.path.join(output_path,total_files[i]))
            i += 1

        sb.SetStatusText("Copying shaders...", 0)
        try:
            os.mkdir(os.path.join(output_path,"shaders"))
        except WindowsError:
            noonecares = True
        gauge.SetValue(0)
        total_files = os.listdir(shader_path)
        i = 0
        while (i < len(total_files)):
            gauge.SetValue(100/float(len(total_files))*i)
            shutil.copy(os.path.join(shader_path,total_files[i]),os.path.join(os.path.join(output_path,"shaders"),total_files[i]))
            i += 1

        gauge.SetValue(0)
        sb.SetStatusText("Build complete.", 0)
        
##        for basedir, createdirs, copyfiles in os.walk(shader_path):
##            extdir = basedir[len(shader_path):]
##            
##            for d in createdirs:
##                tofile = os.path.join(os.path.join(os.path.join(output_path,"shaders"),extdir), d)
##                try:
##                    os.mkdir(tofile)
##                except WindowsError:
##                    noonecares = True
##                
##            for f in copyfiles:
##                fromfile = os.path.join(basedir, f)
##                tofile = os.path.join(os.path.join(os.path.join(output_path,"shaders"),extdir), f)
##                shutil.copy(fromfile,tofile)
        
    def __RunGame(self):
        output_path = os.path.join(self.base_location,self.output_location)
        r3dpath = os.path.join(output_path,"Roket3D.exe") # Run Roket3D
        p = subprocess.Popen("\"" + r3dpath + "\" --debug=info",cwd=output_path,shell=True, stdout=subprocess.PIPE).stdout
        gameoutput = list()
        taskoutput = list()
        while 1:
            line = p.readline()
            if not line: break
            gameoutput.append(line)
            # Output to our new textarea
        self.IDE.taskman.game_full_output = gameoutput
        for i in gameoutput:
            #print i
            if i.startswith('FATL') or i.startswith('ERR') or i.startswith('WARN') or i.startswith('INFO'):
                taskoutput.append(i)

        FATLlist = list()
        ERRlist = list()
        WARNlist = list()
        INFOlist = list()
        tempdict = {'FATL':FATLlist,'ERR':ERRlist,'WARN':WARNlist,'INFO':INFOlist}
        for i in taskoutput:
            if i.startswith('FATL'):
                FATLlist.append(i)
            elif i.startswith('ERR'):
                ERRlist.append(i)
            elif i.startswith('WARN'):
                WARNlist.append(i)
            elif i.startswith('INFO'):
                INFOlist.append(i)

        self.IDE.taskman.list.DeleteAllItems()

        # Find duplicates
        FATLdups = list()
        ERRdups = list()
        WARNdups = list()
        INFOdups = list()
        i = 0
        while i < len(FATLlist):
            FATLdups.append(1)
            a = 0
            while a < len(FATLlist):
                if (FATLlist[i] == FATLlist[a] and i != a):
                    FATLdups[i] += 1
                    FATLlist[a] = ""
                a += 1
            i += 1
        i = 0
        while i < len(ERRlist):
            ERRdups.append(1)
            a = 0
            while a < len(ERRlist):
                if (ERRlist[i] == ERRlist[a] and i != a):
                    ERRdups[i] += 1
                    ERRlist[a] = ""
                a += 1
            i += 1
        i = 0
        while i < len(WARNlist):
            WARNdups.append(1)
            a = 0
            while a < len(WARNlist):
                if (WARNlist[i] == WARNlist[a] and i != a):
                    WARNdups[i] += 1
                    WARNlist[a] = ""
                a += 1
            i += 1
        i = 0
        while i < len(INFOlist):
            INFOdups.append(1)
            a = 0
            while a < len(INFOlist):
                if (INFOlist[i] == INFOlist[a] and i != a):
                    INFOdups[i] += 1
                    INFOlist[a] = ""
                a += 1
            i += 1

        i = 0
        FATLINDEX = -1
        while i < len(FATLlist):
            if (FATLlist[i] == ""):
                i += 1
                continue
            try:
                a,b,c,d = FATLlist[i].split(':',3)
                index = self.IDE.taskman.list.InsertImageStringItem(sys.maxint, "FATAL", self.IDE.projectman.filerks)
                b = b.lstrip()
                d = d[0:-2]
                self.IDE.taskman.list.SetStringItem(index, 1, b)
                self.IDE.taskman.list.SetStringItem(index, 2, c)
                if FATLdups[i] > 1:
                    self.IDE.taskman.list.SetStringItem(index, 3, d + " (Repeated " + str(FATLdups[i]) + " times)")
                else:
                    self.IDE.taskman.list.SetStringItem(index, 3, d)
                self.IDE.taskman.list.SetItemData(index, self.IDE.taskman.list.GetItemCount()+1)
            except:
                a,b = FATLlist[i].split(':',1)
                index = self.IDE.taskman.list.InsertImageStringItem(sys.maxint, "FATAL", self.IDE.projectman.filerks)
                b = b[0:-2]
                self.IDE.taskman.list.SetStringItem(index, 1, "")
                self.IDE.taskman.list.SetStringItem(index, 2, "")
                if FATLdups[i] > 1:
                    self.IDE.taskman.list.SetStringItem(index, 3, b + " (Repeated " + str(FATLdups[i]) + " times)")
                else:
                    self.IDE.taskman.list.SetStringItem(index, 3, b)
                self.IDE.taskman.list.SetItemData(index, self.IDE.taskman.list.GetItemCount()+1)
            i += 1

        i = 0
        while i < len(ERRlist):
            if (ERRlist[i] == ""):
                i += 1
                continue
            try:
                a,b,c,d = ERRlist[i].split(':',3)
                index = self.IDE.taskman.list.InsertImageStringItem(sys.maxint, "ERROR", self.IDE.projectman.filerks)
                b = b.lstrip()
                d = d[0:-2]
                self.IDE.taskman.list.SetStringItem(index, 1, b)
                self.IDE.taskman.list.SetStringItem(index, 2, c)
                if ERRdups[i] > 1:
                    self.IDE.taskman.list.SetStringItem(index, 3, d + " (Repeated " + str(ERRdups[i]) + " times)")
                else:
                    self.IDE.taskman.list.SetStringItem(index, 3, d)
                self.IDE.taskman.list.SetItemData(index, self.IDE.taskman.list.GetItemCount()+1)
            except:
                a,b = ERRlist[i].split(':',1)
                index = self.IDE.taskman.list.InsertImageStringItem(sys.maxint, "ERROR", self.IDE.projectman.filerks)
                b = b[0:-2]
                self.IDE.taskman.list.SetStringItem(index, 1, "")
                self.IDE.taskman.list.SetStringItem(index, 2, "")
                if ERRdups[i] > 1:
                    self.IDE.taskman.list.SetStringItem(index, 3, b + " (Repeated " + str(ERRdups[i]) + " times)")
                else:
                    self.IDE.taskman.list.SetStringItem(index, 3, b)
                self.IDE.taskman.list.SetItemData(index, self.IDE.taskman.list.GetItemCount()+1)
            i += 1

        i = 0
        while i < len(WARNlist):
            if (WARNlist[i] == ""):
                i += 1
                continue
            try:
                a,b,c,d = WARNlist[i].split(':',3)
                index = self.IDE.taskman.list.InsertImageStringItem(sys.maxint, "WARN", self.IDE.projectman.filerks)
                b = b.lstrip()
                d = d[0:-2]
                self.IDE.taskman.list.SetStringItem(index, 1, b)
                self.IDE.taskman.list.SetStringItem(index, 2, c)
                if WARNdups[i] > 1:
                    self.IDE.taskman.list.SetStringItem(index, 3, d + " (Repeated " + str(WARNdups[i]) + " times)")
                else:
                    self.IDE.taskman.list.SetStringItem(index, 3, d)
                self.IDE.taskman.list.SetItemData(index, self.IDE.taskman.list.GetItemCount()+1)
            except:
                a,b = WARNlist[i].split(':',1)
                index = self.IDE.taskman.list.InsertImageStringItem(sys.maxint, "WARN", self.IDE.projectman.filerks)
                b = b[0:-2]
                self.IDE.taskman.list.SetStringItem(index, 1, "")
                self.IDE.taskman.list.SetStringItem(index, 2, "")
                if WARNdups[i] > 1:
                    self.IDE.taskman.list.SetStringItem(index, 3, b + " (Repeated " + str(WARNdups[i]) + " times)")
                else:
                    self.IDE.taskman.list.SetStringItem(index, 3, b)
                self.IDE.taskman.list.SetItemData(index, self.IDE.taskman.list.GetItemCount()+1)
            i += 1

        i = 0
        while i < len(INFOlist):
            if (INFOlist[i] == ""):
                i += 1
                continue
            try:
                a,b,c,d = INFOlist[i].split(':',3)
                index = self.IDE.taskman.list.InsertImageStringItem(sys.maxint, "INFO", self.IDE.projectman.filerks)
                b = b.lstrip()
                d = d[0:-2]
                self.IDE.taskman.list.SetStringItem(index, 1, b)
                self.IDE.taskman.list.SetStringItem(index, 2, c)
                if INFOdups[i] > 1:
                    self.IDE.taskman.list.SetStringItem(index, 3, d + " (Repeated " + str(INFOdups[i]) + " times)")
                else:
                    self.IDE.taskman.list.SetStringItem(index, 3, d)
                self.IDE.taskman.list.SetItemData(index, self.IDE.taskman.list.GetItemCount()+1)
            except:
                a,b = INFOlist[i].split(':',1)
                index = self.IDE.taskman.list.InsertImageStringItem(sys.maxint, "INFO", self.IDE.projectman.filerks)
                b = b[0:-2]
                self.IDE.taskman.list.SetStringItem(index, 1, "")
                self.IDE.taskman.list.SetStringItem(index, 2, "")
                if INFOdups[i] > 1:
                    self.IDE.taskman.list.SetStringItem(index, 3, b + " (Repeated " + str(INFOdups[i]) + " times)")
                else:
                    self.IDE.taskman.list.SetStringItem(index, 3, b)
                self.IDE.taskman.list.SetItemData(index, self.IDE.taskman.list.GetItemCount()+1)
            i += 1

        self.__RefreshStyling()
        self.ProjectRunning = False

        

    def __AddExtension_CallBack_Install(self,name):
        return 0

    def __AddExtension_CallBack_Cancel(self):
        return 0

    def __RefreshStyling(self):
        # Runs through all the open files and resets all their styling.
        total = self.IDE.notebook.GetPageCount()
        i = 0
        while i < total:
            self.IDE.notebook.GetPage(i).ResetStyles()
            self.IDE.notebook.GetPage(i).StyleAllLines(self.IDE.taskman, self.IDE.notebook.GetPage(i).path)
            i += 1

    def CheckIsFileOpen(self, path):
        return self.__CheckIsFileOpen(path);

    def __CheckIsFileOpen(self, path):
        # Focuses the tab with the path if it is already open (and returns it's ID), or returns -1
        total = self.IDE.notebook.GetPageCount()
        i = 0
        while i < total:
            c1 = os.path.abspath(self.IDE.notebook.GetPage(i).path)
            c2 = os.path.abspath(path)
##            c1.replace("\\","/")
##            c1.replace("/./","/")
##            c1.replace("\\","/")
##            c1.replace("/./","/")
            if (c1 == c2):
                self.IDE.notebook.SetSelection(i)
                return i
            i += 1
        return -1

    def __ShowContextMenu(self, event):
        currentMenuItem = event.GetItem()
        self.SelectItem(currentMenuItem)
        if (not currentMenuItem.IsOk()):
            return 0
        if (currentMenuItem == self.root or self.GetItemParent(currentMenuItem) == self.root):
            return 0

        if not hasattr(self, "popupID1"):
            self.popupDELETE = wx.NewId()
            self.Bind(wx.EVT_MENU, self.DeleteFile, id=self.popupDELETE)

        # Show delete menu
        menu = wx.Menu()
        menu.Append(self.popupDELETE, "Delete Item")
        self.PopupMenu(menu)
        menu.Destroy()
        
        return 0

    def __DeleteFile(self, currentMenuItem):
        if (self.GetItemParent(currentMenuItem) != self.Tree_ScriptList and self.GetItemParent(currentMenuItem) != self.Tree_ExtraList):
            return 0
        
        if (self.GetItemText(currentMenuItem) != ""):
            data_path = os.path.join(self.base_location,self.data_location)
            if (os.path.exists(data_path + "/" + self.GetItemText(currentMenuItem))):
                try:
                    os.remove(data_path + "/" + self.GetItemText(currentMenuItem))
                except:
                    NooneCares = True

        if (self.GetItemParent(currentMenuItem) == self.Tree_ScriptList):
            self.Data_ScriptList.remove(currentMenuItem)
        if (self.GetItemParent(currentMenuItem) == self.Tree_ExtraList):
            self.Data_ExtraList.remove(currentMenuItem)
        
        self.Delete(currentMenuItem)
        self.__SaveProject()
    
##                self.projectman.SetItemImage(handler.scriptlist[:], self.fldridx, wx.TreeItemIcon_Normal)
##                #self.last_name_saved = path
##                self.statusbar.SetStatusText('Successfully opened ' + path, 0)
##            except IOError, error:
##                dlg = wx.MessageDialog(self, 'Error opening file ' + str(error))
##                dlg.ShowModal()
##            except UnicodeDecodeError, error:
##                dlg = wx.MessageDialog(self, 'Error opening file ' + str(error))
##                dlg.ShowModal()
##
##            self.projectman.SetItemImage(self.handler.scriptlist, self.filerksm, wx.TreeItemIcon_Normal)
