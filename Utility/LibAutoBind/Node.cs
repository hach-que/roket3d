using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    class Node
    {
        protected string p_Content;
        protected int p_ParentCount;

        internal virtual string Content
        {
            get { return this.p_Content; }
        }

        internal virtual int ParentCount
        {
            get { return this.p_ParentCount; }
            set { this.p_ParentCount = value; }
        }
    }
}
