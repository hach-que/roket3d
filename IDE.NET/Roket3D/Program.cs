using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Roket3D
{
    static class Program
    {
        public static String ROOT_PATH = null;
        public static String DEFAULT_PROJECT_AREA = null;
        public static MainForm MainWindow = null;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Program.ROOT_PATH = Environment.CurrentDirectory;
            Program.DEFAULT_PROJECT_AREA = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)
                                                + "\\Roket3D Projects";
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            SplashScreen splash = new SplashScreen();
            splash.Show();
            Program.MainWindow = new MainForm(splash);
            Application.Run(Program.MainWindow);
        }
    }
}
