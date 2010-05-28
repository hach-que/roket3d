namespace Roket3D
{
    partial class NewSolutionForm
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
            System.Windows.Forms.ListViewGroup listViewGroup2 = new System.Windows.Forms.ListViewGroup("Roket3D Templates", System.Windows.Forms.HorizontalAlignment.Left);
            System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem("Managed Lua", "LuaManaged");
            System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem("Unmanaged Lua", "LuaUnmanaged");
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NewSolutionForm));
            this.projectTypeListView = new System.Windows.Forms.ListView();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.templateDescriptionTextBox = new System.Windows.Forms.TextBox();
            this.nameLabel = new System.Windows.Forms.Label();
            this.projectNameTextBox = new System.Windows.Forms.TextBox();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.cancelButton = new System.Windows.Forms.Button();
            this.OKButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.solutionLocationTextBox = new System.Windows.Forms.TextBox();
            this.lblFolder = new System.Windows.Forms.Label();
            this.solutionFolderTextBox = new System.Windows.Forms.TextBox();
            this.useSolutionFolderCheckBox = new System.Windows.Forms.CheckBox();
            this.solutionLocationLabel = new System.Windows.Forms.Label();
            this.solutionFolderInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.solutionLocationInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.projectNameInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.browseSolutionLocationButton = new System.Windows.Forms.Button();
            this.projectTypeInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.flowLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.solutionFolderInvalidPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.solutionLocationInvalidPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.projectNameInvalidPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.projectTypeInvalidPictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // projectTypeListView
            // 
            this.projectTypeListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            listViewGroup2.Header = "Roket3D Templates";
            listViewGroup2.Name = "lvgTemplates";
            this.projectTypeListView.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] {
            listViewGroup2});
            this.projectTypeListView.HideSelection = false;
            listViewItem3.Group = listViewGroup2;
            listViewItem4.Group = listViewGroup2;
            this.projectTypeListView.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem3,
            listViewItem4});
            this.projectTypeListView.LargeImageList = this.imageList;
            this.projectTypeListView.Location = new System.Drawing.Point(12, 12);
            this.projectTypeListView.MultiSelect = false;
            this.projectTypeListView.Name = "projectTypeListView";
            this.projectTypeListView.Size = new System.Drawing.Size(592, 138);
            this.projectTypeListView.TabIndex = 0;
            this.projectTypeListView.UseCompatibleStateImageBehavior = false;
            this.projectTypeListView.SelectedIndexChanged += new System.EventHandler(this.projectTypeListView_SelectedIndexChanged);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList.Images.SetKeyName(0, "LuaManaged");
            this.imageList.Images.SetKeyName(1, "LuaUnmanaged");
            // 
            // templateDescriptionTextBox
            // 
            this.templateDescriptionTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.templateDescriptionTextBox.Location = new System.Drawing.Point(13, 156);
            this.templateDescriptionTextBox.Name = "templateDescriptionTextBox";
            this.templateDescriptionTextBox.ReadOnly = true;
            this.templateDescriptionTextBox.Size = new System.Drawing.Size(591, 20);
            this.templateDescriptionTextBox.TabIndex = 1;
            this.templateDescriptionTextBox.Text = "Creates an IDE managed, Roket3D project with Lua as the primary scripting languag" +
                "e.";
            // 
            // nameLabel
            // 
            this.nameLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.nameLabel.AutoSize = true;
            this.nameLabel.Location = new System.Drawing.Point(12, 186);
            this.nameLabel.Name = "nameLabel";
            this.nameLabel.Size = new System.Drawing.Size(38, 13);
            this.nameLabel.TabIndex = 2;
            this.nameLabel.Text = "Name:";
            // 
            // projectNameTextBox
            // 
            this.projectNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.projectNameTextBox.Location = new System.Drawing.Point(154, 183);
            this.projectNameTextBox.Name = "projectNameTextBox";
            this.projectNameTextBox.Size = new System.Drawing.Size(353, 20);
            this.projectNameTextBox.TabIndex = 3;
            this.projectNameTextBox.Text = "ManagedGame";
            this.projectNameTextBox.TextChanged += new System.EventHandler(this.projectNameTextBox_TextChanged);
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.flowLayoutPanel1.Controls.Add(this.cancelButton);
            this.flowLayoutPanel1.Controls.Add(this.OKButton);
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(439, 281);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(168, 29);
            this.flowLayoutPanel1.TabIndex = 4;
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(90, 3);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 1;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // OKButton
            // 
            this.OKButton.Location = new System.Drawing.Point(9, 3);
            this.OKButton.Name = "OKButton";
            this.OKButton.Size = new System.Drawing.Size(75, 23);
            this.OKButton.TabIndex = 0;
            this.OKButton.Text = "OK";
            this.OKButton.UseVisualStyleBackColor = true;
            this.OKButton.Click += new System.EventHandler(this.OKButton_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Location = new System.Drawing.Point(15, 259);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(589, 7);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            // 
            // solutionLocationTextBox
            // 
            this.solutionLocationTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.solutionLocationTextBox.Location = new System.Drawing.Point(154, 209);
            this.solutionLocationTextBox.Name = "solutionLocationTextBox";
            this.solutionLocationTextBox.Size = new System.Drawing.Size(353, 20);
            this.solutionLocationTextBox.TabIndex = 7;
            this.solutionLocationTextBox.Text = "Z:\\My Documents\\Roket3D Projects\\";
            this.solutionLocationTextBox.TextChanged += new System.EventHandler(this.solutionLocationTextBox_TextChanged);
            // 
            // lblFolder
            // 
            this.lblFolder.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblFolder.AutoSize = true;
            this.lblFolder.Location = new System.Drawing.Point(12, 212);
            this.lblFolder.Name = "lblFolder";
            this.lblFolder.Size = new System.Drawing.Size(87, 13);
            this.lblFolder.TabIndex = 6;
            this.lblFolder.Text = "Project Location:";
            // 
            // solutionFolderTextBox
            // 
            this.solutionFolderTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.solutionFolderTextBox.Enabled = false;
            this.solutionFolderTextBox.Location = new System.Drawing.Point(154, 235);
            this.solutionFolderTextBox.Name = "solutionFolderTextBox";
            this.solutionFolderTextBox.ReadOnly = true;
            this.solutionFolderTextBox.Size = new System.Drawing.Size(353, 20);
            this.solutionFolderTextBox.TabIndex = 9;
            this.solutionFolderTextBox.Text = "ManagedSolution";
            this.solutionFolderTextBox.TextChanged += new System.EventHandler(this.solutionFolderTextBox_TextChanged);
            // 
            // useSolutionFolderCheckBox
            // 
            this.useSolutionFolderCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.useSolutionFolderCheckBox.AutoSize = true;
            this.useSolutionFolderCheckBox.CheckAlign = System.Drawing.ContentAlignment.TopLeft;
            this.useSolutionFolderCheckBox.Location = new System.Drawing.Point(15, 236);
            this.useSolutionFolderCheckBox.Name = "useSolutionFolderCheckBox";
            this.useSolutionFolderCheckBox.Size = new System.Drawing.Size(121, 17);
            this.useSolutionFolderCheckBox.TabIndex = 10;
            this.useSolutionFolderCheckBox.Text = "Use Solution Folder:";
            this.useSolutionFolderCheckBox.UseVisualStyleBackColor = true;
            this.useSolutionFolderCheckBox.CheckedChanged += new System.EventHandler(this.useSolutionFolderCheckBox_CheckedChanged);
            // 
            // solutionLocationLabel
            // 
            this.solutionLocationLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.solutionLocationLabel.AutoEllipsis = true;
            this.solutionLocationLabel.Location = new System.Drawing.Point(37, 281);
            this.solutionLocationLabel.Name = "solutionLocationLabel";
            this.solutionLocationLabel.Size = new System.Drawing.Size(400, 41);
            this.solutionLocationLabel.TabIndex = 11;
            this.solutionLocationLabel.Text = "Z:\\My Documents\\Roket3D Projects\\ManagedGame";
            this.solutionLocationLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // solutionFolderInvalidPictureBox
            // 
            this.solutionFolderInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.solutionFolderInvalidPictureBox.Image = global::Roket3D.Properties.Resources.icon_invalid;
            this.solutionFolderInvalidPictureBox.Location = new System.Drawing.Point(513, 235);
            this.solutionFolderInvalidPictureBox.Name = "solutionFolderInvalidPictureBox";
            this.solutionFolderInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.solutionFolderInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.solutionFolderInvalidPictureBox.TabIndex = 14;
            this.solutionFolderInvalidPictureBox.TabStop = false;
            this.solutionFolderInvalidPictureBox.Visible = false;
            // 
            // solutionLocationInvalidPictureBox
            // 
            this.solutionLocationInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.solutionLocationInvalidPictureBox.Image = global::Roket3D.Properties.Resources.icon_invalid;
            this.solutionLocationInvalidPictureBox.Location = new System.Drawing.Point(545, 209);
            this.solutionLocationInvalidPictureBox.Name = "solutionLocationInvalidPictureBox";
            this.solutionLocationInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.solutionLocationInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.solutionLocationInvalidPictureBox.TabIndex = 13;
            this.solutionLocationInvalidPictureBox.TabStop = false;
            this.solutionLocationInvalidPictureBox.Visible = false;
            // 
            // projectNameInvalidPictureBox
            // 
            this.projectNameInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.projectNameInvalidPictureBox.Image = global::Roket3D.Properties.Resources.icon_invalid;
            this.projectNameInvalidPictureBox.Location = new System.Drawing.Point(513, 183);
            this.projectNameInvalidPictureBox.Name = "projectNameInvalidPictureBox";
            this.projectNameInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.projectNameInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.projectNameInvalidPictureBox.TabIndex = 12;
            this.projectNameInvalidPictureBox.TabStop = false;
            this.projectNameInvalidPictureBox.Visible = false;
            // 
            // browseSolutionLocationButton
            // 
            this.browseSolutionLocationButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.browseSolutionLocationButton.Location = new System.Drawing.Point(513, 209);
            this.browseSolutionLocationButton.Name = "browseSolutionLocationButton";
            this.browseSolutionLocationButton.Size = new System.Drawing.Size(26, 20);
            this.browseSolutionLocationButton.TabIndex = 15;
            this.browseSolutionLocationButton.Text = "...";
            this.browseSolutionLocationButton.UseVisualStyleBackColor = true;
            this.browseSolutionLocationButton.Click += new System.EventHandler(this.browseSolutionLocationButton_Click);
            // 
            // projectTypeInvalidPictureBox
            // 
            this.projectTypeInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.projectTypeInvalidPictureBox.BackColor = System.Drawing.Color.White;
            this.projectTypeInvalidPictureBox.Image = global::Roket3D.Properties.Resources.icon_invalid;
            this.projectTypeInvalidPictureBox.Location = new System.Drawing.Point(581, 16);
            this.projectTypeInvalidPictureBox.Name = "projectTypeInvalidPictureBox";
            this.projectTypeInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.projectTypeInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.projectTypeInvalidPictureBox.TabIndex = 16;
            this.projectTypeInvalidPictureBox.TabStop = false;
            this.projectTypeInvalidPictureBox.Visible = false;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoEllipsis = true;
            this.label1.Location = new System.Drawing.Point(16, 269);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(417, 14);
            this.label1.TabIndex = 17;
            this.label1.Text = "Project will be created at:";
            // 
            // NewSolutionForm
            // 
            this.AcceptButton = this.OKButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(616, 322);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.projectTypeInvalidPictureBox);
            this.Controls.Add(this.browseSolutionLocationButton);
            this.Controls.Add(this.solutionFolderInvalidPictureBox);
            this.Controls.Add(this.solutionLocationInvalidPictureBox);
            this.Controls.Add(this.projectNameInvalidPictureBox);
            this.Controls.Add(this.solutionLocationLabel);
            this.Controls.Add(this.useSolutionFolderCheckBox);
            this.Controls.Add(this.solutionFolderTextBox);
            this.Controls.Add(this.solutionLocationTextBox);
            this.Controls.Add(this.lblFolder);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.flowLayoutPanel1);
            this.Controls.Add(this.projectNameTextBox);
            this.Controls.Add(this.nameLabel);
            this.Controls.Add(this.templateDescriptionTextBox);
            this.Controls.Add(this.projectTypeListView);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(624, 349);
            this.Name = "NewSolutionForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "New Solution";
            this.Load += new System.EventHandler(this.NewSolutionForm_Load);
            this.flowLayoutPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.solutionFolderInvalidPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.solutionLocationInvalidPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.projectNameInvalidPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.projectTypeInvalidPictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.TextBox templateDescriptionTextBox;
        private System.Windows.Forms.Label nameLabel;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Button OKButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.GroupBox groupBox1;
        public System.Windows.Forms.TextBox projectNameTextBox;
        public System.Windows.Forms.ListView projectTypeListView;
        public System.Windows.Forms.TextBox solutionLocationTextBox;
        private System.Windows.Forms.Label lblFolder;
        public System.Windows.Forms.TextBox solutionFolderTextBox;
        private System.Windows.Forms.PictureBox projectNameInvalidPictureBox;
        private System.Windows.Forms.PictureBox solutionLocationInvalidPictureBox;
        private System.Windows.Forms.PictureBox solutionFolderInvalidPictureBox;
        private System.Windows.Forms.Button browseSolutionLocationButton;
        private System.Windows.Forms.PictureBox projectTypeInvalidPictureBox;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.CheckBox useSolutionFolderCheckBox;
        public System.Windows.Forms.Label solutionLocationLabel;
    }
}