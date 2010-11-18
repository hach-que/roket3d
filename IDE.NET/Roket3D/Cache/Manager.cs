using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Roket3D.Cache
{
    public class Manager
    {
        private Scintilla p_ScintillaCache = null;
        private DefinedClasses p_DefinedClasses = null;

        /// <summary>
        /// Creates a new cache manager, which manages all of the sub-caches.
        /// </summary>
        public Manager()
        {
            this.p_ScintillaCache = new Scintilla();
            this.p_DefinedClasses = new DefinedClasses();
        }

        /// <summary>
        /// The scintilla caching object.
        /// </summary>
        public Scintilla ScintillaCache
        {
            get
            {
                return this.p_ScintillaCache;
            }
        }

        /// <summary>
        /// The defined classes caching object.
        /// </summary>
        public DefinedClasses DefinedClasses
        {
            get
            {
                return this.p_DefinedClasses;
            }
        }
    }

    public delegate void ProgressCallback(int progress);
}
