import wx

class IDEStatusBar(wx.StatusBar):
    def __init__(self, parent):
        wx.StatusBar.__init__(self, parent, -1)

        # This status bar has three fields
        self.SetFieldsCount(4)
        # Sets the three fields to be relative widths to each other.
        self.SetStatusWidths([-2, -2, 40, 100])
        
        self.sizeChanged = False
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_IDLE, self.OnIdle)

        # Setup fields
        # Field 0 is just status text
        self.SetStatusText("", 0)

        # Field 1 is also just status text
        self.SetStatusText("", 1)

        # Field 2 is also just status text (line numbers)
        self.SetStatusText("", 2)

        # Field 3 is a gauge (for autoupdates)
        self.gauge = wx.Gauge(self, -1 ,style=wx.GA_SMOOTH)
        self.gauge.SetValue(0)

        # set the initial position of the checkbox
        self.Reposition()

    def OnSize(self, evt):
        self.Reposition()  # for normal size events

        # Set a flag so the idle time handler will also do the repositioning.
        # It is done this way to get around a buglet where GetFieldRect is not
        # accurate during the EVT_SIZE resulting from a frame maximize.
        self.sizeChanged = True

    def OnIdle(self, evt):
        if self.sizeChanged:
            self.Reposition()

    # reposition the checkbox
    def Reposition(self):
        rect = self.GetFieldRect(3)
        self.gauge.SetPosition((rect.x+2, rect.y+2))
        self.gauge.SetSize((rect.width+12, rect.height-4))
        self.sizeChanged = False

    def GetGauge(self):
        return self.gauge
