using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;
using System.Reflection;

namespace Roket3D.Menus
{
    class MenuLoader
    {
        XmlReader Reader;
        public MenuStrip MainMenu = new MenuStrip();
        public ToolStrip ToolBar = new ToolStrip();

        private ToolStripMenuItem activeMenuItem = null;
        private ToolStripDropDownButton activeDropDown = null;
        private ToolStripComboBox activeComboBox = null;
        private ToolStripItem activeItem = null;
        private Assembly currentAssembly = Assembly.GetExecutingAssembly();
        private MainForm window = null;

        public MenuLoader(MainForm window)
        {
            if (!File.Exists(Program.ROOT_PATH + "\\Menus.xml"))
            {
                throw new Exception("Menu information file was not found.  Please make sure Menus.xml exists in the application directory.");
            }

            // style menubar and toolbar
            // TODO: Finish style system.
            //this.MainMenu.Renderer = new MenuRenderer();
            //this.ToolBar.Renderer = new ToolBarRenderer();

            this.window = window;
            this.window.MenuActions.Clear();

            this.Reader = XmlReader.Create(new StreamReader(Program.ROOT_PATH + "\\Menus.xml"));
            
            while (this.Reader.Read())
            {
                switch (this.Reader.NodeType)
                {
                    case XmlNodeType.Element:
                        switch (this.Reader.Name)
                        {
                            case "menus":
                            case "menubar":
                            case "toolbar":
                                // Nothing to do here.
                                break;
                            case "menuitem":
                                this.AddMenuItem(this.Reader.GetAttribute("text"));

                                this.AddReflectionHandler(this.Reader.GetAttribute("action"));

                                if (this.Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    activeItem = activeItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (activeItem != null && !(activeItem is ToolStripDropDownButton))
                                        activeItem.Enabled = true;
                                }
                                break;
                            case "menuseperator":
                                this.AddMenuItem("-");

                                if (this.Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    activeItem = activeItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (activeItem != null && !(activeItem is ToolStripDropDownButton))
                                        activeItem.Enabled = true;
                                }
                                break;
                            case "toolitem":
                                if (this.Reader.GetAttribute("type") == "dropdown")
                                    this.AddToolDropDown(this.Reader.GetAttribute("text"));
                                else
                                    this.AddToolItem(this.Reader.GetAttribute("text"));

                                this.AddReflectionHandler(this.Reader.GetAttribute("action"));

                                if (this.Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    activeItem = activeItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (activeItem != null)
                                        activeItem.Enabled = true;
                                }
                                break;
                            case "toolseperator":
                                this.AddToolItem("-");

                                if (this.Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    activeItem = activeItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (activeItem != null)
                                        activeItem.Enabled = true;
                                }
                                break;
                            case "toolcombo":
                                this.AddToolComboBox(this.Reader.GetAttribute("text"));

                                if (this.Reader.GetAttribute("editable") == "false")
                                    activeComboBox.DropDownStyle = ComboBoxStyle.DropDownList;

                                //this.AddReflectionHandler(this.Reader.GetAttribute("action"));

                                if (this.Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    activeItem = activeItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (activeItem != null)
                                        activeItem.Enabled = true;
                                }
                                break;
                            case "text":
                                this.UpdateObjects();

                                if (activeComboBox != null)
                                {
                                    Type enumType = currentAssembly.GetType(this.Reader.GetAttribute("type"));
                                    Array enumValues = Enum.GetValues(enumType);
                                    foreach (object o in enumValues)
                                    {
                                        String name = Enum.GetName(enumType, o);
                                        if (this.Reader.GetAttribute("type") + "." + name == this.Reader.GetAttribute("value"))
                                        {
                                            // HACK: This could probably be organised better by
                                            //       using classes instead of enums, but oh well..
                                            switch (this.Reader.GetAttribute("type"))
                                            {
                                                case "Roket3D.Compilation.BuildMode":
                                                    activeComboBox.Items.Add(
                                                        new EnumWrapper(
                                                            (Int32)o,
                                                            new List<String>() { "Debug", "Release" })
                                                        );
                                                    break;
                                                default:
                                                    activeComboBox.Items.Add(o);
                                                    break;
                                            }
                                        }
                                    }
                                }
                                break;
                        }
                        break;
                    case XmlNodeType.EndElement:
                        switch (this.Reader.Name)
                        {
                            case "menus":
                            case "menubar":
                            case "toolbar":
                                // Nothing to do here.
                                break;
                            case "menuitem":
                                activeItem = activeItem.OwnerItem;
                                this.UpdateObjects();

                                // The parent had some menu items, therefore we
                                // enable it regardless of whether it has an action.
                                if (activeItem != null && !(activeItem is ToolStripDropDownButton))
                                    activeItem.Enabled = true;
                                break;
                            case "toolitem":
                                activeItem = activeItem.OwnerItem;
                                this.UpdateObjects();

                                // The parent had some menu items, therefore we
                                // enable it regardless of whether it has an action.
                                if (activeItem != null)
                                    activeItem.Enabled = true;
                                break;
                            case "menuseperator":
                                activeItem = activeItem.OwnerItem;
                                this.UpdateObjects();

                                // The parent had some menu items, therefore we
                                // enable it regardless of whether it has an action.
                                if (activeItem != null && !(activeItem is ToolStripDropDownButton))
                                    activeItem.Enabled = true;
                                break;
                            case "toolseperator":
                                activeItem = activeItem.OwnerItem;
                                this.UpdateObjects();

                                // The parent had some menu items, therefore we
                                // enable it regardless of whether it has an action.
                                if (activeItem != null)
                                    activeItem.Enabled = true;
                                break;
                        }
                        break;
                }
            }

            this.Reader.Close();
        }

        private void AddMenuItem(String text)
        {
            if (activeItem == null)
            {
                activeItem = this.MainMenu.Items.Add(text);
                this.UpdateObjects();
            }
            else if (activeDropDown != null)
            {
                // Add to the drop down.
                activeItem = activeDropDown.DropDownItems.Add(text);
                this.UpdateObjects();
            }
            else
            {
                activeItem = activeMenuItem.DropDownItems.Add(text);
                this.UpdateObjects();
            }
        }

        private void AddToolItem(String text)
        {
            activeItem = this.ToolBar.Items.Add(text);
            activeItem.TextImageRelation = TextImageRelation.ImageAboveText;
            activeItem.Text = "";
            activeItem.Image = Properties.Resources.tools_unknown;
            this.UpdateObjects();
        }

        private void AddToolComboBox(String text)
        {
            activeComboBox = new ToolStripComboBox(text);
            activeItem = activeComboBox;
            activeItem.TextImageRelation = TextImageRelation.ImageAboveText;
            activeItem.Text = "";
            activeItem.Image = null;
            this.ToolBar.Items.Add(activeComboBox);
            this.UpdateObjects();
        }

        private void AddToolDropDown(String text)
        {
            activeDropDown = new ToolStripDropDownButton(text);
            activeItem = activeDropDown;
            activeItem.TextImageRelation = TextImageRelation.ImageAboveText;
            activeItem.Text = "";
            activeItem.Image = Properties.Resources.tools_unknown;
            this.ToolBar.Items.Add(activeDropDown);
            this.UpdateObjects();
        }

        private void AddReflectionHandler(String actionName)
        {
            if (actionName != null)
            {
                // Use reflection to associate the menu item with a defined
                // action.
                Type actionType = currentAssembly.GetType("Roket3D.Menus.Definitions." + actionName);
                if (actionType != null)
                {
                    object actionObj = Activator.CreateInstance(actionType);
                    if (actionObj is Action)
                    {
                        Action action = (Action)actionObj;
                        String resString = action.GetText();
                        if (resString != null)
                        {
                            if (activeMenuItem != null)
                                activeItem.Text = resString;
                            else
                                activeItem.ToolTipText = resString;
                        }
                        action.MenuItem = activeMenuItem;
                        action.Item = activeItem;
                        activeItem.Click += new EventHandler(delegate(object sender, EventArgs e)
                            {
                                action.OnActivate();
                            }
                        );
                        this.window.MenuActions.Add(action);

                        action.OnSetSettings();
                        activeItem.Enabled = action.Enabled;
                        if (action.ItemIcon != null)
                            activeItem.Image = action.ItemIcon;
                        action.OnLoad();
                    }
                    else
                        activeItem.Enabled = false;
                }
                else
                    activeItem.Enabled = false;
            }
            else
                activeItem.Enabled = false;
        }

        private void UpdateObjects()
        {
            if (activeItem is ToolStripMenuItem)
                activeMenuItem = (ToolStripMenuItem)activeItem;
            else
                activeMenuItem = null;
            
            if (activeItem is ToolStripDropDownButton)
                activeDropDown = (ToolStripDropDownButton)activeItem;
            else
                activeDropDown = null;

            if (activeItem is ToolStripComboBox)
                activeComboBox = (ToolStripComboBox)activeItem;
            else
                activeComboBox = null;                     
        }
    }
}
