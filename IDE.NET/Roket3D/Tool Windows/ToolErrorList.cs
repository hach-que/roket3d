using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DockPanelSuite;

namespace Roket3D.ToolWindows
{
    public partial class ToolErrorList : DockContent
    {
        public ToolErrorList()
        {
            InitializeComponent();
        }

        private void ToolErrorList_Resize(object sender, EventArgs e)
        {
            columnIcon.Width = 24;
            columnDescription.Width = errorList.Width - 24 - 16 - 120 - 60 * 2;
            columnLine.Width = 60;
            columnColumn.Width = 60;
            columnProject.Width = 120;
        }

        public void ClearErrorsForFile(String filename, String project)
        {
            ListViewGroup fileGroup = null;
            foreach (ListViewGroup g in errorList.Groups)
            {
                if (g.Header == filename)
                {
                    fileGroup = g;
                }
            }

            if (fileGroup == null) return;

            for (Int32 i = 0; i < fileGroup.Items.Count; i += 1)
            {
                if (fileGroup.Items[i].SubItems[4].Text == project)
                {
                    ListViewItem li = fileGroup.Items[i];
                    fileGroup.Items.Remove(li);
                    errorList.Items.Remove(li);
                    i -= 1; // readjust current index because we removed one.
                }
            }
        }

        public void AddError(String description, String filename, Int32 line, Int32 column, String project)
        {
            // Get the group.
            ListViewGroup fileGroup = null;
            foreach (ListViewGroup g in errorList.Groups)
            {
                if (g.Header == filename)
                {
                    fileGroup = g;
                }
            }
            if (fileGroup == null)
            {
                fileGroup = errorList.Groups.Add("dynamicGroup" + filename.Replace(".",""), filename);
            }

            ListViewItem newError = errorList.Items.Add("", "Error");
            newError.SubItems.Add(line.ToString());
            newError.SubItems.Add(column.ToString());
            newError.SubItems.Add(description);
            newError.SubItems.Add(project);
            newError.Group = fileGroup;
        }
    }
}
