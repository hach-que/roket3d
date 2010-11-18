using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Roket3D.Management
{
    public class Folder : File
    {
        private DirectoryInfo p_FolderInfo = null;
        private List<File> p_Files = null;

        /// <summary>
        /// Creates a new Folder object for representing a folder in the project.
        /// </summary>
        /// <param name="p">The project that owns this folder.</param>
        public Folder(Project p)
            : base(p)
        {
            this.p_Files = new List<File>();
        }

        /// <summary>
        /// The DirectoryInfo object that represents this folder on-disk.
        /// </summary>
        public DirectoryInfo FolderInfo
        {
            get
            {
                return this.p_FolderInfo;
            }
        }
    }
}
