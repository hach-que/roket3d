namespace Roket3D.ContentEditors
{
    partial class DesignerModelForm
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
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.toolStrip2 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.textureToolStripComboBox = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.toolStrip2.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripContainer1
            // 
            this.toolStripContainer1.BottomToolStripPanelVisible = false;
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(460, 1);
            this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Top;
            this.toolStripContainer1.LeftToolStripPanelVisible = false;
            this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer1.Name = "toolStripContainer1";
            this.toolStripContainer1.RightToolStripPanelVisible = false;
            this.toolStripContainer1.Size = new System.Drawing.Size(460, 26);
            this.toolStripContainer1.TabIndex = 1;
            this.toolStripContainer1.Text = "toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.toolStrip2);
            // 
            // toolStrip2
            // 
            this.toolStrip2.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip2.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.textureToolStripComboBox});
            this.toolStrip2.Location = new System.Drawing.Point(3, 0);
            this.toolStrip2.Name = "toolStrip2";
            this.toolStrip2.Size = new System.Drawing.Size(386, 25);
            this.toolStrip2.TabIndex = 0;
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(81, 22);
            this.toolStripLabel1.Text = "Select Texture:";
            // 
            // textureToolStripComboBox
            // 
            this.textureToolStripComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.textureToolStripComboBox.DropDownWidth = 300;
            this.textureToolStripComboBox.Name = "textureToolStripComboBox";
            this.textureToolStripComboBox.Size = new System.Drawing.Size(300, 25);
            this.textureToolStripComboBox.SelectedIndexChanged += new System.EventHandler(this.textureToolStripComboBox_SelectedIndexChanged);
            // 
            // DesignerModelForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlDark;
            this.ClientSize = new System.Drawing.Size(460, 351);
            this.Controls.Add(this.toolStripContainer1);
            this.DockAreas = DockPanelSuite.DockAreas.Document;
            this.DoubleBuffered = true;
            this.Name = "DesignerModelForm";
            this.TabText = "Model Designer";
            this.Text = "Model Designer";
            this.Load += new System.EventHandler(this.DesignerModelForm_Load);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.DesignerModelForm_MouseUp);
            this.Shown += new System.EventHandler(this.DesignerModelForm_Shown);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.DesignerModelForm_MouseDown);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DesignerModelForm_FormClosing);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.DesignerModelForm_MouseMove);
            this.ResizeEnd += new System.EventHandler(this.DesignerModelForm_ResizeEnd);
            this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.PerformLayout();
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.toolStrip2.ResumeLayout(false);
            this.toolStrip2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
        private System.Windows.Forms.ToolStrip toolStrip2;
        private System.Windows.Forms.ToolStripComboBox textureToolStripComboBox;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;

    }
}