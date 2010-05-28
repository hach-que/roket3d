using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Roket3D
{
    public partial class NewSolutionForm : Form
    {
        public NewSolutionForm()
        {
            InitializeComponent();

            if (!Directory.Exists(Program.DEFAULT_PROJECT_AREA))
            {
                Directory.CreateDirectory(Program.DEFAULT_PROJECT_AREA);
            }
            solutionLocationTextBox.Text = Program.DEFAULT_PROJECT_AREA;
            UpdateProjectFolderLabel();
        }

        private void useSolutionFolderCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (useSolutionFolderCheckBox.Checked)
            {
                lblFolder.Text = "Solution Location:";
                solutionFolderTextBox.Enabled = true;
                solutionFolderTextBox.ReadOnly = false;
            }
            else
            {
                lblFolder.Text = "Project Location:";
                solutionFolderTextBox.Enabled = false;
                solutionFolderTextBox.ReadOnly = true;
            }
            UpdateProjectFolderLabel();
        }

        private void UpdateProjectFolderLabel()
        {
            String fL = solutionLocationTextBox.Text;
            if (!fL.EndsWith("\\"))
                fL += "\\";
            if (useSolutionFolderCheckBox.Checked)
            {
                solutionLocationLabel.Text = fL + solutionFolderTextBox.Text + "\\" + projectNameTextBox.Text;
            }
            else
            {
                solutionLocationLabel.Text = fL + projectNameTextBox.Text;
            }
        }

        private void projectNameTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateProjectFolderLabel();
            projectNameInvalidPictureBox.Visible = !this.IsValidName(projectNameTextBox.Text);
        }

        private void solutionLocationTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateProjectFolderLabel();
            solutionLocationInvalidPictureBox.Visible = !System.IO.Directory.Exists(solutionLocationTextBox.Text);
        }

        private void solutionFolderTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateProjectFolderLabel();
            solutionFolderInvalidPictureBox.Visible = !this.IsValidName(solutionFolderTextBox.Text);
        }

        private void NewSolutionForm_Load(object sender, EventArgs e)
        {
            // Select Managed by default.
            projectTypeListView.SelectedIndices.Clear();
            projectTypeListView.SelectedIndices.Add(0);
        }

        private Boolean IsValidName(String Name)
        {
            return (Name.IndexOfAny(new char[] { '\\', '/', ':', '*', '?', '"', '<', '>', '|' }) == -1);
        }

        private void browseSolutionLocationButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "Select a location to store the project or solution folder in.";
            if (!Directory.Exists(Program.DEFAULT_PROJECT_AREA))
            {
                Directory.CreateDirectory(Program.DEFAULT_PROJECT_AREA);
            }
            fbd.SelectedPath = Program.DEFAULT_PROJECT_AREA;
            fbd.ShowNewFolderButton = true;
            if (fbd.ShowDialog() == DialogResult.OK)
            {
                solutionLocationTextBox.Text = fbd.SelectedPath;
            }
        }

        private void projectTypeListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            projectTypeInvalidPictureBox.Visible = (projectTypeListView.SelectedItems.Count == 0);

            if (projectTypeListView.SelectedItems.Count == 1)
            {
                ListViewItem selectedItem = null;
                Boolean shouldProjectChangeName = false;
                Boolean shouldSolutionChangeName = false;
                selectedItem = projectTypeListView.SelectedItems[0];

                switch (projectNameTextBox.Text)
                {
                    case "ManagedGame":
                    case "UnmanagedGame":
                        shouldProjectChangeName = true;
                        break;
                }
                switch (solutionFolderTextBox.Text)
                {
                    case "ManagedSolution":
                    case "UnmanagedSolution":
                        shouldSolutionChangeName = true;
                        break;
                }

                switch (selectedItem.Text)
                {
                    case "Managed Lua":
                        if (shouldProjectChangeName)
                            projectNameTextBox.Text = "ManagedGame";
                        if (shouldSolutionChangeName)
                            solutionFolderTextBox.Text = "ManagedSolution";
                        templateDescriptionTextBox.Text = "Creates an IDE managed, Roket3D project with Lua as the scripting language.";
                        break;
                    case "Unmanaged Lua":
                        if (shouldProjectChangeName)
                            projectNameTextBox.Text = "UnmanagedGame";
                        if (shouldSolutionChangeName)
                            solutionFolderTextBox.Text = "UnmanagedSolution";
                        templateDescriptionTextBox.Text = "Creates an unmanaged Roket3D project with Lua as the scripting language.  Some features may not be available.";
                        break;
                }
            }
        }

        private void OKButton_Click(object sender, EventArgs e)
        {
            if (projectNameInvalidPictureBox.Visible || projectTypeInvalidPictureBox.Visible ||
                solutionFolderInvalidPictureBox.Visible || solutionLocationInvalidPictureBox.Visible)
            {
                MessageBox.Show("One or more of the provided fields is invalid.  Make sure that all of the fields are valid and a project type is selected and try again.",
                    "Invalid Information Provided", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        static public Boolean HandleNewProjectCreation(NewSolutionForm nsf)
        {
            String projectName = nsf.projectNameTextBox.Text;
            String solutionName = null;
            if (nsf.useSolutionFolderCheckBox.Checked)
                solutionName = nsf.solutionFolderTextBox.Text;
            else
                solutionName = projectName;

            String projectFileLocation = nsf.solutionLocationLabel.Text + "\\" + projectName + ".rproj";
            String projectFolderLocation = nsf.solutionLocationLabel.Text;
            String solutionFileLocation = null;
            String solutionFolderLocation = null;

            String fL = nsf.solutionLocationTextBox.Text;
            if (!fL.EndsWith("\\"))
                fL += "\\";

            if (nsf.useSolutionFolderCheckBox.Checked)
            {
                solutionFileLocation = fL + nsf.solutionFolderTextBox.Text + "\\" + solutionName + ".rsln";
                solutionFolderLocation = fL + nsf.solutionFolderTextBox.Text;
            }
            else
            {
                solutionFileLocation = nsf.solutionLocationLabel.Text + "\\" + solutionName + ".rsln";
                solutionFolderLocation = nsf.solutionLocationLabel.Text;
            }

            // Create the directories
            if (!System.IO.Directory.Exists(solutionFolderLocation))
            {
                System.IO.Directory.CreateDirectory(solutionFolderLocation);
            }
            if (!System.IO.Directory.Exists(projectFolderLocation))
            {
                System.IO.Directory.CreateDirectory(projectFolderLocation);
            }

            // TODO: Make use of the selected project type.
            Program.MainWindow.CurrentSolution.New(solutionName, solutionFileLocation, solutionFolderLocation,
                                                    projectName, projectFileLocation, projectFolderLocation);
            Program.MainWindow.SolutionExplorer.ReloadTree();
            Program.MainWindow.SendMenuEvent(MenuEvent.SOLUTION_OPEN);

            // TODO: Capture exceptions and report false if the creation failed.
            return true;
        }
    }
}
