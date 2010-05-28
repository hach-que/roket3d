using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Roket3D.Menus.Definitions.Solution
{
    class New : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.solution_create;
            this.Enabled = true;
        }

        public override String GetText()
        {
            return "New Solution";
        }

        public override void OnSolutionOpen()
        {
            this.Enabled = false;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnSolutionClose()
        {
            this.Enabled = true;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnActivate()
        {
            NewSolutionForm nsf = new NewSolutionForm();
            if (nsf.ShowDialog() == DialogResult.OK)
            {
                NewSolutionForm.HandleNewProjectCreation(nsf);
            }
        }
    }

    class Open : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.solution_open;
            this.Enabled = true;
        }

        public override String GetText()
        {
            return "Open Solution";
        }

        public override void OnSolutionOpen()
        {
            this.Enabled = false;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnSolutionClose()
        {
            this.Enabled = true;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnActivate()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.CheckFileExists = true;
            ofd.CheckPathExists = true;
            ofd.RestoreDirectory = true;
            ofd.Filter = "Roket3D Solutions|*.rsln|Roket3D Projects|*.rproj";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                String filename = ofd.FileName;
                Program.MainWindow.CurrentSolution.Load(filename);
                Program.MainWindow.SolutionExplorer.ReloadTree();
                Program.MainWindow.SendMenuEvent(MenuEvent.SOLUTION_OPEN);
            }
        }
    }

    class Close : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Close Solution";
        }

        public override void OnActivate()
        {
            // TODO: Implement asking whether to save changes here.
            Program.MainWindow.CurrentSolution.Unload();
            Program.MainWindow.SolutionExplorer.ReloadTree();
            Program.MainWindow.SendMenuEvent(MenuEvent.SOLUTION_CLOSE);
        }
    }

    class StartWithDebug : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.debug;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Start with Debugging";
        }
    }

    class StartWithoutDebug : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.debug_without;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Start without Debugging";
        }
    }
}
