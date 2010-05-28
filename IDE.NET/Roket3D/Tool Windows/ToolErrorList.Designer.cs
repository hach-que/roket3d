namespace Roket3D.ToolWindows
{
    partial class ToolErrorList
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToolErrorList));
            this.errorList = new System.Windows.Forms.ListView();
            this.columnIcon = new System.Windows.Forms.ColumnHeader();
            this.columnLine = new System.Windows.Forms.ColumnHeader();
            this.columnColumn = new System.Windows.Forms.ColumnHeader();
            this.columnDescription = new System.Windows.Forms.ColumnHeader();
            this.columnProject = new System.Windows.Forms.ColumnHeader();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.SuspendLayout();
            // 
            // errorList
            // 
            this.errorList.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.errorList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnIcon,
            this.columnLine,
            this.columnColumn,
            this.columnDescription,
            this.columnProject});
            this.errorList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.errorList.FullRowSelect = true;
            this.errorList.Location = new System.Drawing.Point(0, 0);
            this.errorList.MultiSelect = false;
            this.errorList.Name = "errorList";
            this.errorList.Size = new System.Drawing.Size(802, 210);
            this.errorList.SmallImageList = this.imageList;
            this.errorList.Sorting = System.Windows.Forms.SortOrder.Descending;
            this.errorList.TabIndex = 0;
            this.errorList.UseCompatibleStateImageBehavior = false;
            this.errorList.View = System.Windows.Forms.View.Details;
            // 
            // columnIcon
            // 
            this.columnIcon.Text = "";
            this.columnIcon.Width = 24;
            // 
            // columnLine
            // 
            this.columnLine.Text = "Line";
            this.columnLine.Width = 80;
            // 
            // columnColumn
            // 
            this.columnColumn.Text = "Column";
            this.columnColumn.Width = 80;
            // 
            // columnDescription
            // 
            this.columnDescription.Text = "Description";
            this.columnDescription.Width = 360;
            // 
            // columnProject
            // 
            this.columnProject.Text = "Project";
            this.columnProject.Width = 120;
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList.Images.SetKeyName(0, "Error");
            this.imageList.Images.SetKeyName(1, "Warning");
            this.imageList.Images.SetKeyName(2, "Info");
            // 
            // ToolErrorList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(802, 210);
            this.Controls.Add(this.errorList);
            this.DockAreas = ((DockPanelSuite.DockAreas)(((((DockPanelSuite.DockAreas.Float | DockPanelSuite.DockAreas.DockLeft)
                        | DockPanelSuite.DockAreas.DockRight)
                        | DockPanelSuite.DockAreas.DockTop)
                        | DockPanelSuite.DockAreas.DockBottom)));
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ToolErrorList";
            this.TabText = "Error List";
            this.Text = "Error List";
            this.Resize += new System.EventHandler(this.ToolErrorList_Resize);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ColumnHeader columnIcon;
        private System.Windows.Forms.ColumnHeader columnDescription;
        private System.Windows.Forms.ColumnHeader columnLine;
        private System.Windows.Forms.ColumnHeader columnColumn;
        private System.Windows.Forms.ColumnHeader columnProject;
        private System.Windows.Forms.ImageList imageList;
        public System.Windows.Forms.ListView errorList;

    }
}