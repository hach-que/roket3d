using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Roket3D.Menus.Definitions.Views
{
    class Code : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.view_code;
            this.Enabled = false;
        }

        public override void OnLoad()
        {
            if (this.MenuItem != null)
            {
                this.MenuItem.Checked = true;
            }
        }

        public override String GetText()
        {
            return "Code";
        }
    }

    class Designer : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.view_designer;
            this.Enabled = false;
        }

        public override void OnLoad()
        {
            if (this.MenuItem != null)
            {
                this.MenuItem.Checked = false;
            }
        }

        public override String GetText()
        {
            return "Designer";
        }
    }
}
