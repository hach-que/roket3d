using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Roket3D.LuaDebugging;
using DockPanelSuite;

namespace Roket3D.ToolWindows
{
    public partial class ToolConsole : DockContent
    {
        // TODO: Run the LuaConsole and lua operations in another thread so that we can cancel it if the user accidently runs
        //       an infinite loop.
        private LuaConsole myConsole = null;
        private List<String> commandList = new List<String>();
        private Int32 currentCommand = 0;

        public ToolConsole()
        {
            InitializeComponent();
        }

        private void ToolConsole_Load(object sender, EventArgs e)
        {
            myConsole = new LuaConsole(this.consoleTextBox);
        }

        private void ToolConsole_Enter(object sender, EventArgs e)
        {
            commandTextBox.Focus();
        }

        private void commandTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                // Interpret it as command execute.
                myConsole.HandleCommand(commandTextBox.Text);
                commandList.Add(commandTextBox.Text);
                currentCommand = commandList.Count; // set the current command to the last
                consoleTextBox.Select(consoleTextBox.Text.Length - 1, 0);
                consoleTextBox.ScrollToCaret();
                commandTextBox.Text = "";
                commandTextBox.Focus();
                e.SuppressKeyPress = true;
            }
            if (e.KeyCode == Keys.Up)
            {
                // Previous command.
                if (currentCommand > 0)
                {
                    currentCommand -= 1;
                    commandTextBox.Text = commandList[currentCommand];
                }
                e.SuppressKeyPress = true;
            }
            if (e.KeyCode == Keys.Down)
            {
                // Previous command.
                if (currentCommand < commandList.Count - 1)
                {
                    currentCommand += 1;
                    commandTextBox.Text = commandList[currentCommand];
                }
                else if (currentCommand == commandList.Count - 1)
                {
                    currentCommand += 1;
                    commandTextBox.Text = "";
                }
                e.SuppressKeyPress = true;
            }
        }
    }
}
