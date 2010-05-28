namespace Roket3D.ToolWindows
{
    partial class ToolSolutionExplorer
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToolSolutionExplorer));
            this.solutionTree = new System.Windows.Forms.TreeView();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openWithToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.seperator1ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.viewCodeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewDesignerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.seperator2ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.excludeFromProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.seperator3ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.cutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pasteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.seperator4ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.propertiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addModelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addAudioToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addTemplateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addAreaToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addWorldToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripSeparator();
            this.addClassToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addScriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem6 = new System.Windows.Forms.ToolStripSeparator();
            this.addNewItemToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addExistingItemToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem7 = new System.Windows.Forms.ToolStripSeparator();
            this.addReferenceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // solutionTree
            // 
            this.solutionTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.solutionTree.ImageIndex = 0;
            this.solutionTree.ImageList = this.imageList;
            this.solutionTree.Location = new System.Drawing.Point(0, 0);
            this.solutionTree.Name = "solutionTree";
            this.solutionTree.SelectedImageIndex = 0;
            this.solutionTree.Size = new System.Drawing.Size(246, 475);
            this.solutionTree.TabIndex = 0;
            this.solutionTree.DoubleClick += new System.EventHandler(this.solutionTree_DoubleClick);
            this.solutionTree.MouseUp += new System.Windows.Forms.MouseEventHandler(this.solutionTree_MouseUp);
            // 
            // imageList
            // 
            this.imageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
            this.imageList.ImageSize = new System.Drawing.Size(16, 16);
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // contextMenuStrip
            // 
            this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.openToolStripMenuItem,
            this.openWithToolStripMenuItem,
            this.seperator1ToolStripMenuItem,
            this.viewCodeToolStripMenuItem,
            this.viewDesignerToolStripMenuItem,
            this.seperator2ToolStripMenuItem,
            this.excludeFromProjectToolStripMenuItem,
            this.seperator3ToolStripMenuItem,
            this.cutToolStripMenuItem,
            this.copyToolStripMenuItem,
            this.pasteToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.seperator4ToolStripMenuItem,
            this.propertiesToolStripMenuItem});
            this.contextMenuStrip.Name = "contextMenuStrip";
            this.contextMenuStrip.Size = new System.Drawing.Size(187, 292);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Image = global::Roket3D.Properties.Resources.actions_open;
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.openToolStripMenuItem.Text = "Open";
            // 
            // openWithToolStripMenuItem
            // 
            this.openWithToolStripMenuItem.Name = "openWithToolStripMenuItem";
            this.openWithToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.openWithToolStripMenuItem.Text = "Open With...";
            // 
            // seperator1ToolStripMenuItem
            // 
            this.seperator1ToolStripMenuItem.Name = "seperator1ToolStripMenuItem";
            this.seperator1ToolStripMenuItem.Size = new System.Drawing.Size(183, 6);
            // 
            // viewCodeToolStripMenuItem
            // 
            this.viewCodeToolStripMenuItem.Image = global::Roket3D.Properties.Resources.view_code;
            this.viewCodeToolStripMenuItem.Name = "viewCodeToolStripMenuItem";
            this.viewCodeToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.viewCodeToolStripMenuItem.Text = "View Code";
            // 
            // viewDesignerToolStripMenuItem
            // 
            this.viewDesignerToolStripMenuItem.Image = global::Roket3D.Properties.Resources.view_designer;
            this.viewDesignerToolStripMenuItem.Name = "viewDesignerToolStripMenuItem";
            this.viewDesignerToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.viewDesignerToolStripMenuItem.Text = "View Designer";
            // 
            // seperator2ToolStripMenuItem
            // 
            this.seperator2ToolStripMenuItem.Name = "seperator2ToolStripMenuItem";
            this.seperator2ToolStripMenuItem.Size = new System.Drawing.Size(183, 6);
            // 
            // excludeFromProjectToolStripMenuItem
            // 
            this.excludeFromProjectToolStripMenuItem.Name = "excludeFromProjectToolStripMenuItem";
            this.excludeFromProjectToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.excludeFromProjectToolStripMenuItem.Text = "Exclude From Project";
            // 
            // seperator3ToolStripMenuItem
            // 
            this.seperator3ToolStripMenuItem.Name = "seperator3ToolStripMenuItem";
            this.seperator3ToolStripMenuItem.Size = new System.Drawing.Size(183, 6);
            // 
            // cutToolStripMenuItem
            // 
            this.cutToolStripMenuItem.Image = global::Roket3D.Properties.Resources.actions_cut;
            this.cutToolStripMenuItem.Name = "cutToolStripMenuItem";
            this.cutToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.cutToolStripMenuItem.Text = "Cut";
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Image = global::Roket3D.Properties.Resources.actions_copy;
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.copyToolStripMenuItem.Text = "Copy";
            // 
            // pasteToolStripMenuItem
            // 
            this.pasteToolStripMenuItem.Image = global::Roket3D.Properties.Resources.actions_paste;
            this.pasteToolStripMenuItem.Name = "pasteToolStripMenuItem";
            this.pasteToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.pasteToolStripMenuItem.Text = "Paste";
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            // 
            // seperator4ToolStripMenuItem
            // 
            this.seperator4ToolStripMenuItem.Name = "seperator4ToolStripMenuItem";
            this.seperator4ToolStripMenuItem.Size = new System.Drawing.Size(183, 6);
            // 
            // propertiesToolStripMenuItem
            // 
            this.propertiesToolStripMenuItem.Image = global::Roket3D.Properties.Resources.tools_properties_window;
            this.propertiesToolStripMenuItem.Name = "propertiesToolStripMenuItem";
            this.propertiesToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.propertiesToolStripMenuItem.Text = "Properties";
            // 
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addModelToolStripMenuItem,
            this.addImageToolStripMenuItem,
            this.addAudioToolStripMenuItem,
            this.addTemplateToolStripMenuItem,
            this.addAreaToolStripMenuItem,
            this.addWorldToolStripMenuItem,
            this.toolStripMenuItem5,
            this.addClassToolStripMenuItem,
            this.addScriptToolStripMenuItem,
            this.toolStripMenuItem6,
            this.addNewItemToolStripMenuItem,
            this.addExistingItemToolStripMenuItem,
            this.toolStripMenuItem7,
            this.addReferenceToolStripMenuItem});
            this.addToolStripMenuItem.Image = global::Roket3D.Properties.Resources.add;
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.Size = new System.Drawing.Size(186, 22);
            this.addToolStripMenuItem.Text = "Add";
            // 
            // addModelToolStripMenuItem
            // 
            this.addModelToolStripMenuItem.Image = global::Roket3D.Properties.Resources.model_add;
            this.addModelToolStripMenuItem.Name = "addModelToolStripMenuItem";
            this.addModelToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addModelToolStripMenuItem.Text = "Add Model...";
            this.addModelToolStripMenuItem.Click += new System.EventHandler(this.addModelToolStripMenuItem_Click);
            // 
            // addImageToolStripMenuItem
            // 
            this.addImageToolStripMenuItem.Image = global::Roket3D.Properties.Resources.image_add;
            this.addImageToolStripMenuItem.Name = "addImageToolStripMenuItem";
            this.addImageToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addImageToolStripMenuItem.Text = "Add Image...";
            this.addImageToolStripMenuItem.Click += new System.EventHandler(this.addImageToolStripMenuItem_Click);
            // 
            // addAudioToolStripMenuItem
            // 
            this.addAudioToolStripMenuItem.Image = global::Roket3D.Properties.Resources.audio_add;
            this.addAudioToolStripMenuItem.Name = "addAudioToolStripMenuItem";
            this.addAudioToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addAudioToolStripMenuItem.Text = "Add Audio...";
            this.addAudioToolStripMenuItem.Click += new System.EventHandler(this.addAudioToolStripMenuItem_Click);
            // 
            // addTemplateToolStripMenuItem
            // 
            this.addTemplateToolStripMenuItem.Name = "addTemplateToolStripMenuItem";
            this.addTemplateToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addTemplateToolStripMenuItem.Text = "Add Template...";
            this.addTemplateToolStripMenuItem.Click += new System.EventHandler(this.addTemplateToolStripMenuItem_Click);
            // 
            // addAreaToolStripMenuItem
            // 
            this.addAreaToolStripMenuItem.Image = global::Roket3D.Properties.Resources.area_add;
            this.addAreaToolStripMenuItem.Name = "addAreaToolStripMenuItem";
            this.addAreaToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addAreaToolStripMenuItem.Text = "Add Area...";
            this.addAreaToolStripMenuItem.Click += new System.EventHandler(this.addAreaToolStripMenuItem_Click);
            // 
            // addWorldToolStripMenuItem
            // 
            this.addWorldToolStripMenuItem.Image = global::Roket3D.Properties.Resources.world_add;
            this.addWorldToolStripMenuItem.Name = "addWorldToolStripMenuItem";
            this.addWorldToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addWorldToolStripMenuItem.Text = "Add World...";
            this.addWorldToolStripMenuItem.Click += new System.EventHandler(this.addWorldToolStripMenuItem_Click);
            // 
            // toolStripMenuItem5
            // 
            this.toolStripMenuItem5.Name = "toolStripMenuItem5";
            this.toolStripMenuItem5.Size = new System.Drawing.Size(166, 6);
            // 
            // addClassToolStripMenuItem
            // 
            this.addClassToolStripMenuItem.Image = global::Roket3D.Properties.Resources.class_add;
            this.addClassToolStripMenuItem.Name = "addClassToolStripMenuItem";
            this.addClassToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addClassToolStripMenuItem.Text = "Add Class...";
            this.addClassToolStripMenuItem.Click += new System.EventHandler(this.addClassToolStripMenuItem_Click);
            // 
            // addScriptToolStripMenuItem
            // 
            this.addScriptToolStripMenuItem.Image = global::Roket3D.Properties.Resources.script_add;
            this.addScriptToolStripMenuItem.Name = "addScriptToolStripMenuItem";
            this.addScriptToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addScriptToolStripMenuItem.Text = "Add Script...";
            this.addScriptToolStripMenuItem.Click += new System.EventHandler(this.addScriptToolStripMenuItem_Click);
            // 
            // toolStripMenuItem6
            // 
            this.toolStripMenuItem6.Name = "toolStripMenuItem6";
            this.toolStripMenuItem6.Size = new System.Drawing.Size(166, 6);
            // 
            // addNewItemToolStripMenuItem
            // 
            this.addNewItemToolStripMenuItem.Name = "addNewItemToolStripMenuItem";
            this.addNewItemToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addNewItemToolStripMenuItem.Text = "Add New Item";
            this.addNewItemToolStripMenuItem.Click += new System.EventHandler(this.addNewItemToolStripMenuItem_Click);
            // 
            // addExistingItemToolStripMenuItem
            // 
            this.addExistingItemToolStripMenuItem.Name = "addExistingItemToolStripMenuItem";
            this.addExistingItemToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addExistingItemToolStripMenuItem.Text = "Add Existing Item";
            this.addExistingItemToolStripMenuItem.Click += new System.EventHandler(this.addExistingItemToolStripMenuItem_Click);
            // 
            // toolStripMenuItem7
            // 
            this.toolStripMenuItem7.Name = "toolStripMenuItem7";
            this.toolStripMenuItem7.Size = new System.Drawing.Size(166, 6);
            // 
            // addReferenceToolStripMenuItem
            // 
            this.addReferenceToolStripMenuItem.Image = global::Roket3D.Properties.Resources.reference;
            this.addReferenceToolStripMenuItem.Name = "addReferenceToolStripMenuItem";
            this.addReferenceToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.addReferenceToolStripMenuItem.Text = "Add Reference";
            this.addReferenceToolStripMenuItem.Click += new System.EventHandler(this.addReferenceToolStripMenuItem_Click);
            // 
            // ToolSolutionExplorer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(246, 475);
            this.Controls.Add(this.solutionTree);
            this.DockAreas = ((DockPanelSuite.DockAreas)(((((DockPanelSuite.DockAreas.Float | DockPanelSuite.DockAreas.DockLeft)
                        | DockPanelSuite.DockAreas.DockRight)
                        | DockPanelSuite.DockAreas.DockTop)
                        | DockPanelSuite.DockAreas.DockBottom)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ToolSolutionExplorer";
            this.TabText = "Solution Explorer";
            this.Text = "Solution Explorer";
            this.Load += new System.EventHandler(this.ToolSolutionExplorer_Load);
            this.contextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView solutionTree;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addModelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addAudioToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addTemplateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addAreaToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addWorldToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem5;
        private System.Windows.Forms.ToolStripMenuItem addClassToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openWithToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator seperator1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewCodeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewDesignerToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator seperator2ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem excludeFromProjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator seperator3ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pasteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator seperator4ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem propertiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addScriptToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem6;
        private System.Windows.Forms.ToolStripMenuItem addNewItemToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addExistingItemToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem7;
        private System.Windows.Forms.ToolStripMenuItem addReferenceToolStripMenuItem;
    }
}