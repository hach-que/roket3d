using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class DirectNode : Node
    {
        public DirectNode(string content)
        {
            this.p_Content = content;
        }
    }
}
