namespace Roket3D.ToolWindows
{
    partial class ToolBuildOutput
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToolBuildOutput));
            this.buildOutputTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // buildOutputTextBox
            // 
            this.buildOutputTextBox.BackColor = System.Drawing.Color.White;
            this.buildOutputTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buildOutputTextBox.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buildOutputTextBox.Location = new System.Drawing.Point(0, 0);
            this.buildOutputTextBox.Multiline = true;
            this.buildOutputTextBox.Name = "buildOutputTextBox";
            this.buildOutputTextBox.ReadOnly = true;
            this.buildOutputTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.buildOutputTextBox.Size = new System.Drawing.Size(802, 210);
            this.buildOutputTextBox.TabIndex = 0;
            // 
            // ToolBuildOutput
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(802, 210);
            this.Controls.Add(this.buildOutputTextBox);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ToolBuildOutput";
            this.Text = "Build Output";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox buildOutputTextBox;
    }
}