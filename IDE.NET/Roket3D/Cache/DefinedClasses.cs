using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;

namespace Roket3D.Cache
{
    public class DefinedClasses
    {
        private List<string> m_BaseClasses = new List<string> {
                                                 "Engine.Collections.Color",
                                                 "Engine.Collections.Cube3D",
                                                 "Engine.Collections.Plane",
                                                 "Engine.Collections.Quaternion",
                                                 "Engine.Collections.Line2D",
                                                 "Engine.Collections.Line3D ",
                                                 "Engine.Collections.Rectangle",
                                                 "Engine.Collections.Triangle3D",
                                                 "Engine.Collections.Vector2D",
                                                 "Engine.Collections.Vector3D",
                                                 "Engine.Unmanaged.Audio.Controller",
                                                 "Engine.Unmanaged.Audio.Sound",
                                                 "Engine.Unmanaged.Core.CursorControl",
                                                 "Engine.Unmanaged.Core.Device",
                                                 "Engine.Unmanaged.Core.Event",
                                                 "Engine.Unmanaged.GUI.Button",
                                                 "Engine.Unmanaged.GUI.CheckBox",
                                                 "Engine.Unmanaged.GUI.ComboBox",
                                                 "Engine.Unmanaged.GUI.EditBox",
                                                 "Engine.Unmanaged.GUI.Element",
                                                 "Engine.Unmanaged.GUI.Environment",
                                                 "Engine.Unmanaged.GUI.FileOpenDialog",
                                                 "Engine.Unmanaged.GUI.Font",
                                                 "Engine.Unmanaged.GUI.Image",
                                                 "Engine.Unmanaged.GUI.ListBox",
                                                 "Engine.Unmanaged.GUI.MeshViewer",
                                                 "Engine.Unmanaged.GUI.ScrollBar",
                                                 "Engine.Unmanaged.GUI.Skin",
                                                 "Engine.Unmanaged.GUI.StaticText",
                                                 "Engine.Unmanaged.GUI.Window",
                                                 "Engine.Unmanaged.Net.Socket",
                                                 "Engine.Unmanaged.Physics.Joint",
                                                 "Engine.Unmanaged.Physics.Manager",
                                                 "Engine.Unmanaged.Physics.MaterialHandler",
                                                 "Engine.Unmanaged.Scene.ParticleSystemNode",
                                                 "Engine.Unmanaged.Scene.BillboardNode",
                                                 "Engine.Unmanaged.Scene.BillboardTextNode",
                                                 "Engine.Unmanaged.Scene.Camera",
                                                 "Engine.Unmanaged.Scene.CollisionManager",
                                                 "Engine.Unmanaged.Scene.LazyCamera",
                                                 "Engine.Unmanaged.Scene.LightNode",
                                                 "Engine.Unmanaged.Scene.Manager",
                                                 "Engine.Unmanaged.Scene.Mesh",
                                                 "Engine.Unmanaged.Scene.MeshNode",
                                                 "Engine.Unmanaged.Scene.SpecialEffects",
                                                 "Engine.Unmanaged.Scene.TerrainNode",
                                                 "Engine.Unmanaged.Video.Driver",
                                                 "Engine.Unmanaged.Video.Fog",
                                                 "Engine.Unmanaged.Video.Light",
                                                 "Engine.Unmanaged.Video.Material",
                                                 "Engine.Unmanaged.Video.Texture"
                                             };
        private List<string> m_UserClasses = new List<string>();

        /// <summary>
        /// A list of base classes defined by the engine itself.
        /// </summary>
        public ReadOnlyCollection<string> BaseClasses
        {
            get
            {
                return this.m_BaseClasses.AsReadOnly();
            }
        }

        /// <summary>
        /// A list of user-defined classes in the current solution.
        /// </summary>
        public List<string> UserClasses
        {
            get
            {
                return this.m_UserClasses;
            }
        }

        /// <summary>
        /// A combined list of the base classes and the user classes.
        /// </summary>
        public ReadOnlyCollection<string> AllClasses
        {
            get
            {
                List<string> list = new List<string>(this.m_BaseClasses);
                list.AddRange(this.m_UserClasses);
                return list.AsReadOnly();
            }
        }
    }
}
