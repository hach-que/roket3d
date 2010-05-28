using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Roket3D.Menus.Definitions.Tools
{
    class BuildOutput : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.tools_build_output;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Build Output";
        }
    }

    class ErrorList : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.tools_error_list;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Error List";
        }
    }

    class PropertiesWindow : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.tools_properties_window;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Properties Window";
        }
    }

    class SolutionExplorer : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.tools_solution_explorer;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Solution Explorer";
        }
    }

    class LuaConsole : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.tools_console;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Lua Console";
        }
    }

    class ObjectBrowser : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.tools_object_browser;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Object Browser";
        }
    }

    class Toolbox : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.tools_toolbox;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Toolbox";
        }
    }

    class Manage : Action
    {
        public override void OnSetSettings()
        {
            this.ItemIcon = Properties.Resources.tools_addons_manage;
            this.Enabled = false;
        }

        public override String GetText()
        {
            return "Manage Tools";
        }
    }
}
