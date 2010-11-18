﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DockPanelSuite;

namespace Roket3D.Tools
{
    public class Tool : DockContent
	{
        public virtual DockState DefaultState
        {
            get
            {
                return DockState.Document;
            }
        }

        public virtual void OnSolutionLoaded()
        {
        }

        public virtual void OnSolutionUnloaded()
        {
        }
	}
}
