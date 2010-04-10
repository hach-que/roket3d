import os
import  wx
import  wx.lib.mixins.listctrl  as  listmix
import sys
import IDE_texteditor
import  wx.lib.dialogs

class TaskManagerListCtrl(wx.ListCtrl, listmix.ListCtrlAutoWidthMixin):
    def __init__(self, parent, ID, pos=wx.DefaultPosition,
                 size=wx.DefaultSize, style=0):
        wx.ListCtrl.__init__(self, parent, ID, pos, size, style)
        listmix.ListCtrlAutoWidthMixin.__init__(self)

class TaskManager(wx.Panel, listmix.ColumnSorterMixin):
    def __init__(self, parent, id, ide):
        wx.Panel.__init__(self, parent, -1)

        self.IDE = ide
        self.game_full_output = "You have not run the game yet."
        self.SetBackgroundColour(wx.BLACK)

        tID = wx.NewId()
        self.list = TaskManagerListCtrl(self, tID,
                                 style=wx.LC_REPORT 
                                 #| wx.BORDER_SUNKEN
                                 #| wx.BORDER_NONE
                                 #| wx.LC_EDIT_LABELS
                                 | wx.LC_SORT_ASCENDING
                                 | wx.EXPAND
                                 #| wx.LC_NO_HEADER
                                 | wx.LC_VRULES
                                 | wx.LC_HRULES
                                 | wx.LC_SINGLE_SEL
                                #, size = parent.GetSize()
                                 )

        self.sizer = wx.BoxSizer(wx.HORIZONTAL)
        self.sizer.Add(self.list, 1, wx.EXPAND, 0)
        self.SetSizer(self.sizer);

        self.list.SetImageList(self.IDE.projectman.il, wx.IMAGE_LIST_SMALL)

        info = wx.ListItem()
        info.m_mask = wx.LIST_MASK_TEXT | wx.LIST_MASK_FORMAT # | wx.LIST_MASK_IMAGE
        #info.m_image = -1
        info.m_format = wx.LIST_FORMAT_LEFT
        info.m_text = "Error"
        self.list.InsertColumnInfo(0, info)

        info.m_format = wx.LIST_FORMAT_LEFT
        info.SetAlign(wx.LC_ALIGN_LEFT)
        info.SetWidth(90)
        info.m_text = "File"
        self.list.InsertColumnInfo(1, info)

        info.m_format = wx.LIST_FORMAT_LEFT
        info.SetAlign(wx.LC_ALIGN_LEFT)
        info.SetWidth(100)
        info.m_text = "Line"
        self.list.InsertColumnInfo(2, info)

        info.m_format = wx.LIST_FORMAT_LEFT
        info.SetAlign(wx.LC_ALIGN_LEFT)
        info.m_text = "Description"
        self.list.InsertColumnInfo(3, info)

        self.list.SetColumnWidth(0, 80)
        self.list.SetColumnWidth(1, 90)
        self.list.SetColumnWidth(2, 36)
        self.list.SetColumnWidth(3, wx.LIST_AUTOSIZE_USEHEADER)

        self.Bind(wx.EVT_LIST_ITEM_ACTIVATED, self.ItemActivate, self.list)
        self.Bind(wx.EVT_LIST_INSERT_ITEM, self.ItemInsert, self.list)
        self.Bind(wx.EVT_LIST_ITEM_RIGHT_CLICK, self.ShowContextMenu, self.list)

    def GetColumnText(self, index, col):
        item = self.list.GetItem(index, col)
        return item.GetText()

    def ItemActivate(self, event):
        self.currentItem = event.m_itemIndex
##        list_item = wx.ListItem
##        list_item.SetId(self.currentItem)
##        list_item.SetColumn(1)
##        list_item.SetMask(wxLIST_MASK_TEXT) 
##        self.list.GetItem(list_item)
        a = self.GetColumnText(self.currentItem, 1)
        line = self.GetColumnText(self.currentItem, 2)
        if (a != ""):
            path = self.IDE.projectman.base_location + "/" + self.IDE.projectman.data_location + "/" + a
            if (self.IDE.projectman.CheckIsFileOpen(path) != -1):
                c = self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection())
                c.GotoLine(min(int(line)+5,c.GetLineCount()))
                c.GotoLine(int(line)-1)
                return;
        
            try:
                file = open(path, 'r')
                text = file.read()
                file.close()
                # Get the wxWindow for the current tab, get the first child (textarea)
                self.IDE.notebook.AddPage(IDE_texteditor.TextEditor(self.IDE.notebook, 1000, '', 1), os.path.basename(path), select = 1)
                c = self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection())
                c.ClearAll()
                c.AddText(text)

                c.StyleAllLines(self, path)
                
                c.GotoLine(min(int(line)+5,c.GetLineCount()))
                c.GotoLine(int(line)-1)
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

    def ItemInsert(self, event):
        self.currentItem = self.list.GetItemCount() - 1
        if self.currentItem >= 0:
            if (self.GetColumnText(self.currentItem, 0) == "FATAL"):
                a = self.GetColumnText(self.currentItem, 1)
                line = self.GetColumnText(self.currentItem, 2)
                if (a != ""):
                    path = self.IDE.projectman.base_location + "/" + self.IDE.projectman.data_location + "/" + a
                    if (self.IDE.projectman.CheckIsFileOpen(path) != -1):
                        return;
                
                    try:
                        file = open(path, 'r')
                        text = file.read()
                        file.close()
                        # Get the wxWindow for the current tab, get the first child (textarea)
                        self.IDE.notebook.AddPage(IDE_texteditor.TextEditor(self.IDE.notebook, 1000, '', 1), os.path.basename(path), select = 1)
                        c = self.IDE.notebook.GetPage(self.IDE.notebook.GetSelection())
                        c.ClearAll()
                        c.AddText(text)

                        c.StyleAllLines(self, a)
                        
                        c.GotoLine(min(int(line)+5,c.GetLineCount()))
                        c.GotoLine(int(line)-1)
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
                        
    def ShowContextMenu(self,event):
        if not hasattr(self, "popupID1"):
            self.popupSHOWALL = wx.NewId()
            self.Bind(wx.EVT_MENU, self.ShowAllOutput, id=self.popupSHOWALL)

        # Show delete menu
        menu = wx.Menu()
        menu.Append(self.popupSHOWALL, "Show All Output")
        self.PopupMenu(menu)
        menu.Destroy()
        
        return 0

    def ShowAllOutput(self, event):
        gout = ""
        for i in self.game_full_output:
            gout += i
        dlg = wx.lib.dialogs.ScrolledMessageDialog(self, gout, "Full Game Output")
        dlg.ShowModal()

##        self.InsertColumn(0, 'Task')
##        self.InsertColumn(1, 'Due Date')
##        self.InsertColumn(2, 'Completed')
##
##        self.SetColumnWidth(0, 220)
##        self.SetColumnWidth(1, 70)
##        self.SetColumnWidth(2, 100)
##
##        self.InsertStringItem(0, "Test")
        

#---------------------------------------------------------------------------

##class CustomDataTable(gridlib.PyGridTableBase):
##    def __init__(self):
##        gridlib.PyGridTableBase.__init__(self)
##
##        self.colLabels = ['Type', '', 'Description', 'File']
##
##        self.dataTypes = [gridlib.GRID_VALUE_STRING,
##                          gridlib.GRID_VALUE_NUMBER,
##                          gridlib.GRID_VALUE_STRING,
##                          gridlib.GRID_VALUE_STRING
##                       .list   ]
##
##        self.data = [
##            ["FATAL", 15, "Attempt to index nil", "main.rks"],
##            ["WARN", 27, "This object cannot have physics applied to it.", "phys.rks"],
##            ["", 36, "This object cannot have physics applied to it.", "phys.rks"]
##            ]
##
##
##    #--------------------------------------------------
##    # required methods for the wxPyGridTableBase interface
##
##    def GetNumberRows(self):
##        return len(self.data) + 1
##
##    def GetNumberCols(self):
##        return len(self.data[0])
##
##    def IsEmptyCell(self, row, col):
##        try:
##            return not self.data[row][col]
##        except IndexError:
##            return True
##
##    # Get/Set values in.list the table.  The Python version of these
##    # methods can handle any data-type, (as long as the Editor and
##    # Renderer understands the type too,) not just strings as in the
##    # C++ version.
##    def GetValue(self, row, col):
##        try:
##            return self.data[row][col]
##        except IndexError:
##            return ''
##
##    def SetValue(self, row, col, value):
##        def innerSetValue(row, col, value):
##            try:
##                self.data[row][col] = value
##            except IndexError:
##                # add a new row
##                self.data.append([''] * self.GetNumberCols())
##                innerSetValue(row, col, value)
##
##                # tell the grid we've added a row
##                msg = gridlib.GridTableMessage(self,            # The table
##                        gridlib.GRIDTABLE_NOTIFY_ROWS_APPENDED, # what we did to it
##                        1                                       # how many
##                        )
##
##                self.GetView().ProcessTableMessage(msg)
##        innerSetValue(row, col, value) 
##
##    #--------------------------------------------------
##    # Some optional methods
##
##    # Called when the grid needs to display labels
##    def GetColLabelValue(self, col):
##        return self.colLabels[col]
##
##    # Called to determine the kind of editor/renderer to use by
##    # default, doesn't necessarily have to be the same type used
##    # natively by the editor/renderer if they know how to convert.
##    def GetTypeName(self, row, col):
##        return self.dataTypes[col]
##
##    # Called to determine how the data can be fetched and stored by the
##    # editor and renderer.  This allows you to enforce some type-safety
##    # in the grid.
##    def CanGetValueAs(self, row, col, typeName):
##        colType = self.dataTypes[col].split(':')[0]
##        if typeName == colType:
##            return True
##        else:
##            return False
##
##    def CanSetValueAs(self, row, col, typeName):
##        return self.CanGetValueAs(row, col, typeName)
##
##
##
##
##
###---------------------------------------------------------------------------
##
##
##
##class TaskManager(gridlib.Grid):
##    def __init__(self, parent, test):
##        gridlib.Grid.__init__(self, parent, -1)
##
##        table = CustomDataTable()
##
##        # The second parameter means that the grid is to take ownership of the
##        # table and will destroy it when done.  Otherwise you would need to keep
##        # a reference to it and call it's Destroy method later.
##        self.SetTable(table, True)
##
##        self.SetRowLabelSize(0)
##        self.SetMargins(0,0)
##        self.AutoSizeColumns(True)
##
##        gridlib.EVT_GRID_CELL_LEFT_DCLICK(self, self.OnLeftDClick)
##
##
##    # I do this because I don't like the default behaviour of not starting the
##    # cell editor on double clicks, but only a second click.
##    def OnLeftDClick(self, evt):
##        if self.CanEnableCellControl():
##            self.EnableCellEditControl()

##
###---------------------------------------------------------------------------
##
##class TestFrame(wx.Frame):
##    def __init__(self, parent, log):
##
##        wx.Frame.__init__(
##            self, parent, -1, "Custom Table, data driven Grid  Demo", size=(640,480)
##            )
##
##        p = wx.Panel(self, -1, style=0)
##        grid = CustTableGrid(p, log)
##        b = wx.Button(p, -1, "Another Control...")
##        b.SetDefault()
##        self.Bind(wx.EVT_BUTTON, self.OnButton, b)
##        b.Bind(wx.EVT_SET_FOCUS, self.OnButtonFocus)
##        bs = wx.BoxSizer(wx.VERTICAL)
##        bs.Add(grid, 1, wx.GROW|wx.ALL, 5)
##        bs.Add(b)
##        p.SetSizer(bs)
##
##    def OnButton(self, evt):
##        print "button selected"
##
##    def OnButtonFocus(self, evt):
##        print "button focus"
##
##
###---------------------------------------------------------------------------
##
##if __name__ == '__main__':
##    import sys
##    app = wx.PySimpleApp()
##    frame = TestFrame(None, sys.stdout)
##    frame.Show(True)
##    app.MainLoop()
##
##
###---------------------------------------------------------------------------
