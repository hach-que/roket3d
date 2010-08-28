using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    class Node
    {
        protected string p_Content;

        internal virtual string Content
        {
            get { return this.p_Content; }
        }
    }
}
