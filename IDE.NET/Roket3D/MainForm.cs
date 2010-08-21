using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DockPanelSuite;
using Roket3D.ToolWindows;
using Roket3D.ContentEditors;
using Roket3D.Xml;
using LuaInterface;
using System.IO;
using Roket3D.Menus;
using Roket3D.ContentEditors;

namespace Roket3D
{
    public partial class MainForm : Form
    {
        public Solution CurrentSolution = new Solution();
        public ToolSolutionExplorer SolutionExplorer;
        public ToolConsole LuaConsole = new ToolConsole();
        public ToolErrorList ErrorList = new ToolErrorList();
        public ToolBuildOutput BuildOutput = new ToolBuildOutput();
        public Stack<ScintillaNet.Scintilla> ScintillaAvailable = new Stack<ScintillaNet.Scintilla>();
        private FormWindowState m_OldWindowState = FormWindowState.Normal;
        private SplashScreen SplashScreen = null;
        public List<Roket3D.Menus.Action> MenuActions = new List<Roket3D.Menus.Action>();

        // TODO: Use an ActionManager and Action class to build menus and toolbars
        //       dynamically so that we don't have to duplicate code unnessicarily.
        public MainForm(SplashScreen splash)
        {
            InitializeComponent();
            this.SplashScreen = splash;

            // Preload a certain number of scintilla editors.
            Int32 PRELOAD_NUMBER = 5;

            SplashScreen.SetProgress(0, "Loading Scintilla...");
            for (Int32 i = 0; i < PRELOAD_NUMBER; i += 1)
            {
                if (i == 0)
                    SplashScreen.SetProgress(0, "Loading Scintilla...");
                else
                    SplashScreen.SetProgress(100 / PRELOAD_NUMBER * i, "Preloading editors ( " + i.ToString() + " / " + PRELOAD_NUMBER.ToString() + " )...");
                ScintillaAvailable.Push(new ScintillaNet.Scintilla());
            }
            SplashScreen.SetProgress(100, "Starting IDE...");

            // Load the menus and toolbars.
            MenuLoader Loader = new MenuLoader(this);
            this.Controls.Add(Loader.MainMenu);
            this.MainMenuStrip = Loader.MainMenu;
            //this.toolStripContainer1.TopToolStripPanel.Renderer = new ToolBarRenderer();
            this.ToolStripContainer.TopToolStripPanel.Controls.Add(Loader.ToolBar);

            // Load a solution.
            //CurrentSolution.Load("X:\\IDE.NET\\TestSolution\\MyGame.rsln");
            //CurrentSolution.Load("C:\\Server Storage\\Roket3D\\IDE.NET\\TestSolution\\MyGame.rsln");

            // Set up the workspace.
            this.SolutionExplorer = new ToolSolutionExplorer(this);
            this.SolutionExplorer.Text = "Solution Explorer";
            this.SolutionExplorer.Show(DockWorkspace, DockState.DockLeft);
            this.LuaConsole.Text = "Lua Console";
            this.LuaConsole.Show(DockWorkspace, DockState.DockBottom);
            this.ErrorList.Text = "Error List";
            this.ErrorList.Show(DockWorkspace, DockState.DockBottom);
            this.BuildOutput.Text = "Build Output";
            this.BuildOutput.Show(DockWorkspace, DockState.DockBottom);

            SpecialStartForm startPage = new SpecialStartForm();
            startPage.Text = "Start Page";
            startPage.TabText = "Start Page";
            startPage.Show(DockWorkspace);

            /*String syntaxTest = @"-- Syntax Check

" + "var = \"123abc\";" + @"
" + "1var = \"this is definitely invalid!\";" + @"
" + @"abc123def = 5;
notmalformed = 5.0;
malformed1 = 5.0.;
malformed2 = 5.0.1;
unfinishedstring = " + "\"" + @"it really is!
invalidproperty1 = abc.2;
invalidproperty2 = abc1.2;
invalidproperty3 = abc1.;
invalidproperty4 = a1.2;
invalidproperty5 = a.2;

45another invalid 45identifier";

            CodeScriptForm sCoder = new CodeScriptForm(this, this.RetrieveAvailableEditor());
            sCoder.Text = "syntax check";
            sCoder.TabText = "syntax check";
            sCoder.Show(dockWorkspace);*/
            //sCoder.scintillaEditor.AppendText(syntaxTest);

            // Create some documents.
            /*foreach (Project i in CurrentSolution.Projects)
            {
                CodeScriptForm coder = new CodeScriptForm(this);
                coder.Text = i.Filename + " [project]";
                coder.TabText = i.Filename + " [project]";
                coder.scintillaEditor.Text = "Found: " + (!i.NotFound).ToString() + @"
Project Filename: " + i.Filename + @"
Project Path: " + i.FullPath + @"

";
                foreach (String s in i.GetConfigurationList())
                {
                    Configuration c = i.GetConfiguration(s);
                    coder.scintillaEditor.Text += "---- Configuration: " + c.Name + " ----\r\n";
                    foreach (KeyValuePair<String,String> kv in c.Settings)
                    {
                        coder.scintillaEditor.Text += kv.Key + " = " + kv.Value + "\r\n";
                    }
                    coder.scintillaEditor.Text += "\r\n";
                }

                coder.scintillaEditor.Text += "---- References: ----\r\n";
                foreach (Reference r in i.References)
                {
                    coder.scintillaEditor.Text += r.IncludeString + "\r\n";
                }

                coder.scintillaEditor.Text += "\r\n---- Files: ----\r\n";
                foreach (File f in i.Files)
                {
                    coder.scintillaEditor.Text += f.Name + "\r\n";
                }
                coder.Show(dockWorkspace);
            }*/

            /*String syntaxTest = ""; @"-- Syntax Check

" + "var = \"123abc\";" + @"
" + "1var = \"this is definitely invalid!\";" + @"
" + @"abc123def = 5;
notmalformed = 5.0;
malformed1 = 5.0.;
malformed2 = 5.0.1;
unfinishedstring = " + "\"" + @"it really is!
invalidproperty1 = abc.2;
invalidproperty2 = abc1.2;
invalidproperty3 = abc1.;
invalidproperty4 = a1.2;
invalidproperty5 = a.2;

45another invalid 45identifier";

            CodeScriptForm sCoder = new CodeScriptForm(this);
            sCoder.Text = "syntax check";
            sCoder.TabText = "syntax check";
            sCoder.Show(dockWorkspace);
            sCoder.scintillaEditor.AppendText(syntaxTest);*/
        }

        public ScintillaNet.Scintilla RetrieveAvailableEditor()
        {
            if (this.ScintillaAvailable.Count == 0)
            {
                // The user has used up all the preloaded Scintilla editors.
                return new ScintillaNet.Scintilla();
            }
            else
            {
                return this.ScintillaAvailable.Pop();
            }
        }

        public void RetireInuseEditor(ScintillaNet.Scintilla editor)
        {
            this.ScintillaAvailable.Push(editor);
        }

        public void OpenFile(String path, Roket3D.Xml.File file)
        {
            // TODO: Check to see whether the file actually exists here.
            //       If it doesn't, show a message and then change the icon
            //       in the solution explorer.
            //
            //       Possibly a new boolean member Exists for the File class?
            if (!System.IO.File.Exists(path))
            {

            }

            // Search through all the tabs that are currently
            // open and make sure we're not reloading the file.
            foreach (IDockContent f in DockWorkspace.Documents)
            {
                if (f.DockHandler.Form is EditorBase)
                {
                    EditorBase eB = (EditorBase)f.DockHandler.Form;

                    // Check the path
                    if (eB.Path == path)
                    {
                        // Switch to this tab.
                        f.DockHandler.Activate();
                        return;
                    }
                }
            }

            // If we reach here, the file is not open.
            this.CreateEditor(new FileInfo(path), path, file);
        }

        public void CreateEditor(FileInfo path, String relativePath, Roket3D.Xml.File file)
        {
            switch (Roket3D.Xml.File.DetectTypeFromExtension(path.Extension))
            {
                case FileType.Script:
                case FileType.CHeader:
                case FileType.CScript:
                    // Create a code editor.
                    CodeScriptForm codeTab = new CodeScriptForm(this, this.RetrieveAvailableEditor());
                    codeTab.Path = relativePath;
                    codeTab.File = file;
                    codeTab.Text = path.Name;
                    codeTab.TabText = path.Name;

                    try
                    {
                        StreamReader f = new StreamReader(codeTab.Path);
                        codeTab.ScintillaEditor.Text = f.ReadToEnd();
                        f.Close();
                    }
                    catch (FileNotFoundException)
                    {
                        codeTab.ScintillaEditor.Text = "";
                    }

                    codeTab.Show(DockWorkspace);
                    DockWorkspace.ActiveTab = codeTab;
                    OnActiveTabChanged();
                    break;
                case FileType.Model:
                    // Create a model viewer.
                    DesignerModelForm modelTab = new DesignerModelForm(this);
                    modelTab.Path = relativePath;
                    modelTab.File = file;
                    modelTab.Text = path.Name;
                    modelTab.TabText = path.Name;
                    modelTab.Show(DockWorkspace);
                    DockWorkspace.ActiveTab = modelTab;
                    OnActiveTabChanged();
                    break;
                case FileType.Image:
                    // Create a model viewer.
                    DesignerImageForm imageTab = new DesignerImageForm(new Bitmap(path.FullName));
                    imageTab.Path = relativePath;
                    imageTab.File = file;
                    imageTab.Text = path.Name;
                    imageTab.TabText = path.Name;
                    imageTab.Show(DockWorkspace);
                    DockWorkspace.ActiveTab = imageTab;
                    OnActiveTabChanged();
                    break;
                case FileType.Folder:
                    // Nothing to do here.
                    break;
                default:
                    // Unimplemented.
                    MessageBox.Show("Roket3D does not have support for opening the requested filetype.");
                    break;
            }
        }

        public List<Roket3D.Xml.File> GetAllFilesOfType(FileType fileType)
        {
            List<Roket3D.Xml.File> files = new List<Roket3D.Xml.File>();

            foreach (Project p in this.CurrentSolution.Projects)
            {
                foreach (Roket3D.Xml.File f in p.Files)
                {
                    if (f.Type == fileType)
                        files.Add(f);

                    files.AddRange(f.GetAllFilesOfType(fileType));
                }
            }

            return files;
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            /*if (e.CloseReason == CloseReason.UserClosing)
            {
                Application.Exit();
            }
            else
            {
                /*IEnumerable<IDockContent> copy = dockWorkspace.Documents;
                foreach (IDockContent f in dockWorkspace.Documents)
                {
                    if (f.DockHandler.Form is EditorBase)
                    {
                        EditorBase eB = (EditorBase)f.DockHandler.Form;
                        eB.TerminateThread();
                    }
                }*
            }*/
        }

        private void MainForm_ResizeEnd(object sender, EventArgs e)
        {
            IEnumerable<IDockContent> copy = DockWorkspace.Documents;
            foreach (IDockContent f in DockWorkspace.Documents)
            {
                if (f.DockHandler.Form is EditorBase)
                {
                    EditorBase eB = (EditorBase)f.DockHandler.Form;
                    eB.OnResizeEnd();
                }
            }
        }

        private void MainForm_StyleChanged(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized || 
                this.WindowState == FormWindowState.Normal)
            {
                this.MainForm_ResizeEnd(sender, e);
            }
        }

        private void MainForm_SizeChanged(object sender, EventArgs e)
        {
            if ((this.WindowState == FormWindowState.Normal &&
                 this.m_OldWindowState == FormWindowState.Maximized) ||
                (this.WindowState == FormWindowState.Maximized &&
                 this.m_OldWindowState == FormWindowState.Normal))
            {
                this.MainForm_ResizeEnd(sender, e);
                this.m_OldWindowState = this.WindowState;
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            SplashScreen.Hide();
        }

        private void dockWorkspace_ActiveTabChanged(object sender, EventArgs e)
        {
            OnActiveTabChanged();
        }

        private void OnActiveTabChanged()
        {
            foreach (DockContent f in DockWorkspace.Documents)
            {
                if (f.DockHandler.Form is EditorBase)
                {
                    EditorBase eB = (EditorBase)f.DockHandler.Form;
                    this.SendMenuEvent(MenuEvent.TAB_CHANGED, eB);
                    eB.OnActiveTabChanged();
                }
            }
        }

        public void SendMenuEvent(MenuEvent Event)
        {
            this.SendMenuEvent(Event, null);
        }

        public void SendMenuEvent(MenuEvent Event, Object Data)
        {
            foreach (Roket3D.Menus.Action a in MenuActions)
            {
                switch (Event)
                {
                    case MenuEvent.SOLUTION_OPEN:
                        a.OnSolutionOpen();
                        break;
                    case MenuEvent.SOLUTION_CLOSE:
                        a.OnSolutionClose();
                        break;
                    case MenuEvent.TAB_CHANGED:
                        if (Data is EditorBase)
                            a.OnTabChanged((EditorBase)Data);
                        else
                            a.OnTabChanged(null);
                        break;
                }
            }

            // Check the events to see if there are other things to do...
            switch (Event)
            {
                case MenuEvent.SOLUTION_OPEN:
                    this.Text = this.CurrentSolution.SolutionName + " - Roket3D";
                    break;
                case MenuEvent.SOLUTION_CLOSE:
                    this.Text = "No Solution Loaded - Roket3D";
                    break;
            }
        }
    }

    public enum MenuEvent
    {
        UNKNOWN,
        SOLUTION_OPEN,
        SOLUTION_CLOSE,
        TAB_CHANGED
    }
}
