using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DockPanelSuite;
using Roket3D.LuaDebugging;
using System.Threading;

namespace Roket3D.ContentEditors
{
    enum LexerStyleConstants
    {
        SCE_LUA_DEFAULT = 0,
        SCE_LUA_COMMENT = 1,
        SCE_LUA_COMMENTLINE = 2,
        SCE_LUA_COMMENTDOC = 3,
        SCE_LUA_NUMBER = 4,
        SCE_LUA_WORD_KEYWORDS = 5,
        SCE_LUA_STRING = 6,
        SCE_LUA_CHARACTER = 7,
        SCE_LUA_LITERALSTRING = 8,
        SCE_LUA_PREPROCESSOR = 9,
        SCE_LUA_OPERATOR = 10,
        SCE_LUA_IDENTIFIER = 11,
        SCE_LUA_STRINGEOL = 12,
        SCE_LUA_WORD_BASIC_FUNCTIONS = 13,
        SCE_LUA_WORD_STRING_TABLE_MATH = 14,
        SCE_LUA_WORD_COROUTINE_IO_SYSTEM = 15,
        SCE_LUA_WORD_ROKET3D_CLASSES = 16,
        SCE_LUA_WORD_CLASS_KEYWORDS = 17,
        SCE_LUA_WORD7 = 18,
        SCE_LUA_WORD8 = 19
    }

    enum LexerKeywordGroupConstants
    {
        LUA_KEYWORDS = 0,
        LUA_BASIC_FUNCTIONS = 1,
        LUA_STRING_TABLE_MATH = 2,
        LUA_COROUTINE_IO_SYSTEM = 3,
        LUA_ROKET3D_CLASSES = 4,
        LUA_CLASS_KEYWORDS = 5,
        LUA_USER3 = 6,
        LUA_USER4 = 7
    }

    public partial class CodeScriptForm : EditorBase
    {
        public static Font fontSetting = new Font("Courier New", 9.0f);
        public Int32 styleCount = 0;
        public MainForm MainForm;
        public bool canSyntaxCheck = true;
        public List<LuaError> errors = new List<LuaError>();
        private ToolTip toolTip = new ToolTip();

        public CodeScriptForm(MainForm myMain, ScintillaNet.Scintilla editor)
        {
            InitializeComponent();
            this.MainForm = myMain;
            this.scintillaEditor = editor;

            InitalizeScintilla();
        }

        private void InitalizeScintilla()
        {
            #region Windows Forms Designer Generated Code
            ((System.ComponentModel.ISupportInitialize)(this.scintillaEditor)).BeginInit();
            // 
            // scintillaEditor
            // 
            this.scintillaEditor.CallTip.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scintillaEditor.LineWrap.Mode = ScintillaNet.WrapMode.Word;
            this.scintillaEditor.Location = new System.Drawing.Point(0, 0);
            this.scintillaEditor.LongLines.EdgeColumn = 160;
            this.scintillaEditor.LongLines.EdgeMode = ScintillaNet.EdgeMode.Line;
            this.scintillaEditor.Margins.Margin2.Width = 16;
            this.scintillaEditor.Name = "scintillaEditor";
            this.scintillaEditor.Size = new System.Drawing.Size(553, 534);
            this.scintillaEditor.Styles.BraceBad.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Styles.BraceBad.FontName = "Courier New";
            this.scintillaEditor.Styles.BraceBad.Size = 8.25F;
            this.scintillaEditor.Styles.BraceLight.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Styles.BraceLight.FontName = "Courier New";
            this.scintillaEditor.Styles.BraceLight.Size = 8.25F;
            this.scintillaEditor.Styles.CallTip.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Styles.CallTip.FontName = "Courier New";
            this.scintillaEditor.Styles.ControlChar.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Styles.ControlChar.FontName = "Courier New";
            this.scintillaEditor.Styles.ControlChar.Size = 8.25F;
            this.scintillaEditor.Styles.Default.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Styles.Default.CharacterSet = ScintillaNet.CharacterSet.Ansi;
            this.scintillaEditor.Styles.Default.FontName = "Courier New";
            this.scintillaEditor.Styles.Default.Size = 8.25F;
            this.scintillaEditor.Styles.IndentGuide.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Styles.IndentGuide.FontName = "Courier New";
            this.scintillaEditor.Styles.IndentGuide.Size = 8.25F;
            this.scintillaEditor.Styles.LastPredefined.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Styles.LastPredefined.FontName = "Courier New";
            this.scintillaEditor.Styles.LastPredefined.Size = 8.25F;
            this.scintillaEditor.Styles.LineNumber.FontName = "Courier New";
            this.scintillaEditor.Styles.LineNumber.Size = 8.25F;
            this.scintillaEditor.Styles.Max.BackColor = System.Drawing.SystemColors.Window;
            this.scintillaEditor.Styles.Max.FontName = "Courier New";
            this.scintillaEditor.Styles.Max.Size = 8.25F;
            this.scintillaEditor.TabIndex = 0;
            this.scintillaEditor.DwellStart += new System.EventHandler<ScintillaNet.ScintillaMouseEventArgs>(this.scintillaEditor_DwellStart);
            this.scintillaEditor.KeyUp += new System.Windows.Forms.KeyEventHandler(this.scintillaEditor_KeyUp);
            this.scintillaEditor.DwellEnd += new System.EventHandler<ScintillaNet.ScintillaMouseEventArgs>(this.scintillaEditor_DwellEnd);

            this.Refresh();
            this.initalizationLabel.Dispose();

            this.toolStripContainer1.ContentPanel.Controls.Add(this.scintillaEditor);

            ((System.ComponentModel.ISupportInitialize)(this.scintillaEditor)).EndInit();
            #endregion

            #region Manual Setup
            // Set up the font settings.
            this.scintillaEditor.Font = CodeScriptForm.fontSetting;
            this.scintillaEditor.Styles.Default.Font = CodeScriptForm.fontSetting;
            this.scintillaEditor.UseFont = true;

            // Set up the margins.
            this.scintillaEditor.Margins.Left = 5;
            this.scintillaEditor.Margins[0].Width = 45;
            this.scintillaEditor.Margins[2].Width = 16;

            // Set the dwell time.
            this.scintillaEditor.NativeInterface.SetMouseDwellTime(200);

            // Set up the lexer.
            this.scintillaEditor.ConfigurationManager.Language = "lua";
            this.scintillaEditor.Styles.LineNumber.BackColor = Color.White;
            this.scintillaEditor.NativeInterface.SetFoldMarginColour(true, 0xE6F0FA); // Color.FromArgb(250, 240, 230)
            this.scintillaEditor.NativeInterface.SetFoldMarginHiColour(true, 0xF5F5F5); // Color.FromArgb(245, 245, 245)

            // Configure keywords.
            this.SetScintillaKeywords(LexerKeywordGroupConstants.LUA_KEYWORDS,
                "and break do else elseif " +
                "end false for function if " +
                "in local nil not or " +
                "repeat return then true until while");
            this.SetScintillaKeywords(LexerKeywordGroupConstants.LUA_BASIC_FUNCTIONS,
                "_G _VERSION assert collectgarbage " +
                "dofile error getfenv getmetatable " +
                "ipairs load loadfile loadstring " +
                "module next pairs pcall print " +
                "rawequal rawget rawset require " +
                "select setfenv setmetatable tonumber " +
                "tostring type unpack xpcall");
            this.SetScintillaKeywords(LexerKeywordGroupConstants.LUA_STRING_TABLE_MATH,
                // Math
                "math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos " +
                "math.cosh math.deg math.exp math.floor math.fmod math.frexp math.huge " +
                "math.ldexp math.log math.log10 math.max math.min math.modf math.pi " +
                "math.pow math.rad math.random math.randomseed math.sin math.sinh " +
                "math.sqrt math.tan math.tanh " +
                // String
                "string.byte string.char string.dump string.find string.format " +
                "string.gmatch string.gsub string.len string.lower string.match " +
                "string.rep string.reverse string.sub string.upper " +
                // Table
                "table.concat table.insert table.maxn table.remove table.sort");
            this.SetScintillaKeywords(LexerKeywordGroupConstants.LUA_COROUTINE_IO_SYSTEM,
                // Coroutine
                "coroutine.create coroutine.resume coroutine.running coroutine.status " +
                "coroutine.wrap coroutine.yield " +
                // Debug
                "debug.debug debug.getfenv debug.gethook debug.getinfo debug.getlocal " +
                "debug.getmetatable debug.getregistry debug.getupvalue debug.setfenv " +
                "debug.sethook debug.setlocal debug.setmetatable debug.setupvalue " +
                "debug.traceback " +
                // IO
                "io.close io.flush io.input io.lines io.open io.output io.popen " +
                "io.read io.stderr io.stdin io.stdout io.tmpfile io.type io.write " +
                // OS
                "os.clock os.date os.difftime os.execute os.exit os.getenv os.remove " +
                "os.rename os.setlocale os.time os.tmpname " +
                // Package
                "package.cpath package.loaded package.loaders package.loadlib " +
                "package.path package.preload package.seeall");
            this.SetScintillaKeywords(LexerKeywordGroupConstants.LUA_ROKET3D_CLASSES,
                // Core
                "Core.Area2D Core.Area3D Core.Color Core.Matrix Core.Plane " +
                "Core.Quaternion Core.RCursorControl Core.RDevice Core.Ray3D " +
                "Core.RoketEventReciever Core.Triangle3D Core.Vector2D " +
                "Core.Vector3D " +
                // Audio
                "Audio.RAudioController Audio.RAudioSound " +
                // GUI
                "GUI.RGUIButton GUI.RGUICheckbox GUI.RGUICombobox GUI.RGUIEditBox " +
                "GUI.RGUIElement GUI.RGUIEnvironment GUI.RGUIFileOpenDialog " +
                "GUI.RGUIImage GUI.RGUIListbox GUI.RGUIStaticText " +
                // Net
                "Net.RNetSocket " +
                // Physics
                "Physics.RPhysicsJoint Physics.RPhysicsManager " +
                // Scene
                "Scene.RMesh Scene.RMeshSceneNode Scene.RSceneCamera " +
                "Scene.RSceneCollisionManager Scene.RSceneManager Scene.RSceneNode " +
                "Scene.RTerrainSceneNode " +
                // Video
                "Video.RTexture Video.RVideoDriver Video.RVideoMaterial");
            this.SetScintillaKeywords(LexerKeywordGroupConstants.LUA_CLASS_KEYWORDS,
                // Class keywords
                "name inherits description lastmodified author");

            // Configure colours.
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_DEFAULT, Color.FromArgb(0, 0, 0));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_COMMENT, Color.FromArgb(0, 128, 0));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_COMMENTLINE, Color.FromArgb(0, 128, 0));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_COMMENTDOC, Color.FromArgb(0, 128, 0));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_NUMBER, Color.FromArgb(0, 0, 153));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_STRING, Color.FromArgb(163, 21, 21));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_CHARACTER, Color.FromArgb(163, 21, 21));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_LITERALSTRING, Color.FromArgb(163, 21, 21));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_OPERATOR, Color.FromArgb(0, 0, 0));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_IDENTIFIER, Color.FromArgb(0, 0, 0));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_WORD_KEYWORDS, Color.FromArgb(21, 24, 255));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_WORD_BASIC_FUNCTIONS, Color.FromArgb(21, 24, 255));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_WORD_STRING_TABLE_MATH, Color.FromArgb(21, 24, 255));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_WORD_COROUTINE_IO_SYSTEM, Color.FromArgb(21, 24, 255));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_WORD_ROKET3D_CLASSES, Color.FromArgb(0, 128, 128));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_WORD_CLASS_KEYWORDS, Color.FromArgb(160, 0, 200));

            // Not yet configured.
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_PREPROCESSOR, Color.FromArgb(127, 127, 0));
            this.SetScintillaStyle(LexerStyleConstants.SCE_LUA_STRINGEOL, Color.FromArgb(163, 21, 21));

            // Configure indicators.
            this.scintillaEditor.Indicators[0].Style = ScintillaNet.IndicatorStyle.Squiggle;
            this.scintillaEditor.Indicators[0].Color = Color.Red;

            // Start the timer so that we get proper syntax checking after the lexer
            // has finished lexing the text (unfortunatly there is no event for when
            // the lexer has finished parsing the text).
            this.syntaxTimerLimit.Start();
            #endregion
        }

        public void SyntaxCheck()
        {
            // Prevent flicker.
            if (this.canSyntaxCheck)
            {
                this.MainForm.ErrorList.Refresh();
            }

            // Clear all the indicators.
            this.scintillaEditor.GetRange(0, this.scintillaEditor.Text.Length).ClearIndicator(0);

            // Get errors.
            errors = SyntaxChecker.Check(this.scintillaEditor.Text, this.scintillaEditor);

            // Apply highlighting to the errors.
            // TODO: Use a class variable to store the current filename.
            if (this.canSyntaxCheck)
            {
                this.MainForm.ErrorList.ClearErrorsForFile(this.TabText, "");
            }
            foreach (LuaError err in errors)
            {
                // Indicators have an offset by one.
                this.scintillaEditor.GetRange(err.IndicatorIndex, err.Index + err.Length).SetIndicator(0);

                // Add the error to the error list.
                if (this.canSyntaxCheck)
                {
                    this.MainForm.ErrorList.AddError(err.ErrorMsg, this.TabText, err.Line, err.Column, "");
                }
            }

            // Prevent flicker.
            if (this.canSyntaxCheck)
            {
                this.MainForm.ErrorList.Refresh();
            }
        }

        private void SetScintillaStyle(LexerStyleConstants style, Color forecolor)
        {
            this.scintillaEditor.Styles[(Int32)style].ForeColor = forecolor;
            this.scintillaEditor.Styles[(Int32)style].BackColor = Color.Transparent;
        }

        private void SetScintillaKeywords(LexerKeywordGroupConstants group, String keywordslist)
        {
            this.scintillaEditor.Lexing.SetKeywords((Int32)group, keywordslist);
        }

        private void scintillaEditor_DwellStart(object sender, ScintillaNet.ScintillaMouseEventArgs e)
        {
            // The user may want to find out what the error is.
            foreach (LuaError error in this.errors)
            {
                if (e.Position >= error.IndicatorIndex && e.Position <= error.Index + error.Length)
                {
                    toolTip.UseFading = false;
                    toolTip.UseAnimation = false;
                    toolTip.Show(error.ErrorMsg, this.scintillaEditor, e.X+20, e.Y+10);
                }
            }
        }

        private void scintillaEditor_KeyUp(object sender, KeyEventArgs e)
        {
            this.canSyntaxCheck = false;
            SyntaxCheck();
            syntaxTimerLimit.Stop();
            syntaxTimerLimit.Start();
        }

        private void syntaxTimerLimit_Tick(object sender, EventArgs e)
        {
            this.canSyntaxCheck = true;
            SyntaxCheck();
            syntaxTimerLimit.Stop();
        }

        private void scintillaEditor_DwellEnd(object sender, ScintillaNet.ScintillaMouseEventArgs e)
        {
            toolTip.Hide(this.scintillaEditor);
        }

        #region Unused Callbacks

        public override void OnResizeEnd()
        {
        }

        #endregion

        public override void OnActiveTabChanged()
        {
            if (this.MainForm.dockWorkspace.ActiveTab == this)
                this.scintillaEditor.Focus();
        }

        public override void TerminateThread()
        {
            this.toolStripContainer1.ContentPanel.Controls.Remove(this.scintillaEditor);
            this.MainForm.RetireInuseEditor(this.scintillaEditor);
        }

        private void CodeScriptForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.toolStripContainer1.ContentPanel.Controls.Remove(this.scintillaEditor);
            this.MainForm.RetireInuseEditor(this.scintillaEditor);
        }
    }
}
