using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;

namespace Roket3D.Xml
{
    public class Solution
    {
        private Boolean _initalized = false;
        private List<Project> _projects;
        private String filename = "<unknown>";
        public Boolean HasLoaded = false;

        public Solution()
        {
            this._projects = new List<Project>();
            this._initalized = true;
        }

        public List<Project> Projects
        {
            get
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Solution class has not yet been initalized.");

                return _projects;
            }
        }

        public String SolutionName
        {
            get
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Solution class has not yet been initalized.");

                return this.filename.Substring(0, this.filename.LastIndexOf("."));
            }
        }

        #region Conversion

        public FileTreeNode ToTreeNode()
        {
            if (this.HasLoaded == false)
            {
                // Return a root node that indicates no solution open.
                FileTreeNode nrootNode = new FileTreeNode("No Solution Loaded");

                nrootNode.ImageKey = "Solution";
                nrootNode.SelectedImageKey = nrootNode.ImageKey;

                return nrootNode;
            }

            String solutionName = new FileInfo(this.filename).Name;
            solutionName = solutionName.Substring(0, solutionName.Length -
                            new FileInfo(this.filename).Extension.Length);
            FileTreeNode rootNode = new FileTreeNode("Solution '" + solutionName + "' " +
                                                        "(" + this._projects.Count.ToString() + " projects)");
            rootNode.solutionRef = this;
            
            rootNode.ImageKey = "Solution";
            rootNode.SelectedImageKey = rootNode.ImageKey;

            // Add the projects to the root node.
            foreach (Project p in this._projects)
            {
                rootNode.Nodes.Add(p.ToTreeNode());
            }

            return rootNode;
        }

        #endregion

        #region XML Reading
        public void Load(XmlReader reader)
        {
            if (!this._initalized)
                throw new NullReferenceException("The Solution class has not yet been initalized.");

            // This function assumes the working directory 
            // has been set to the location that we are reading
            // the solution file from (as it is impossible to
            // tell the original filename once in XmlReader form).

            List<XmlNodeStorage> nodeStack = new List<XmlNodeStorage>();

            // This reads solution data from an XmlReader.
            while (reader.Read())
            {
                Int32 currentLevel = nodeStack.Count - 1;
                if (reader.NodeType == XmlNodeType.Element)
                {
                    // Add node to stack.
                    nodeStack.Add(new XmlNodeStorage(reader.Name, reader.NodeType, reader.Value));
                    currentLevel += 1;

                    // Case insensitize the node name (in case this is not already
                    // done by the XmlReader class).
                    nodeStack[currentLevel].Name = nodeStack[currentLevel].Name.ToLower();
                }
                else if (reader.NodeType == XmlNodeType.Text)
                {
                    nodeStack[currentLevel].TextBuffer += reader.Value;
                }
                else if (reader.NodeType == XmlNodeType.CDATA)
                {
                    nodeStack[currentLevel].TextBuffer += reader.Value;
                }
                else if (reader.NodeType == XmlNodeType.EndElement)
                {
                    // Pop node from stack.
                    XmlNodeStorage currentNode = nodeStack[currentLevel];
                    nodeStack.RemoveAt(nodeStack.Count - 1);
                    currentLevel -= 1;

                    // Now that we've collect the data, we can handle our
                    // node :)
                    if (currentNode.Name == "solution")
                    {
                        // Nothing to do here really, but we should be
                        // at the end of the XML document, so throw a
                        // formatting exception if there are still nodes
                        // on the stack.
                        if (nodeStack.Count > 0)
                        {
                            throw new FormattingException("The solution node was not the top level " +
                                "node, or the solution node ended unexpectedly");
                        }
                    }
                    else if (currentNode.Name == "project")
                    {
                        // Okay, we've just recieved notification of a project
                        // that should be included in the solution.

                        Project newProj = new Project(currentNode.TextBuffer);
                        this._projects.Add(newProj);
                    }
                }
            }

            this.HasLoaded = true;
        }
        #endregion

        public void Load(String Filename)
        {
            // Calls Load with the specified filename and SetWorkingDirectory
            // defaulting to true.
            this.Load(Filename, true);
        }

        public void Load(String Filename, Boolean SetWorkingDirectory)
        {
            if (SetWorkingDirectory)
            {
                // Set the current working directory required
                // when using solutions (we should only ever have
                // one solution loaded at a time).
                FileInfo p = new FileInfo(Filename);
                Environment.CurrentDirectory = p.DirectoryName;
            }

            this.filename = Filename;
            XmlReader x = new XmlTextReader(Filename);
            this.Load(x);
            x.Close();
        }

        public void New(String solutionName, String solutionFileLocation, String solutionFolderLocation,
                        String projectName, String projectFileLocation, String projectFolderLocation)
        {
            // Find the relative path between the solutionFileLocation and projectFileLocation.
            String projectRelativeFileLocation = Solution.EvaluateRelativePath(solutionFolderLocation, projectFolderLocation);

            // Create the new solution file.            
            StreamWriter sStream = new StreamWriter(solutionFileLocation);
            sStream.Write("<?xml version=\"1.0\" encoding=\"utf-8\"?>" + @"
<Solution>
	<Project>" + projectRelativeFileLocation + "\\" + projectName + @".rproj</Project>	
</Solution>");
            sStream.Close();

            // Attempt to create the default directories.
            if (!System.IO.Directory.Exists(projectFolderLocation + "\\Models"))
                System.IO.Directory.CreateDirectory(projectFolderLocation + "\\Models");
            if (!System.IO.Directory.Exists(projectFolderLocation + "\\Images"))
                System.IO.Directory.CreateDirectory(projectFolderLocation + "\\Images");
            if (!System.IO.Directory.Exists(projectFolderLocation + "\\Audio"))
                System.IO.Directory.CreateDirectory(projectFolderLocation + "\\Audio");
            if (!System.IO.Directory.Exists(projectFolderLocation + "\\Templates"))
                System.IO.Directory.CreateDirectory(projectFolderLocation + "\\Templates");
            if (!System.IO.Directory.Exists(projectFolderLocation + "\\Areas"))
                System.IO.Directory.CreateDirectory(projectFolderLocation + "\\Areas");
            if (!System.IO.Directory.Exists(projectFolderLocation + "\\Worlds"))
                System.IO.Directory.CreateDirectory(projectFolderLocation + "\\Worlds");

            // Create the new project file.
            StreamWriter pStream = new StreamWriter(projectFileLocation);
            pStream.WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
            pStream.WriteLine("<Project ToolsVersion=\"1.5\">");
            pStream.WriteLine("	<Configuration Global=\"true\">");
            pStream.WriteLine("		<AssemblyName>" + projectName + "</AssemblyName>");
            pStream.WriteLine("		<AssemblyVersion>1.0.0.0</AssemblyVersion>");
            pStream.WriteLine("		<RootNamespace>" + projectName + "</RootNamespace>");
            pStream.WriteLine("		<TargetFrameworkVersion>v1.5</TargetFrameworkVersion>");
            pStream.WriteLine("		<AppDesignerFolder>Properties</AppDesignerFolder>");
            pStream.WriteLine("		<OutputType>Game</OutputType>");
            pStream.WriteLine("		<IDEVersion>1.5</IDEVersion>");
            pStream.WriteLine("		<SchemaVersion>1.0</SchemaVersion>");
            pStream.WriteLine("	</Configuration>");
            pStream.WriteLine("	<Configuration Name=\"Debug\" Default=\"true\">");
            pStream.WriteLine("		<DebugType>full</DebugType>");
            pStream.WriteLine("		<DebugStepByStep>allow</DebugStepByStep>");
            pStream.WriteLine("		<Optimize>full</Optimize>");
            pStream.WriteLine("		<OutputPath>bin\\Debug\\$(Platform)\\</OutputPath>");
            pStream.WriteLine("		<DefineConstants>DEBUG</DefineConstants>");
            pStream.WriteLine("	</Configuration>");
            pStream.WriteLine("	");
            pStream.WriteLine("	<!-- Project Items -->");
            pStream.WriteLine("	<ItemGroup>");
            pStream.WriteLine("    <EFolder Include=\"Models\" />");
            pStream.WriteLine("    <EFolder Include=\"Images\" />");
            pStream.WriteLine("    <EFolder Include=\"Audio\" />");
            pStream.WriteLine("    <EFolder Include=\"Templates\" />");
            pStream.WriteLine("    <EFolder Include=\"Areas\" />");
            pStream.WriteLine("    <EFolder Include=\"Worlds\" />");
            pStream.WriteLine("	</ItemGroup>");
            pStream.WriteLine("</Project>");
            pStream.Close();

            this.Load(solutionFileLocation);
        }

        /*
         * Code sourced from http://bytes.com/topic/c-sharp/answers/260727-absolute-path-relative-path-conversion#post1049132
         */
        private static String EvaluateRelativePath(String mainDirPath, String absoluteFilePath)
        {
            String[] firstPathParts = mainDirPath.Trim(Path.DirectorySeparatorChar).Split(Path.DirectorySeparatorChar);
            String[] secondPathParts = absoluteFilePath.Trim(Path.DirectorySeparatorChar).Split(Path.DirectorySeparatorChar);

            int sameCounter=0;
            for (Int32 i = 0; i < Math.Min(firstPathParts.Length, secondPathParts.Length); i++)
            {
                if(!firstPathParts[i].ToLower().Equals(secondPathParts[i].ToLower()) )
                {
                    break;
                }
                sameCounter++;
            }

            if (sameCounter==0)
            {
                return absoluteFilePath;
            }

            String newPath = String.Empty;
            for (Int32 i = sameCounter; i < firstPathParts.Length; i++)
            {
                if (i>sameCounter)
                {
                    newPath += Path.DirectorySeparatorChar;
                }
                newPath += "..";
            }
            if (newPath.Length==0)
            {
                newPath=".";
            }

            for (Int32 i = sameCounter; i < secondPathParts.Length; i++)
            {
                newPath += Path.DirectorySeparatorChar;
                newPath += secondPathParts[i];
            }
            return newPath;
        }

        public void Unload()
        {
            Environment.CurrentDirectory = Program.ROOT_PATH;
            this.Projects.Clear();
            this.HasLoaded = false;
        }

        private class XmlNodeStorage
        {
            public String Name;
            public XmlNodeType Type;
            public String Value;
            public String TextBuffer;

            public XmlNodeStorage(String Name, XmlNodeType Type, String Value)
            {
                this.Name = Name;
                this.Type = Type;
                this.Value = Value;
            }
        }
    }

    public class FileTreeNode : TreeNode
    {
        // FileTreeNodes are also used for project and solution nodes.
        public File fileRef = null;
        public Project projectRef = null;
        public Solution solutionRef = null;

        public FileTreeNode() {}
        public FileTreeNode(string text)
        {
            this.Text = text;
        }
    }

    class FormattingException : Exception
    {
        public FormattingException() {}
        public FormattingException(string message) {}
        public FormattingException(string message, System.Exception inner) {}
     
        // Constructor needed for serialization 
        // when exception propagates from a remoting server to the client.
        protected FormattingException(System.Runtime.Serialization.SerializationInfo info,
            System.Runtime.Serialization.StreamingContext context) {}
    }
}
