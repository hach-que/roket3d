using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DockPanelSuite;
using Roket3D.Properties;
using Roket3D.Xml;
using Roket3D.Menus.Definitions.Project;

namespace Roket3D.ToolWindows
{
    public partial class ToolSolutionExplorer : DockContent
    {
        public MainForm MainForm;

        public ToolSolutionExplorer(MainForm myMain)
        {
            InitializeComponent();
            this.MainForm = myMain;

            // Reload the image list (because sometimes the transparency gets
            // mucked up).
            imageList.Images.Clear();
            imageList.Images.Add("Solution", Resources.solution);
            imageList.Images.Add("Project", Resources.project);
            imageList.Images.Add("Model", Resources.model);
            imageList.Images.Add("Folder", Resources.folder);
            imageList.Images.Add("Audio", Resources.audio);
            imageList.Images.Add("Area", Resources.area);
            imageList.Images.Add("Image", Resources.image);
            imageList.Images.Add("ProjectReference", Resources.projectreference);
            imageList.Images.Add("Reference", Resources.reference);
            imageList.Images.Add("Script", Resources.script);
            imageList.Images.Add("CHeader", Resources.cheader);
            imageList.Images.Add("CScript", Resources.cscript);
            imageList.Images.Add("World", Resources.world);
            imageList.Images.Add("NotFound", Resources.not_found);
            imageList.Images.Add("NotFoundFolder", Resources.not_found_folder);

            // Fill the solution explorer with the tree nodes.
            solutionTree.Nodes.Clear();
            solutionTree.Nodes.Add(this.MainForm.CurrentSolution.ToTreeNode());
            solutionTree.ExpandAll();
        }

        public void ReloadTree()
        {
            // Fill the solution explorer with the tree nodes.
            solutionTree.Nodes.Clear();
            solutionTree.Nodes.Add(this.MainForm.CurrentSolution.ToTreeNode());
            solutionTree.ExpandAll();
        }

        private void ToolSolutionExplorer_Load(object sender, EventArgs e)
        {
            // Fully expand the tree.
            solutionTree.ExpandAll();
        }

        private void solutionTree_DoubleClick(object sender, EventArgs e)
        {
            // Transform the selected node into a FileTreeNode if possible.
            List<File> files = new List<File>();
            FileTreeNode pN = null;
            if (solutionTree.SelectedNode is FileTreeNode)
                pN = (FileTreeNode)solutionTree.SelectedNode;
            else
                return;

            // Build a path to the specified file.
            File fN = pN.fileRef;

            while (fN != null)
            {
                files.Add(fN);
                fN = fN.Parent;
            }

            files.Reverse();

            if (files.Count == 0)
            {
                // The user did not select a valid file to open.
                return;
            }

            String path = "";
            foreach (File f in files)
            {
                path += f.Name + "/";
            }
            path = path.Substring(0, path.Length - 1); // strip the last / that was appended.

            MainForm.OpenFile(path, pN.fileRef);
        }

        private void solutionTree_MouseUp(object sender, MouseEventArgs e)
        {
            if(e.Button == MouseButtons.Right)
            {
                // Select the clicked node
                solutionTree.SelectedNode = solutionTree.GetNodeAt(e.X, e.Y);

                if(solutionTree.SelectedNode != null)
                {
                    this.ShowRelevantOptions();
                    contextMenuStrip.Show(solutionTree, e.Location);
                }
            }
        }

        private void ShowRelevantOptions()
        {
            // Set them all to invisible by default.
            addToolStripMenuItem.Visible = false;
            openToolStripMenuItem.Visible = false;
            openWithToolStripMenuItem.Visible = false;
            seperator1ToolStripMenuItem.Visible = false;
            viewCodeToolStripMenuItem.Visible = false;
            viewDesignerToolStripMenuItem.Visible = false;
            seperator2ToolStripMenuItem.Visible = false;
            excludeFromProjectToolStripMenuItem.Visible = false;
            seperator3ToolStripMenuItem.Visible = false;
            cutToolStripMenuItem.Visible = false;
            copyToolStripMenuItem.Visible = false;
            pasteToolStripMenuItem.Visible = false;
            deleteToolStripMenuItem.Visible = false;
            seperator4ToolStripMenuItem.Visible = false;
            propertiesToolStripMenuItem.Visible = false;

            // Determine the file type.
            if (!(solutionTree.SelectedNode is FileTreeNode))
                return;
            FileTreeNode treeNode = (FileTreeNode)solutionTree.SelectedNode;
            
            // First check for project / solution node.
            if (treeNode.projectRef != null)
            {
                addToolStripMenuItem.Visible = true;
                return;
            }
            else if (treeNode.solutionRef != null)
            {
                propertiesToolStripMenuItem.Visible = true;
                return;
            }

            File f = treeNode.fileRef;
            if (f == null)
                return;

            switch (f.Type)
            {
                case FileType.Model:
                case FileType.Image:
                case FileType.Audio:
                    openToolStripMenuItem.Visible = true;
                    openWithToolStripMenuItem.Visible = true;
                    seperator1ToolStripMenuItem.Visible = true;
                    viewDesignerToolStripMenuItem.Visible = true;
                    seperator2ToolStripMenuItem.Visible = true;
                    excludeFromProjectToolStripMenuItem.Visible = true;
                    break;
                case FileType.Template:
                case FileType.Area:
                case FileType.World:
                    openToolStripMenuItem.Visible = true;
                    openWithToolStripMenuItem.Visible = true;
                    seperator1ToolStripMenuItem.Visible = true;
                    viewCodeToolStripMenuItem.Visible = true;
                    viewDesignerToolStripMenuItem.Visible = true;
                    seperator2ToolStripMenuItem.Visible = true;
                    excludeFromProjectToolStripMenuItem.Visible = true;
                    break;
                case FileType.Script:
                case FileType.CHeader:
                case FileType.CScript:
                    openToolStripMenuItem.Visible = true;
                    openWithToolStripMenuItem.Visible = true;
                    seperator1ToolStripMenuItem.Visible = true;
                    viewCodeToolStripMenuItem.Visible = true;
                    seperator2ToolStripMenuItem.Visible = true;
                    excludeFromProjectToolStripMenuItem.Visible = true;
                    break;
                case FileType.Folder:
                    addToolStripMenuItem.Visible = true;
                    seperator1ToolStripMenuItem.Visible = true;
                    excludeFromProjectToolStripMenuItem.Visible = true;
                    break;
                case FileType.Generic:
                case FileType.Unknown:
                    excludeFromProjectToolStripMenuItem.Visible = true;
                    break;
                case FileType.Reference:
                case FileType.ProjectReference:
                    excludeFromProjectToolStripMenuItem.Visible = true;
                    break;
            }
        }

        private void addModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddModel act = new AddModel();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddImage act = new AddImage();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addAudioToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddAudio act = new AddAudio();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addTemplateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddTemplate act = new AddTemplate();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addAreaToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddArea act = new AddArea();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addWorldToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddWorld act = new AddWorld();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addClassToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddClass act = new AddClass();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addScriptToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddScript act = new AddScript();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addNewItemToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddNewItem act = new AddNewItem();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addExistingItemToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddExistingItem act = new AddExistingItem();
            act.TargetDirectory = GetFileFromTreeNode(solutionTree.SelectedNode);
            act.OnActivate();
        }

        private void addReferenceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddReference act = new AddReference();
            act.OnActivate();
        }

        static private File GetFileFromTreeNode(TreeNode node)
        {
            if (!(node is FileTreeNode))
                return null;
            FileTreeNode treeNode = (FileTreeNode)node;
            File f = treeNode.fileRef;
            if (f == null)
                return null;
            return f;
        }
    }
}
