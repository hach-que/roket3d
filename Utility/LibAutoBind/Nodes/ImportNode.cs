using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class ImportNode : Node
    {
        public ImportNode(string content)
        {
            this.p_Content = content;
        }
    }
}
