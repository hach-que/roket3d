using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Roket3D.Xml;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

namespace Roket3D.Management
{
    class Creation
    {
        public static void AddModel(FileTreeNode treeNode)
        {
            // Models must be imported from other sources.
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "All Supported Files|*.3ds;*.b3d;*.csm;*.dae;*.xml;*.dmf;*.x;*.obj;*.my3d;*.oct;*.mesh;*.lmts;*.bsp;*.md2;*.md3;*.rkm|" +
                         "3D Studio|*.3ds|" + 
                         "Bliz Basic B3D|*.b3d|" + 
                         "Cartography Shop 4 |*.csm|" + 
                         "COLLADA|*.dae;*.xml|" + 
                         "Delgine DeleD|*.dmf|" + 
                         "DirectX|*.x|" + 
                         "Maya|*.obj|" + 
                         "OCT|*.oct|" + 
                         "OGRE Meshes|*.mesh|" + 
                         "Quake 3 levels|*.bsp|" + 
                         "Quake 2 models|*.md2;*.md3|" + 
                         "Roket3D models|*.rkm|" + 
                         "All Files|*.*";
            ofd.CheckFileExists = true;
            ofd.CheckPathExists = true;
            ofd.RestoreDirectory = true;
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                // Add the model to the specified storage folder.
                Roket3D.Xml.File n = new Roket3D.Xml.File();
                n.Name = new FileInfo(ofd.FileName).Name;
                if (treeNode.projectRef != null)
                {
                    // Add directly to project.
                    n.Project = treeNode.projectRef;
                    n.Parent = null;
                    n.Exists = true;
                    treeNode.projectRef.Files.Add(n);
                }
                else if (treeNode.fileRef != null && treeNode.fileRef.Type == FileType.Folder)
                {
                    // Add to a folder.
                    n.Project = treeNode.fileRef.Project;
                    n.Parent = treeNode.fileRef;
                    n.Exists = true;
                    treeNode.fileRef.Files.Add(n);
                }
                else
                {
                    throw new ArgumentException("The FileTreeNode must be associated with a project or a folder.", "treeNode", null);
                }

                // Copy the file (and prompt for overwrite if needed).
                if (!Creation.AttemptToCopyFile(ofd.FileName, n.GetAbsolutePath()))
                {
                    if (treeNode.projectRef != null)
                        treeNode.projectRef.Files.Remove(n);
                    else
                        treeNode.fileRef.Files.Remove(n);
                }

                Program.MainWindow.SolutionExplorer.ReloadTree();
            }
        }

        public static void AddImage(FileTreeNode treeNode)
        {
            // Create the new File object.
            Roket3D.Xml.File n = new Roket3D.Xml.File();
            if (treeNode.projectRef != null)
            {
                // Add directly to project.
                n.Name = Creation.GetUniqueFilename(treeNode.projectRef.FullPath, "image", "png");
                n.Project = treeNode.projectRef;
                n.Parent = null;
                n.Exists = (n.Name != null);
                if (n.Name != null)
                    treeNode.projectRef.Files.Add(n);
            }
            else if (treeNode.fileRef != null && treeNode.fileRef.Type == FileType.Folder)
            {
                // Add to a folder.
                n.Name = Creation.GetUniqueFilename(treeNode.fileRef.GetAbsolutePath(), "image", "png");
                n.Project = treeNode.fileRef.Project;
                n.Parent = treeNode.fileRef;
                n.Exists = (n.Name != null);
                if (n.Name != null)
                    treeNode.fileRef.Files.Add(n);
            }
            else
            {
                throw new ArgumentException("The FileTreeNode must be associated with a project or a folder.", "treeNode", null);
            }

            if (n.Name == null)
                throw new IOException("Unable to determine a unique filename for the new image.");

            // Create a new transparent, 128x128 PNG image.
            Bitmap newImage = new Bitmap(128, 128, PixelFormat.Format32bppArgb);
            newImage.Save(n.GetAbsolutePath());

            Program.MainWindow.SolutionExplorer.ReloadTree();
        }

        public static String GetUniqueFilename(String location, String prefix, String extension)
        {
            if (!Directory.Exists(location))
                return null;

            DirectoryInfo d = new DirectoryInfo(location);
            FileInfo[] fs = d.GetFiles(prefix + "*." + extension);
            Boolean validated = false;
            String tryString = prefix + "." + extension;
            Int32 inc = 0;
            while (!validated)
            {
                Boolean insideValidated = true;
                foreach (FileInfo f in fs)
                {
                    if (f.Name == tryString)
                    {
                        inc += 1;
                        tryString = prefix + inc.ToString() + "." + extension;
                        insideValidated = false;
                        break;
                    }
                }
                if (System.IO.File.Exists(tryString))
                {
                    inc += 1;
                    tryString = prefix + inc.ToString() + "." + extension;
                    insideValidated = false;
                }
                validated = insideValidated;
            }

            return tryString;
        }

        public static Boolean AttemptToCopyFile(String source, String destination)
        {
            try
            {
                System.IO.File.Copy(source, destination);
                return true;
            }
            catch (IOException)
            {
                if (System.IO.File.Exists(destination))
                {
                    if (MessageBox.Show(
                        "The target file (" +
                        destination +
                        ") already exists.  Do you want to override it with the new file?",
                        "File Exists",
                        MessageBoxButtons.YesNo,
                        MessageBoxIcon.Warning) == DialogResult.Yes)
                    {
                        System.IO.File.Copy(source, destination, true);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                    return false;
            }
            catch
            {
                return false;
            }
        }
    }
}
