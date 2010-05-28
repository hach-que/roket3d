using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;

namespace Roket3D.Xml
{
    public class Project
    {
        private Boolean _initalized = false;
        private Boolean _notFound;
        private String _fileName;
        private String _fullPath;
        private List<File> _files;
        private List<Reference> _references;
        private List<Configuration> _configurations;

        public Project()
        {
            this._files = new List<File>();
            this._references = new List<Reference>();
            this._configurations = new List<Configuration>();
            this._notFound = true;
            this._initalized = true;
            this._fileName = "<unknown>";
            this._fullPath = "<unknown>";
        }

        public Project(String Filename)
        {
            this._files = new List<File>();
            this._references = new List<Reference>();
            this._configurations = new List<Configuration>();
            this._notFound = false;
            this._initalized = true;
            this._fileName = new FileInfo(Filename).Name;
            this._fullPath = new FileInfo(Filename).FullName;

            // Now attempt to load
            if (System.IO.File.Exists(Filename))
            {
                XmlReader xml = new XmlTextReader(Filename);
                this.Load(xml);
            }
            else
            {
                this._notFound = true;
            }
        }

        public Boolean HasFile(String filename)
        {
            foreach (File f in this._files)
            {
                if (f.Name == filename)
                    return true;
            }
            return false;
        }

        public File GetFile(String filename)
        {
            foreach (File f in this._files)
            {
                if (f.Name == filename)
                    return f;
            }
            return null;
        }

        public DirectoryInfo GetRootFolder()
        {
            return new FileInfo(this._fullPath).Directory;
        }

        private void Load(XmlReader reader)
        {
            if (!this._initalized)
                throw new NullReferenceException("The Solution class has not yet been initalized.");

            // This function assumes the working directory 
            // has been set to the location that we are reading
            // the solution file from (as it is impossible to
            // tell the original filename once in XmlReader form).

            List<XmlNodeStorage> nodeStack = new List<XmlNodeStorage>();

            // These are our temporary data holders.
            Configuration tempConf = new Configuration();
            Reference tempReference = new Reference();
            File tempFile = new File();

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

                    if (nodeStack[currentLevel].Name == "configuration")
                    {
                        // Set the Global, Name and Default settings.
                        tempConf.Name = "<unknown>";
                        tempConf.Global = false;
                        tempConf.Default = false;

                        // Remember that attribute names are case sensitive!
                        if (reader.GetAttribute("Global") != null)
                        {
                            tempConf.Global = (reader.GetAttribute("Global").ToLower() == "true");
                            if (tempConf.Name == "<unknown>") tempConf.Name = "<global>";
                        }
                        if (reader.GetAttribute("Default") != null)
                        {
                            tempConf.Default = (reader.GetAttribute("Default").ToLower() == "true");
                        }
                        if (reader.GetAttribute("Name") != null)
                        {
                            tempConf.Name = reader.GetAttribute("Name");
                        }
                    }
                    else if (nodeStack[currentLevel].Name == "reference")
                    {
                        // Set the IncludeString settings.
                        tempReference.IncludeString = "<unknown>";

                        // Remember that attribute names are case sensitive!
                        if (reader.GetAttribute("Include") != null)
                        {
                            tempReference.IncludeString = reader.GetAttribute("Include");
                        }
                    }
                    else if (nodeStack[currentLevel].Name == "model" || 
                        nodeStack[currentLevel].Name == "image" || 
                        nodeStack[currentLevel].Name == "audio" || 
                        nodeStack[currentLevel].Name == "area" || 
                        nodeStack[currentLevel].Name == "world" || 
                        nodeStack[currentLevel].Name == "class" || 
                        nodeStack[currentLevel].Name == "script")
                    {
                        // Set the filename.
                        if (reader.GetAttribute("Include") != null)
                        {
                            tempFile.Name = reader.GetAttribute("Include");
                        }

                        if (reader.IsEmptyElement)
                        {
                            HandleFileAdd(tempFile);
                            tempFile = new File();
                        }
                    }
                    else if (nodeStack[currentLevel].Name == "efolder")
                    {
                        // Set the filename.
                        if (reader.GetAttribute("Include") != null)
                        {
                            tempFile.Name = reader.GetAttribute("Include");
                        }

                        if (reader.IsEmptyElement)
                        {
                            HandleFolderAdd(tempFile);
                            tempFile = new File();
                        }
                    }
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
                    if (currentNode.Name == "project")
                    {
                        // Nothing to do here really, but we should be
                        // at the end of the XML document, so throw a
                        // formatting exception if there are still nodes
                        // on the stack.
                        if (nodeStack.Count > 0)
                        {
                            throw new FormattingException("The project node was not the top level " +
                                "node, or the project node ended unexpectedly");
                        }
                    }
                    else if (currentNode.Name == "configuration")
                    {
                        // Save the temporary configuration data into the
                        // actual configuration list.
                        this._configurations.Add(tempConf);
                        tempConf = new Configuration();
                    }
                    else if (nodeStack[currentLevel].Name == "configuration")
                    {
                        // This is a configuration option.  Add it to the temporary
                        // configuration.
                        tempConf.Settings.Add(currentNode.Name, currentNode.TextBuffer);
                    }
                    else if (currentNode.Name == "reference")
                    {
                        // Save the temporary reference data into the
                        // actual reference list.
                        this._references.Add(tempReference);
                        tempReference = new Reference();
                    }
                    else if (nodeStack[currentLevel].Name == "reference")
                    {
                        // This is a reference option.  Add it to the temporary
                        // reference.
                        tempReference.Settings.Add(currentNode.Name, currentNode.TextBuffer);
                    }
                    else if (currentNode.Name == "model" ||
                        currentNode.Name == "image" ||
                        currentNode.Name == "audio" ||
                        currentNode.Name == "area" ||
                        currentNode.Name == "world" ||
                        currentNode.Name == "class" ||
                        currentNode.Name == "script")
                    {
                        // Save the temporary file data into the
                        // actual files list (if it's a valid file)
                        if (tempFile.Name != null && tempFile.Name != "")
                        {
                            // We need to split the path of the filename.
                            HandleFileAdd(tempFile);
                            tempFile = new File();
                        }
                    }
                    else if (nodeStack[currentLevel].Name == "model" ||
                        nodeStack[currentLevel].Name == "image" ||
                        nodeStack[currentLevel].Name == "audio" ||
                        nodeStack[currentLevel].Name == "area" ||
                        nodeStack[currentLevel].Name == "world" ||
                        nodeStack[currentLevel].Name == "class" ||
                        nodeStack[currentLevel].Name == "script")
                    {
                        // This is a file option.  Add it to the temporary file.
                        tempFile.Settings.Add(currentNode.Name, currentNode.TextBuffer);
                    }
                    else if (currentNode.Name == "efolder")
                    {
                        // Save the temporary file data into the
                        // actual files list (if it's a valid file)
                        if (tempFile.Name != null && tempFile.Name != "")
                        {
                            // We need to split the path of the filename.
                            HandleFolderAdd(tempFile);
                            tempFile = new File();
                        }
                    }
                }
            }
        }

        private void HandleFileAdd(File tempFile)
        {
            String[] pathComponents = tempFile.Name.Split('\\');
            String pathBuild = "";
            File parentFile = null;
            for (Int32 i = 0; i < pathComponents.Length; i++)
            {
                String pathComponent = pathComponents[i];
                pathBuild += pathComponent + '\\';
                if (i == 0)
                {
                    // This is a top-level file.
                    // Use the Files property of the project.
                    if (this.GetFile(pathComponent) == null)
                    {
                        // Create the file entry.
                        File f = new File();
                        f.Name = pathComponent;
                        f.Project = this;
                        f.Parent = parentFile;
                        f.Exists = (System.IO.File.Exists(pathBuild) || System.IO.Directory.Exists(pathBuild));
                        if (i != pathComponents.Length - 1)
                            f.Type = FileType.Folder;
                        else
                        {
                            f.Type = File.DetectTypeFromExtension(new FileInfo(pathComponent).Extension);
                            f.Settings = tempFile.Settings;
                        }
                        this._files.Add(f);
                    }

                    parentFile = this.GetFile(pathComponent);
                }
                else
                {
                    // Use the Files property of the parentFile.
                    if (parentFile.GetFile(pathComponent) == null)
                    {
                        // Create the file entry.
                        File f = new File();
                        f.Name = pathComponent;
                        f.Project = this;
                        f.Parent = parentFile;
                        f.Exists = (System.IO.File.Exists(pathBuild) || System.IO.Directory.Exists(pathBuild));
                        if (i != pathComponents.Length - 1)
                            f.Type = FileType.Folder;
                        else
                        {
                            f.Type = File.DetectTypeFromExtension(new FileInfo(pathComponent).Extension);
                            f.Settings = tempFile.Settings;
                        }
                        parentFile.Files.Add(f);
                    }

                    parentFile = parentFile.GetFile(pathComponent);
                }
            }
        }

        private void HandleFolderAdd(File tempFolder)
        {
            String[] pathComponents = tempFolder.Name.Split('\\');
            String pathBuild = "";
            File parentFile = null;
            for (Int32 i = 0; i < pathComponents.Length; i++)
            {
                String pathComponent = pathComponents[i];
                pathBuild += pathComponent + '\\';
                if (i == 0)
                {
                    // This is a top-level file.
                    // Use the Files property of the project.
                    if (this.GetFile(pathComponent) == null)
                    {
                        // Create the file entry.
                        File f = new File();
                        f.Name = pathComponent;
                        f.Project = this;
                        f.Parent = parentFile;
                        f.Exists = (System.IO.Directory.Exists(pathBuild));
                        f.Type = FileType.Folder;
                        this._files.Add(f);
                    }

                    parentFile = this.GetFile(pathComponent);
                }
                else
                {
                    // Use the Files property of the parentFile.
                    if (parentFile.GetFile(pathComponent) == null)
                    {
                        // Create the file entry.
                        File f = new File();
                        f.Name = pathComponent;
                        f.Project = this;
                        f.Parent = parentFile;
                        f.Exists = (System.IO.Directory.Exists(pathBuild));
                        f.Type = FileType.Folder;
                        parentFile.Files.Add(f);
                    }

                    parentFile = parentFile.GetFile(pathComponent);
                }
            }
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

        #region Properties
        public List<File> Files
        {
            get
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Project class has not yet been initalized.");

                return _files;
            }
        }

        public List<Reference> References
        {
            get
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Project class has not yet been initalized.");

                return _references;
            }
        }

        public Boolean NotFound
        {
            get
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Project class has not yet been initalized.");

                return _notFound;
            }
        }

        public String Filename
        {
            get
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Project class has not yet been initalized.");

                return _fileName;
            }
            set
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Project class has not yet been initalized.");

                this._fileName = value;
            }
        }

        public String FullPath
        {
            get
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Project class has not yet been initalized.");

                return _fullPath;
            }
            set
            {
                if (!this._initalized)
                    throw new NullReferenceException("The Project class has not yet been initalized.");

                this._fullPath = value;
            }
        }
        #endregion

        #region Configuration Retrieval
        public Configuration GetGlobalConfiguration()
        {
            foreach (Configuration i in this._configurations)
            {
                if (i.Global)
                {
                    return i;
                }
            }

            // We don't have a global configuration, throw an error.
            throw new NoSuchConfigurationException("There is no global configuration for this project.");
        }

        public Configuration GetConfiguration(String ConfigurationName)
        {
            return this.GetConfiguration(ConfigurationName, true);
        }

        public List<String> GetConfigurationList()
        {
            List<String> t = new List<String>();
            foreach (Configuration i in this._configurations)
            {
                t.Add(i.Name);
            }
            return t;
        }

        public Configuration GetConfiguration(String ConfigurationName, Boolean ApplyGlobalSettings)
        {
            Configuration targetConfiguration = null;
            foreach (Configuration i in this._configurations)
            {
                if (i.Name == ConfigurationName)
                {
                    targetConfiguration = i;
                    break;
                }
            }

            if (targetConfiguration == null)
            {
                // We don't have the requested configuration, throw an error.
                throw new NoSuchConfigurationException("The requested configuration was not found.");
            }

            // Now attempt to locate the global configuration, if we have one.
            Configuration globalConfiguration = null;
            if (ApplyGlobalSettings == true)
            {
                try
                {
                    globalConfiguration = this.GetGlobalConfiguration();
                }
                catch (NoSuchConfigurationException)
                {
                    globalConfiguration = null;
                }
            }

            // Apply the global configuration if needed.
            if (globalConfiguration == null)
            {
                return targetConfiguration;
            }
            else
            {
                return targetConfiguration.MergeDown(globalConfiguration);
            }

        }
        #endregion

        #region Conversion

        public TreeNode ToTreeNode()
        {
            String projectName = new FileInfo(this._fileName).Name;
            projectName = projectName.Substring(0, projectName.Length -
                            new FileInfo(this._fileName).Extension.Length);
            FileTreeNode rootNode = new FileTreeNode(projectName);
            rootNode.ImageKey = "Project";
            rootNode.SelectedImageKey = rootNode.ImageKey;
            rootNode.projectRef = this;

            foreach (File f in this._files)
            {
                rootNode.Nodes.Add(f.ToTreeNode());
            }

            return rootNode;
        }

        #endregion
    }

    public class Configuration
    {
        public String Name = "";
        public Boolean Global = false;
        public Boolean Default = false;
        public Dictionary<String, String> Settings = new Dictionary<String, String>();

        #region Merging Functions
        public Configuration MergeUp(Configuration conf)
        {
            // Merges a configuration with this one and returns the result.
            // The settings in conf have priority over the settings of this configuration.

            Configuration result = new Configuration();
            foreach (KeyValuePair<String, String> i in this.Settings)
            {
                result.Settings[i.Key] = i.Value;
            }
            foreach (KeyValuePair<String, String> i in conf.Settings)
            {
                result.Settings[i.Key] = i.Value;
            }
            return result;
        }

        public Configuration MergeDown(Configuration conf)
        {
            // Merges a configuration with this one and returns the result.
            // The settings in conf have priority over the settings of this configuration.

            Configuration result = new Configuration();
            foreach (KeyValuePair<String, String> i in conf.Settings)
            {
                result.Settings[i.Key] = i.Value;
            }
            foreach (KeyValuePair<String, String> i in this.Settings)
            {
                result.Settings[i.Key] = i.Value;
            }
            return result;
        }
        #endregion
    }

    public class File
    {
        public String Name = "";
        public FileType Type = FileType.Unknown;
        public File Parent = null;
        public Project Project = null;
        public List<File> Files = new List<File>();
        public Dictionary<String, String> Settings = new Dictionary<String, String>();
        public Boolean Exists = false;

        public Boolean HasFile(String filename)
        {
            foreach (File f in this.Files)
            {
                if (f.Name == filename)
                    return true;
            }
            return false;
        }

        public File GetFile(String filename)
        {
            foreach (File f in this.Files)
            {
                if (f.Name == filename)
                    return f;
            }
            return null;
        }

        public List<File> GetAllFilesOfType(FileType fileType)
        {
            List<File> files = new List<File>();
            foreach (File f in this.Files)
            {
                if (f.Type == fileType)
                    files.Add(f);

                files.AddRange(f.GetAllFilesOfType(fileType));
            }
            return files;
        }

        public String GetAbsolutePath()
        {
            if (this.Parent == null)
                return this.Project.GetRootFolder().ToString() + "\\" + this.Name;
            else
                return this.Parent.GetAbsolutePath() + "\\" + this.Name;
        }

        public String GetRelativePath()
        {
            if (this.Parent == null)
                return this.Name;
            else
                return this.Parent.GetAbsolutePath() + "\\" + this.Name;
        }

        public override String ToString()
        {
            if (this.Parent == null)
                return this.Name;
            else
                return this.Parent.ToString() + "\\" + this.Name;
        }

        #region Conversion

        public TreeNode ToTreeNode()
        {
            FileTreeNode rootNode = new FileTreeNode(this.Name);
            rootNode.fileRef = this;

            #region Detect ImageKey
            switch (this.Type)
            {
                case FileType.Area:
                    rootNode.ImageKey = "Area";
                    break;
                case FileType.Audio:
                    rootNode.ImageKey = "Audio";
                    break;
                case FileType.CHeader:
                    rootNode.ImageKey = "CHeader";
                    break;
                case FileType.CScript:
                    rootNode.ImageKey = "CScript";
                    break;
                case FileType.Folder:
                    rootNode.ImageKey = "Folder";
                    break;
                case FileType.Generic:
                    rootNode.ImageKey = "Generic";
                    break;
                case FileType.Image:
                    rootNode.ImageKey = "Image";
                    break;
                case FileType.Model:
                    rootNode.ImageKey = "Model";
                    break;
                case FileType.ProjectReference:
                    rootNode.ImageKey = "ProjectReference";
                    break;
                case FileType.Reference:
                    rootNode.ImageKey = "Reference";
                    break;
                case FileType.Script:
                    rootNode.ImageKey = "Script";
                    break;
                case FileType.World:
                    rootNode.ImageKey = "World";
                    break;
                case FileType.Unknown:
                    rootNode.ImageKey = "Unknown";
                    break;
            }
            #endregion

            if (!this.Exists)
                if (rootNode.ImageKey == "Folder")
                    rootNode.ImageKey = "NotFoundFolder";
                else
                    rootNode.ImageKey = "NotFound";

            rootNode.SelectedImageKey = rootNode.ImageKey;

            foreach (File f in this.Files)
            {
                rootNode.Nodes.Add(f.ToTreeNode());
            }

            return rootNode;
        }

        #endregion

        #region Filetype Detection

        static public FileType DetectTypeFromExtension(String Extension)
        {
            if (Extension.StartsWith(".")) Extension = Extension.Substring(1);
            switch (Extension.ToLower())
            {
                case "rks":
                case "rs":
                    return FileType.Script;
                case "rkw":
                    return FileType.World;
                case "rka":
                    return FileType.Area;
                case "ogg":
                    return FileType.Audio;
                case "png":
                case "bmp":
                case "jpg":
                case "tga":
                case "pcx":
                case "psd":
                    return FileType.Image;
                case "3ds":
                case "b3d":
                case "csm":
                case "dae":
                case "dmf":
                case "x":
                case "obj":
                case "my3d":
                case "oct":
                case "mesh":
                case "lmts":
                case "bsp":
                case "md2":
                case "md3":
                case "rkm":
                    return FileType.Model;
                case "c":
                case "cc":
                case "cpp":
                    return FileType.CScript;
                case "h":
                case "hpp":
                    return FileType.CHeader;
                default:
                    return FileType.Generic;
            }
        }

        #endregion
    }

    public class Reference
    {
        public String IncludeString = "";
        public Dictionary<String, String> Settings = new Dictionary<String, String>();
    }

    public enum FileType
    {
        Unknown,
        Folder,
        Generic,
        Reference,
        ProjectReference,
        Script,
        Model,
        Image,
        Audio,
        Template,
        Area,
        World,
        CScript,
        CHeader
    }

    class NoSuchConfigurationException : Exception
    {
        public NoSuchConfigurationException() { }
        public NoSuchConfigurationException(string message) { }
        public NoSuchConfigurationException(string message, System.Exception inner) { }

        // Constructor needed for serialization 
        // when exception propagates from a remoting server to the client.
        protected NoSuchConfigurationException(System.Runtime.Serialization.SerializationInfo info,
            System.Runtime.Serialization.StreamingContext context) { }
    }
}
