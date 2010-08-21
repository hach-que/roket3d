namespace Roket3D
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.StatusStrip = new System.Windows.Forms.StatusStrip();
            this.ToolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.ToolStripContainer = new System.Windows.Forms.ToolStripContainer();
            this.DockWorkspace = new DockPanelSuite.DockPanel();
            this.StatusStrip.SuspendLayout();
            this.ToolStripContainer.ContentPanel.SuspendLayout();
            this.ToolStripContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip
            // 
            this.StatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripStatusLabel});
            this.StatusStrip.Location = new System.Drawing.Point(0, 628);
            this.StatusStrip.Name = "statusStrip";
            this.StatusStrip.Size = new System.Drawing.Size(852, 22);
            this.StatusStrip.TabIndex = 2;
            this.StatusStrip.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.ToolStripStatusLabel.Name = "toolStripStatusLabel1";
            this.ToolStripStatusLabel.Size = new System.Drawing.Size(837, 17);
            this.ToolStripStatusLabel.Spring = true;
            this.ToolStripStatusLabel.Text = "toolStripStatusLabel1";
            this.ToolStripStatusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // toolStripContainer1
            // 
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.ToolStripContainer.ContentPanel.Controls.Add(this.DockWorkspace);
            this.ToolStripContainer.ContentPanel.Size = new System.Drawing.Size(852, 603);
            this.ToolStripContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ToolStripContainer.Location = new System.Drawing.Point(0, 0);
            this.ToolStripContainer.Name = "toolStripContainer1";
            this.ToolStripContainer.Size = new System.Drawing.Size(852, 628);
            this.ToolStripContainer.TabIndex = 8;
            this.ToolStripContainer.Text = "toolStripContainer1";
            // 
            // dockWorkspace
            // 
            this.DockWorkspace.ActiveAutoHideContent = null;
            this.DockWorkspace.ActiveTab = null;
            this.DockWorkspace.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DockWorkspace.DockBottomPortion = 200;
            this.DockWorkspace.DockLeftPortion = 250;
            this.DockWorkspace.DockRightPortion = 200;
            this.DockWorkspace.DockTopPortion = 150;
            this.DockWorkspace.DocumentStyle = DockPanelSuite.DocumentStyle.DockingWindow;
            this.DockWorkspace.Location = new System.Drawing.Point(0, 0);
            this.DockWorkspace.Name = "dockWorkspace";
            this.DockWorkspace.Size = new System.Drawing.Size(852, 603);
            this.DockWorkspace.TabIndex = 0;
            this.DockWorkspace.ActiveTabChanged += new System.EventHandler(this.dockWorkspace_ActiveTabChanged);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(852, 650);
            this.Controls.Add(this.ToolStripContainer);
            this.Controls.Add(this.StatusStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.IsMdiContainer = true;
            this.Name = "MainForm";
            this.Text = "No Solution Loaded - Roket3D";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.StyleChanged += new System.EventHandler(this.MainForm_StyleChanged);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.SizeChanged += new System.EventHandler(this.MainForm_SizeChanged);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.ResizeEnd += new System.EventHandler(this.MainForm_ResizeEnd);
            this.StatusStrip.ResumeLayout(false);
            this.StatusStrip.PerformLayout();
            this.ToolStripContainer.ContentPanel.ResumeLayout(false);
            this.ToolStripContainer.ResumeLayout(false);
            this.ToolStripContainer.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip StatusStrip;
        private System.Windows.Forms.ToolStripStatusLabel ToolStripStatusLabel;
        private System.Windows.Forms.ToolStripContainer ToolStripContainer;
        public DockPanelSuite.DockPanel DockWorkspace;
    }
}

