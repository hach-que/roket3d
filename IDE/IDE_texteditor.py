import wx
import wx.stc as stc
import wx.aui as aui
import keyword
import os

keywordlist = [ "and", "break", "do", "else", "elseif", "end", "for",
    "function", "if", "local", "nil", "not", "or", "repeat", "return", "then",
    "until", "while", "true", "false" ]

class TextEditor(stc.StyledTextCtrl):
    fold_symbols = 2
    
    def __init__(self, parent, id, log, page_no):
        stc.StyledTextCtrl.__init__(self, parent, id, style=wx.SP_NOBORDER)
        self.log = log
        self.modified = False
        self.allow_marker = False
        self.path = ""
        self.active = 0
        self.page_no = page_no
        self.parent = parent

        self.Bind(wx.EVT_WINDOW_DESTROY, self.OnDestroy)
        self.SetupStyling()

    def SetupStyling(self):
        self.SetLexerLanguage("lua")

        self.SetupTextColors()

        self.SetMarginType(0, stc.STC_MARGIN_NUMBER)
        self.SetMarginWidth(0, 30)
        self.StyleSetSpec(stc.STC_STYLE_LINENUMBER, '')
        self.Bind(stc.EVT_STC_MODIFIED, self.OnTextChanged, id=1000)

        self.projectIsLoaded = False

        # STYLING - MARGINS
        self.SetMarginType(1, stc.STC_MARGIN_SYMBOL)
        self.MarkerDefine(0, stc.STC_MARK_CIRCLE, "#D4D0C8", "blue")
        self.MarkerDefine(1, stc.STC_MARK_CIRCLE, "#D4D0C8", "yellow")
        self.MarkerDefine(2, stc.STC_MARK_CIRCLE, "#D4D0C8", "red")
        self.MarkerDefine(3, stc.STC_MARK_ARROW, "#D4D0C8", "red")
        self.MarkerDefine(4, stc.STC_MARK_BACKGROUND, "white", "#FFC1C1")

        # STYLING - UNDERLINES
        self.IndicatorSetStyle(0, stc.STC_INDIC_SQUIGGLE)
        self.IndicatorSetForeground(0, "yellow")
        self.IndicatorSetStyle(1, stc.STC_INDIC_SQUIGGLE)
        self.IndicatorSetForeground(1, "red")
        self.IndicatorSetStyle(2, stc.STC_INDIC_SQUIGGLE)
        self.IndicatorSetForeground(2, "black")

    def ResetStyles(self):
        total = self.GetLineCount()
        i = 0
        while i < total:
            self.MarkerDelete(i,0)
            self.MarkerDelete(i,1)
            self.MarkerDelete(i,2)
            self.MarkerDelete(i,3)
            self.MarkerDelete(i,4)
##            pos = self.PositionFromLine(i)
##            self.StartStyling(pos, stc.STC_INDICS_MASK)
##            self.SetStyling(self.GetLineEndPosition(i)-pos, 0)
            i += 1;

    def StyleLine(self, line, style):
        # delete margins
        self.MarkerDelete(line-1,0)
        self.MarkerDelete(line-1,1)
        self.MarkerDelete(line-1,2)
        self.MarkerDelete(line-1,3)
        self.MarkerDelete(line-1,4)
        # add margin
        if (style == "INFO"):
            self.MarkerAdd(line-1, 0)
        elif (style == "WARN"):
            self.MarkerAdd(line-1, 1)
        elif (style == "ERROR"):
            self.MarkerAdd(line-1, 2)
        elif (style == "FATAL"):
            self.MarkerAdd(line-1, 3)
            self.MarkerAdd(line-1, 4)

        # add style
##        pos = self.PositionFromLine(line-1)+1
##        self.StartStyling(pos, stc.STC_INDICS_MASK)
##        if (style == "WARN"):
##            self.SetStyling(5, stc.STC_INDIC0_MASK) # self.GetLineEndPosition(line-1)-pos
##        elif (style == "ERROR"):
##            self.SetStyling(5, stc.STC_INDIC1_MASK)
##        elif (style == "FATAL"):
##            self.SetStyling(5, stc.STC_INDIC2_MASK)

    def StyleAllLines(self, taskmanager, name):
        total = taskmanager.list.GetItemCount()
        i = 0
        while i < total:
            filename = taskmanager.GetColumnText(i,1)
            c1 = os.path.abspath(os.path.join(self.parent.GetParent().projectman.base_location,self.parent.GetParent().projectman.data_location) + "/" + filename)
            c2 = os.path.abspath(name)
            if (c1 == c2):
                typ = taskmanager.GetColumnText(i,0)
                line = taskmanager.GetColumnText(i,2)
                self.StyleLine(int(line),typ)
            i += 1
        
    
    def OnDestroy(self, event):
        wx.TheClipboard.Flush()
        event.Skip()

    def OnTextChanged(self, event):
        if self.allow_marker == False:
            return 0;
        if self.modified == False:
            if (self.parent.GetPageText(self.parent.GetSelection())[-1:] != "*"):
                self.parent.SetPageText(self.parent.GetSelection(),self.parent.GetPageText(self.parent.GetSelection()) + "*")
        self.modified = True

    def AllowSaveMark(self):
        self.allow_marker = True

    def SetupTextColors(self):
        faces = { 'times': 'Courier New',
              'mono' : 'Courier New',
              'helv' : 'Courier New',
              'other': 'Courier New',
              'size' : 10,
              'size2': 8,
             }
        
        self.CmdKeyAssign(ord('B'), stc.STC_SCMOD_CTRL, stc.STC_CMD_ZOOMIN)
        self.CmdKeyAssign(ord('N'), stc.STC_SCMOD_CTRL, stc.STC_CMD_ZOOMOUT)

        self.SetLexer(stc.STC_LEX_LUA)
        self.SetKeyWords(0, " ".join(keywordlist))

        self.SetProperty("fold", "1")
        self.SetProperty("tab.timmy.whinge.level", "1")
        self.SetMargins(16,0)

        self.SetViewWhiteSpace(False)
        #self.SetBufferedDraw(False)
        #self.SetViewEOL(True)
        #self.SetEOLMode(stc.STC_EOL_CRLF)
        self.SetUseAntiAliasing(True)
        
        #self.SetEdgeMode(stc.STC_EDGE_BACKGROUND)
        #self.SetEdgeColumn(78)

        # Setup a margin to hold fold markers
        #self.SetFoldFlags(16)  ###  WHAT IS THIS VALUE?  WHAT ARE THE OTHER FLAGS?  DOES IT MATTER?
        self.SetMarginType(2, stc.STC_MARGIN_SYMBOL)
        self.SetMarginMask(2, stc.STC_MASK_FOLDERS)
        self.SetMarginSensitive(2, True)
        self.SetMarginWidth(2, 12)

        if self.fold_symbols == 0:
            # Arrow pointing right for contracted folders, arrow pointing down for expanded
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEROPEN,    stc.STC_MARK_ARROWDOWN, "black", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDER,        stc.STC_MARK_ARROW, "black", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERSUB,     stc.STC_MARK_EMPTY, "black", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERTAIL,    stc.STC_MARK_EMPTY, "black", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEREND,     stc.STC_MARK_EMPTY,     "white", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEROPENMID, stc.STC_MARK_EMPTY,     "white", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERMIDTAIL, stc.STC_MARK_EMPTY,     "white", "black")
            
        elif self.fold_symbols == 1:
            # Plus for contracted folders, minus for expanded
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEROPEN,    stc.STC_MARK_MINUS, "white", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDER,        stc.STC_MARK_PLUS,  "white", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERSUB,     stc.STC_MARK_EMPTY, "white", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERTAIL,    stc.STC_MARK_EMPTY, "white", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEREND,     stc.STC_MARK_EMPTY, "white", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEROPENMID, stc.STC_MARK_EMPTY, "white", "black")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERMIDTAIL, stc.STC_MARK_EMPTY, "white", "black")

        elif self.fold_symbols == 2:
            # Like a flattened tree control using circular headers and curved joins
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEROPEN,    stc.STC_MARK_CIRCLEMINUS,          "white", "#404040")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDER,        stc.STC_MARK_CIRCLEPLUS,           "white", "#404040")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERSUB,     stc.STC_MARK_VLINE,                "white", "#404040")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERTAIL,    stc.STC_MARK_LCORNERCURVE,         "white", "#404040")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEREND,     stc.STC_MARK_CIRCLEPLUSCONNECTED,  "white", "#404040")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEROPENMID, stc.STC_MARK_CIRCLEMINUSCONNECTED, "white", "#404040")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERMIDTAIL, stc.STC_MARK_TCORNERCURVE,         "white", "#404040")

        elif self.fold_symbols == 3:
            # Like a flattened tree control using square headers
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEROPEN,    stc.STC_MARK_BOXMINUS,          "white", "#808080")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDER,        stc.STC_MARK_BOXPLUS,           "white", "#808080")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERSUB,     stc.STC_MARK_VLINE,             "white", "#808080")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERTAIL,    stc.STC_MARK_LCORNER,           "white", "#808080")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEREND,     stc.STC_MARK_BOXPLUSCONNECTED,  "white", "#808080")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDEROPENMID, stc.STC_MARK_BOXMINUSCONNECTED, "white", "#808080")
            self.MarkerDefine(stc.STC_MARKNUM_FOLDERMIDTAIL, stc.STC_MARK_TCORNER,           "white", "#808080")


        self.Bind(stc.EVT_STC_UPDATEUI, self.OnUpdateUI)
        self.Bind(stc.EVT_STC_MARGINCLICK, self.OnMarginClick)
        self.Bind(wx.EVT_KEY_DOWN, self.OnKeyPressed)

        # Make some styles,  The lexer defines what each style is used for, we
        # just have to define what each style looks like.  This set is adapted from
        # Scintilla sample property files.

        # Global default styles for all languages
        self.StyleSetSpec(stc.STC_STYLE_DEFAULT,     "face:%(helv)s,size:%(size)d" % faces)
        self.StyleClearAll()  # Reset all to be like the default

        # Global default styles for all languages
        self.StyleSetSpec(stc.STC_STYLE_DEFAULT,     "face:%(helv)s,size:%(size)d" % faces)
        self.StyleSetSpec(stc.STC_STYLE_LINENUMBER,  "back:#C0C0C0,face:%(helv)s,size:%(size2)d" % faces)
        self.StyleSetSpec(stc.STC_STYLE_CONTROLCHAR, "face:%(other)s" % faces)
        self.StyleSetSpec(stc.STC_STYLE_BRACELIGHT,  "fore:#000000,back:#CCCCAA,bold")
        self.StyleSetSpec(stc.STC_STYLE_BRACEBAD,    "fore:#000000,back:#FF0000,bold")

        # Python styles
        # Default 
        self.StyleSetSpec(stc.STC_P_DEFAULT, "fore:#000000,face:%(helv)s,size:%(size)d" % faces)
        # Comments
        self.StyleSetSpec(stc.STC_P_COMMENTLINE, "fore:#007F00,face:%(other)s,size:%(size)d" % faces)
        # Number
        self.StyleSetSpec(stc.STC_P_NUMBER, "fore:#007F7F,size:%(size)d" % faces)
        # String
        self.StyleSetSpec(stc.STC_P_STRING, "fore:#7F007F,face:%(helv)s,size:%(size)d" % faces)
        # Single quoted string
        self.StyleSetSpec(stc.STC_P_CHARACTER, "fore:#7F007F,face:%(helv)s,size:%(size)d" % faces)
        # Keyword
        self.StyleSetSpec(stc.STC_P_WORD, "fore:#00007F,size:%(size)d" % faces) # bold
        # Triple quotes
        self.StyleSetSpec(stc.STC_P_TRIPLE, "fore:#7F0000,size:%(size)d" % faces)
        # Triple double quotes
        self.StyleSetSpec(stc.STC_P_TRIPLEDOUBLE, "fore:#7F0000,size:%(size)d" % faces)
        # Class name definition
        self.StyleSetSpec(stc.STC_P_CLASSNAME, "fore:#0000FF,underline,size:%(size)d" % faces) # bold
        # Function or method name definition
        self.StyleSetSpec(stc.STC_P_DEFNAME, "fore:#007F7F,size:%(size)d" % faces) # bold
        # Operators
        self.StyleSetSpec(stc.STC_P_OPERATOR, "size:%(size)d" % faces) # bold
        # Identifiers
        self.StyleSetSpec(stc.STC_P_IDENTIFIER, "fore:#000000,face:%(helv)s,size:%(size)d" % faces)
        # Comment-blocks
        self.StyleSetSpec(stc.STC_P_COMMENTBLOCK, "fore:#7F7F7F,size:%(size)d" % faces)
        # End of line where string is not closed
        self.StyleSetSpec(stc.STC_P_STRINGEOL, "fore:#000000,face:%(mono)s,back:#E0C0E0,eol,size:%(size)d" % faces)

        self.SetCaretForeground("BLUE")
        self.SetSelBackground(1,"#0A246A")
        self.SetSelForeground(1,"#FFFFFF")

    def OnKeyPressed(self, event):
######################################## AUTOCOMPLETE IS DISABLED  ################################
##        if self.CallTipActive():
##            self.CallTipCancel()
##        key = event.GetKeyCode()
##
##        if key == 32 or key == 12 or key == 15:
##            self.AddText(chr(key))
##            pos = self.GetCurrentPos()
##
##            kw = keyword.kwlist[:]
##            kw.append("Core")
##            kw.append("Scene")
##            kw.append("Video")
##            kw.append("GUI")
##            kw.append("Physics")
##            kw.append("Ext")
##
##            kw.sort()
##            self.AutoCompSetIgnoreCase(False)
##            self.AutoCompShow(0, " ".join(kw))
##        elif key == 46:
##            self.AddText(".")
##            pos = self.GetCurrentPos()
##
##            # Tips
##            if event.ShiftDown():
##                self.CallTipSetBackground("yellow")
##                self.CallTipShow(pos, 'lots of of text: blah, blah, blah\n\n'
##                                 'show some suff, maybe parameters..\n\n'
##                                 'fubar(param1, param2)')
##            # Code completion
##            else:
##                #lst = []
##                #for x in range(50000):
##                #    lst.append('%05d' % x)
##                #st = " ".join(lst)
##                #print len(st)
##                #self.AutoCompShow(0, st)
##
##                kw = keyword.kwlist[:]
##                kw.append("Core")
##                kw.append("Scene")
##                kw.append("Video")
##                kw.append("GUI")
##                kw.append("Physics")
##                kw.append("Ext")
##                #kw.append("this_is_a_much_much_much_much_much_much_much_longer_value")
##
##                kw.sort()  # Python sorts are case sensitive
##                self.AutoCompSetIgnoreCase(False)  # so this needs to match
##
##                # Images are specified with a appended "?type"
##                for i in range(len(kw)):
##                    if kw[i] in keyword.kwlist:
##                        kw[i] = kw[i] + "?1"
##
##                self.AutoCompShow(0, " ".join(kw))
##        else:
            event.Skip()


    def OnUpdateUI(self, evt):
        # check for matching braces
        braceAtCaret = -1
        braceOpposite = -1
        charBefore = None
        caretPos = self.GetCurrentPos()
        end = 'end'

        if caretPos > 0:
            charBefore = self.GetCharAt(caretPos - 1)
            styleBefore = self.GetStyleAt(caretPos - 1)

        # check before
        if charBefore and chr(charBefore) in "[]{}()" and styleBefore == stc.STC_P_OPERATOR and end:
            braceAtCaret = caretPos - 1

        # check after
        if braceAtCaret < 0:
            charAfter = self.GetCharAt(caretPos)
            styleAfter = self.GetStyleAt(caretPos)

            if charAfter and chr(charAfter) in "[]{}()" and styleAfter == stc.STC_P_OPERATOR and end:
                braceAtCaret = caretPos

        if braceAtCaret >= 0:
            braceOpposite = self.BraceMatch(braceAtCaret)

        if braceAtCaret != -1  and braceOpposite == -1:
            self.BraceBadLight(braceAtCaret)
        else:
            self.BraceHighlight(braceAtCaret, braceOpposite)
            #pt = self.PointFromPosition(braceOpposite)
            #self.Refresh(True, wxRect(pt.x, pt.y, 5,5))
            #print pt
            #self.Refresh(False)


    def OnMarginClick(self, evt):
        # fold and unfold as needed
        if evt.GetMargin() == 2:
            if evt.GetShift() and evt.GetControl():
                self.FoldAll()
            else:
                lineClicked = self.LineFromPosition(evt.GetPosition())

                if self.GetFoldLevel(lineClicked) & stc.STC_FOLDLEVELHEADERFLAG:
                    if evt.GetShift():
                        self.SetFoldExpanded(lineClicked, True)
                        self.Expand(lineClicked, True, True, 1)
                    elif evt.GetControl():
                        if self.GetFoldExpanded(lineClicked):
                            self.SetFoldExpanded(lineClicked, False)
                            self.Expand(lineClicked, False, True, 0)
                        else:
                            self.SetFoldExpanded(lineClicked, True)
                            self.Expand(lineClicked, True, True, 100)
                    else:
                        self.ToggleFold(lineClicked)


    def FoldAll(self):
        lineCount = self.GetLineCount()
        expanding = True

        # find out if we are folding or unfolding
        for lineNum in range(lineCount):
            if self.GetFoldLevel(lineNum) & stc.STC_FOLDLEVELHEADERFLAG:
                expanding = not self.GetFoldExpanded(lineNum)
                break

        lineNum = 0

        while lineNum < lineCount:
            level = self.GetFoldLevel(lineNum)
            if level & stc.STC_FOLDLEVELHEADERFLAG and \
               (level & stc.STC_FOLDLEVELNUMBERMASK) == stc.STC_FOLDLEVELBASE:

                if expanding:
                    self.SetFoldExpanded(lineNum, True)
                    lineNum = self.Expand(lineNum, True)
                    lineNum = lineNum - 1
                else:
                    lastChild = self.GetLastChild(lineNum, -1)
                    self.SetFoldExpanded(lineNum, False)

                    if lastChild > lineNum:
                        self.HideLines(lineNum+1, lastChild)

            lineNum = lineNum + 1



    def Expand(self, line, doExpand, force=False, visLevels=0, level=-1):
        lastChild = self.GetLastChild(line, level)
        line = line + 1

        while line <= lastChild:
            if force:
                if visLevels > 0:
                    self.ShowLines(line, line)
                else:
                    self.HideLines(line, line)
            else:
                if doExpand:
                    self.ShowLines(line, line)

            if level == -1:
                level = self.GetFoldLevel(line)

            if level & stc.STC_FOLDLEVELHEADERFLAG:
                if force:
                    if visLevels > 1:
                        self.SetFoldExpanded(line, True)
                    else:
                        self.SetFoldExpanded(line, False)

                    line = self.Expand(line, doExpand, force, visLevels-1)

                else:
                    if doExpand and self.GetFoldExpanded(line):
                        line = self.Expand(line, True, force, visLevels-1)
                    else:
                        line = self.Expand(line, False, force, visLevels-1)
            else:
                line = line + 1

        return line
