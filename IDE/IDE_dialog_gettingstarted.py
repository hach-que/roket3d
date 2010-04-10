import wx
import os

class GettingStartingDialog(wx.Dialog):
    def __init__(self, ide):
        # begin wxGlade: GettingStartingDialog.__init__
        wx.Dialog.__init__(self, ide, style=wx.DEFAULT_DIALOG_STYLE)
        self.bitmap_button_2 = wx.BitmapButton(self, -1, wx.Bitmap(ide.appdir+"/Data/images/backg_gettingstarted.png", wx.BITMAP_TYPE_ANY), style=wx.NO_BORDER)
        self.btnOkay = wx.Button(self, -1, "OK")

        self.__set_properties()
        self.__do_layout()

        self.btnOkay.SetDefault()
        self.btnOkay.SetFocus()
        self.CentreOnScreen()

        wx.EVT_BUTTON(self, self.btnOkay.GetId(), self.closeDialog)
        # end wxGlade

    def __set_properties(self):
        # begin wxGlade: GettingStartingDialog.__set_properties
        self.SetTitle("Getting Started")
        self.SetSize((617, 470))
        self.bitmap_button_2.SetSize(self.bitmap_button_2.GetBestSize())
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: GettingStartingDialog.__do_layout
        sizer_4 = wx.BoxSizer(wx.VERTICAL)
        sizer_4.Add(self.bitmap_button_2, 0, wx.ALL, 5)
        sizer_4.Add(self.btnOkay, 0, wx.ALL|wx.ALIGN_RIGHT, 5)
        self.SetSizer(sizer_4)
        self.Layout()
        # end wxGlade

    def closeDialog(self, event): # wxGlade: GettingStartingDialog.<event_handler>
        self.EndModal(0)
