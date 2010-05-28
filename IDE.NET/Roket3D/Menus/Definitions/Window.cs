using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Roket3D.Menus.Definitions.Window
{
    class NewWindow : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "New Window";
        }
    }

    class Split : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.windows_split;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Split";
        }
    }

    class SetFloating : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Floating";
        }
    }

    class SetDockable : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Dockable";
        }
    }

    class SetTabbedDocument : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Tabbed Document";
        }
    }

    class SetAutoHide : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.windows_autohide;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Auto Hide";
        }
    }

    class Hide : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Hide";
        }
    }

    class AutoHideAll : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Auto Hide All";
        }
    }

    class NewHorizontalTabGroup : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.windows_tile_horizontal;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "New Horizontal Tab Group";
        }
    }

    class NewVerticalTabGroup : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.windows_tile_vertical;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "New Vertical Tab Group";
        }
    }

    class CloseAll : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Close All Documents";
        }
    }

    class Reset : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Reset Window Layout";
        }
    }

    class Windows : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = null;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Windows...";
        }
    }
}
