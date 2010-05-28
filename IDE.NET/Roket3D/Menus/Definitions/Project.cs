using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Roket3D.Xml;
using Roket3D.Management;

namespace Roket3D.Menus.Definitions.Project
{
    class New : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.project_create;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "New Project";
        }
    }

    class AddFile : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.file_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add New Item";
        }
    }

    class AddModel : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.model_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Model...";
        }

        public override void OnActivate()
        {
            FileTreeNode ftn = new FileTreeNode();
            if (TargetDirectory != null)
                ftn.fileRef = TargetDirectory;
            else
                // TODO: Make this check the solution based on what file is currently has focus.
                ftn.projectRef = Program.MainWindow.CurrentSolution.Projects[0];
            Creation.AddModel(ftn);
        }
    }

    class AddImage : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.image_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Image...";
        }

        public override void OnActivate()
        {
            FileTreeNode ftn = new FileTreeNode();
            if (TargetDirectory != null)
                ftn.fileRef = TargetDirectory;
            else
                // TODO: Make this check the solution based on what file is currently has focus.
                ftn.projectRef = Program.MainWindow.CurrentSolution.Projects[0];
            Creation.AddImage(ftn);
        }
    }

    class AddAudio : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.audio_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Audio...";
        }
    }

    class AddTemplate : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = null; //Properties.Resources.template_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Template...";
        }
    }

    class AddArea : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.area_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Image...";
        }
    }

    class AddWorld : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.world_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add World...";
        }
    }

    class AddClass : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.class_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Class...";
        }
    }

    class AddScript : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.script_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Script...";
        }
    }

    class AddNewItem : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = null; // Properties.Resources.script_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add New Item";
        }
    }

    class AddExistingItem : Action
    {
        public File TargetDirectory = null;

        public override void OnSetSettings()
        {
            this.ItemIcon = null; // Properties.Resources.script_add;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Existing Item";
        }
    }

    class AddReference : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.reference;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Add Reference...";
        }
    }

    class ProjProperties : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Project Properties";
        }
    }

    class Build : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Build MyProject";
        }
    }

    class Rebuild : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Rebuild MyProject";
        }
    }

    class Clean : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Clean MyProject";
        }
    }
}
