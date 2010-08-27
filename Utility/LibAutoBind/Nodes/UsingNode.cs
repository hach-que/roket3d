using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class UsingNode : Node
    {
        public UsingNode(string content)
        {
            this.p_Content = content;
        }
    }
}
