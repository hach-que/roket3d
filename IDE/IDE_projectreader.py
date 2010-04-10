from xml.sax import make_parser, SAXException
from xml.sax.handler import ContentHandler
import wx

class ProjectXMLHandler(ContentHandler):
    def __init__(self,ide,manager):
        self.IDE = ide
        self.MANAGER = manager
        self.section_type = ''
        self.element_type = ''
        self.setting_type = ''
        self.buffer = ''
        self.cur_element = ''
        self.scriptlist = list()
        self.templatelist = list()
        self.extralist = list()
        self.arealist = list()
        self.worldlist = list()
        self.scripts = None
        self.settings = None
        self.templates = None
        self.areas = None
        self.worlds = None
        self.extras = None
        self.ProjectOKAY = False
        self.i = 1

    def Reset(self):
        self.section_type = ''
        self.element_type = ''
        self.setting_type = ''
        self.buffer = ''
        self.cur_element = ''
        self.scriptlist = list()
        self.templatelist = list()
        self.extralist = list()
        self.arealist = list()
        self.worldlist = list()
        self.scripts = None
        self.settings = None
        self.templates = None
        self.areas = None
        self.worlds = None
        self.extras = None
        self.ProjectOKAY = False
        self.i = 1

    def startElement(self, name, attrs):
        self.ProjectOKAY = False
        self.buffer = ''
        if name == 'project':
##            self.settings = self.MANAGER.AppendItem(self.MANAGER.root, 'Settings', 1)
##            self.MANAGER.SetItemImage(self.settings, self.MANAGER.filegen, wx.TreeItemIcon_Normal)
##            self.MANAGER.SetItemImage(self.settings, self.MANAGER.filegen, wx.TreeItemIcon_Expanded)
            self.scripts = self.MANAGER.AppendItem(self.MANAGER.root, 'Scripts', 2)
            self.MANAGER.SetItemImage(self.scripts, self.MANAGER.fldridx, wx.TreeItemIcon_Normal)
            self.MANAGER.SetItemImage(self.scripts, self.MANAGER.fldropenidx, wx.TreeItemIcon_Expanded)
            self.extras = self.MANAGER.AppendItem(self.MANAGER.root, 'Extra', 6)
            self.MANAGER.SetItemImage(self.extras, self.MANAGER.fldridx, wx.TreeItemIcon_Normal)
            self.MANAGER.SetItemImage(self.extras, self.MANAGER.fldropenidx, wx.TreeItemIcon_Expanded)
            
        if name == 'scripts':
            self.section_type = 'scripts'
##            self.scripts = self.MANAGER.AppendItem(self.MANAGER.root, 'Scripts', 2)
##            self.MANAGER.SetItemImage(self.scripts, self.MANAGER.fldridx, wx.TreeItemIcon_Normal)
##            self.MANAGER.SetItemImage(self.scripts, self.MANAGER.fldropenidx, wx.TreeItemIcon_Expanded)
        elif name == 'settings':
            self.section_type = 'settings'
##            self.settings = self.MANAGER.AppendItem(self.MANAGER.root, 'Settings', 1)
##            self.MANAGER.SetItemImage(self.settings, self.MANAGER.filegen, wx.TreeItemIcon_Normal)
##            self.MANAGER.SetItemImage(self.settings, self.MANAGER.filegen, wx.TreeItemIcon_Expanded)
        elif name == 'templates':
            self.section_type = 'templates'
##            self.templates = self.MANAGER.AppendItem(self.MANAGER.root, 'Templates', 3)
##            self.MANAGER.SetItemImage(self.templates, self.MANAGER.fldridx, wx.TreeItemIcon_Normal)
##            self.MANAGER.SetItemImage(self.templates, self.MANAGER.fldropenidx, wx.TreeItemIcon_Expanded)
        elif name == 'areas':
            self.section_type = 'areas'
##            self.areas = self.MANAGER.AppendItem(self.MANAGER.root, 'Areas', 4)
##            self.MANAGER.SetItemImage(self.areas, self.MANAGER.fldridx, wx.TreeItemIcon_Normal)
##            self.MANAGER.SetItemImage(self.areas, self.MANAGER.fldropenidx, wx.TreeItemIcon_Expanded)
        elif name == 'worlds':
            self.section_type = 'worlds'
##            self.worlds = self.MANAGER.AppendItem(self.MANAGER.root, 'Worlds', 4)
##            self.MANAGER.SetItemImage(self.worlds, self.MANAGER.fldridx, wx.TreeItemIcon_Normal)
##            self.MANAGER.SetItemImage(self.worlds, self.MANAGER.fldropenidx, wx.TreeItemIcon_Expanded)
        elif name == 'extras':
            self.section_type = 'extras'
##            self.extras = self.MANAGER.AppendItem(self.MANAGER.root, 'Extra', 6)
##            self.MANAGER.SetItemImage(self.extras, self.MANAGER.fldridx, wx.TreeItemIcon_Normal)
##            self.MANAGER.SetItemImage(self.extras, self.MANAGER.fldropenidx, wx.TreeItemIcon_Expanded)

        if self.section_type == 'settings' and name == 'general':
            self.setting_type = 'general'
        elif self.section_type == 'settings' and name == 'module':
            self.setting_type = 'module'
        elif self.section_type == 'settings' and name == 'shaders':
            self.setting_type = 'shaders'

        if name == 'script' and self.section_type == 'scripts':
            self.element_type = 'script'
        elif name == 'template' and self.section_type == 'templates':
            self.element_type = 'template'
        elif name == 'area' and self.section_type == 'areas':
            self.element_type = 'area'
        elif name == 'world' and self.section_type == 'worlds':
            self.element_type = 'world'
        elif name == 'extra' and self.section_type == 'extras':
            self.element_type = 'extra'

##        print '==================================='
##        print 'Start Element:'
##        print 'Name: ' + name
##        print 'Section Name: ' + self.section_type
##        print 'Setting Name: ' + self.setting_type
##        print 'Element Name: ' + self.element_type
##        print 'I: ' + str(self.i)
##        print ' '
        self.i +=1

    def endElement(self, name):
        if self.section_type == name:
            self.section_type = ''
            self.buffer = ''
        if name == 'name' and self.setting_type == 'general':
            if self.buffer:
                self.MANAGER.project_name = self.buffer
            else:
                self.MANAGER.project_name = 'Unknown Project'
        if name == 'dir' and self.setting_type == 'general':
            if self.buffer:
                if not (self.buffer.startswith("#")):
                    self.MANAGER.base_location = self.buffer
                else:
                    self.MANAGER.base_location = self.IDE.projectdir + "/" + self.buffer[1:]
            else:
                dlg = wx.DirDialog(self.MANAGER, "Select base location:", style=wx.DD_DEFAULT_STYLE | wx.DD_NEW_DIR_BUTTON)
                if dlg.ShowModal() == wx.ID_OK:
                    self.MANAGER.base_location = dlg.GetPath()
                dlg.Destroy()
        if name == 'data' and self.setting_type == 'general':
            if self.buffer:
                self.MANAGER.data_location = self.buffer
            else:
                dlg = wx.TextEntryDialog(
                    self, 'Enter the data directory\'s location (relative to base directory):',
                    'Missing data directory', '')
                dlg.SetValue(".")
                if dlg.ShowModal() == wx.ID_OK:
                    self.MANAGER.data_location = dlg.GetValue()
                dlg.Destroy()
        if name == 'output' and self.setting_type == 'general':
            if self.buffer:
                self.MANAGER.output_location = self.buffer
            else:
                dlg = wx.TextEntryDialog(
                    self, 'Enter the output directory\'s location (relative to base directory):',
                    'Missing output directory', '')
                dlg.SetValue(".")
                if dlg.ShowModal() == wx.ID_OK:
                    self.MANAGER.output_location = dlg.GetValue()
                dlg.Destroy()
            
        if self.element_type == name:
            self.cur_element = self.buffer
            self.buffer = ''
            if self.element_type == 'script':
                item = self.MANAGER.AppendItem(self.scripts, self.cur_element, 21)
                self.MANAGER.SetItemImage(item, self.MANAGER.filerks, wx.TreeItemIcon_Normal)
                self.MANAGER.SetItemImage(item, self.MANAGER.filerks, wx.TreeItemIcon_Expanded)
                self.scriptlist.append(item)
            elif self.element_type == 'template':
                item = self.MANAGER.AppendItem(self.templates, self.cur_element, 21)
                self.MANAGER.SetItemImage(item, self.MANAGER.filerkt, wx.TreeItemIcon_Normal)
                self.MANAGER.SetItemImage(item, self.MANAGER.filerkt, wx.TreeItemIcon_Expanded)
                self.templatelist.append(item)
            elif self.element_type == 'area':
                item = self.MANAGER.AppendItem(self.areas, self.cur_element, 21)
                self.MANAGER.SetItemImage(item, self.MANAGER.filerka, wx.TreeItemIcon_Normal)
                self.MANAGER.SetItemImage(item, self.MANAGER.filerka, wx.TreeItemIcon_Expanded)
                self.arealist.append(item)
            elif self.element_type == 'world':
                item = self.MANAGER.AppendItem(self.worlds, self.cur_element, 21)
                self.MANAGER.SetItemImage(item, self.MANAGER.filerkw, wx.TreeItemIcon_Normal)
                self.MANAGER.SetItemImage(item, self.MANAGER.filerkw, wx.TreeItemIcon_Expanded)
                self.worldlist.append(item)
            elif self.element_type == 'extra':
                item = self.MANAGER.AppendItem(self.extras, self.cur_element, 21)
                self.MANAGER.SetItemImage(item, self.MANAGER.fileextra, wx.TreeItemIcon_Normal)
                self.MANAGER.SetItemImage(item, self.MANAGER.fileextra, wx.TreeItemIcon_Expanded)
                self.extralist.append(item)

        if name == 'project':
            self.ProjectOKAY = True
        
##        print '==================================='
##        print 'End Element:'
##        print 'Name: ' + name
##        print 'Section Name: ' + self.section_type
##        print 'Setting Name: ' + self.setting_type
##        print 'Element Name: ' + self.element_type
##        print 'Buffer: ' + self.buffer
##        print 'I: ' + str(self.i)
##        print ' '
        
    def characters(self, chars):
        self.buffer += chars
