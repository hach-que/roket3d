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
    public partial class ToolBuildOutput : DockContent
    {
        public ToolBuildOutput()
        {
            InitializeComponent();
        }

        public void AddLogEntry(String Message)
        {
            buildOutputTextBox.Text += Message + @"
";
            buildOutputTextBox.SelectionStart = buildOutputTextBox.Text.Length;
            buildOutputTextBox.SelectionLength = 0;
            buildOutputTextBox.ScrollToCaret();
        }

        public void ClearLog()
        {
            buildOutputTextBox.Text = "";
        }
    }
}
