using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Roket3D.Menus.Definitions.Actions
{
    class Open : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_open;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Open File";
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
            return "Close";
        }
    }

    class Save : Action
    {
        private Roket3D.ContentEditors.EditorBase m_CurrentEditor = null;
        private string m_Text = "Save";

        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_save;
            this.Enabled = false;
        }

        public override void OnActivate()
        {
            m_CurrentEditor.OnSaveFile();
        }

        public override String GetText()
        {
            return this.m_Text;
        }

        public override void OnTabChanged(Roket3D.ContentEditors.EditorBase editor)
        {
            if (editor == null || editor.File == null)
            {
                this.Enabled = false;
                return;
            }
            
            this.Enabled = editor.Properties.CanSave;
            this.m_CurrentEditor = editor;
            this.m_Text = "Save " + editor.File.Name;
            this.Item.Enabled = this.Enabled;
        }
    }

    class SaveAs : Action
    {
        private Roket3D.ContentEditors.EditorBase m_CurrentEditor = null;
        private string m_Text = "Save as...";

        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override void OnActivate()
        {
            // TODO: Implement Save As...
        }

        public override String GetText()
        {
            return this.m_Text;
        }

        public override void OnTabChanged(Roket3D.ContentEditors.EditorBase editor)
        {
            if (editor == null || editor.File == null)
            {
                this.Enabled = false;
                return;
            }

            this.Enabled = false; // The activate event isn't implement yet..
            this.m_CurrentEditor = editor;
            this.m_Text = "Save " + editor.File.Name + " as...";
            this.Item.Enabled = this.Enabled;
        }
    }

    class SaveAll : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_save_all;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Save All";
        }
    }

    class Exit : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = true;
        }

        public override void OnActivate()
        {
            // TODO: Add proper unsaved changes checking etc.. here
            Program.MainWindow.Close();
        }

        public override String GetText()
        {
            return "Exit";
        }

        public override void OnSolutionOpen()
        {
            this.Enabled = true;
            this.Item.Enabled = this.Enabled;
        }

        public override void OnSolutionClose()
        {
            this.Enabled = true;
            this.Item.Enabled = this.Enabled;
        }
    }

    class Undo : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_undo;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Undo";
        }
    }

    class Redo : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_redo;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Redo";
        }
    }

    class Cut : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_cut;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Cut";
        }
    }

    class Copy : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_copy;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Copy";
        }
    }

    class Paste : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_paste;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Paste";
        }
    }

    class Delete : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Delete";
        }
    }

    class SelectAll : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Select All";
        }
    }

    class QuickFind : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Quick Find";
        }
    }

    class QuickReplace : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Quick Replace";
        }
    }

    class FindInFiles : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.actions_find_in_files;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Find in Files";
        }
    }

    class ReplaceInFiles : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Replace in Files";
        }
    }

    class GoTo : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Go To...";
        }
    }

    class Preferences : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Preferences";
        }
    }
}
