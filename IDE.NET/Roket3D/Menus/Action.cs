using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Roket3D.Menus
{
    public abstract class Action
    {
        public Image ItemIcon = null;
        public Boolean Enabled = false;
        public ToolStripMenuItem MenuItem = null;
        public ToolStripItem Item = null;

        public Action() { }
        public virtual void OnSetSettings() { }
        public virtual void OnLoad() { }
        public virtual void OnActivate() { }
        public virtual String GetText() { return null; }
        public virtual void OnSolutionOpen() { this.Enabled = true; this.Item.Enabled = this.Enabled; }
        public virtual void OnSolutionClose() { this.Enabled = false; this.Item.Enabled = this.Enabled; }
        public virtual void OnTabChanged(Roket3D.ContentEditors.EditorBase editor) { }
    }
}
