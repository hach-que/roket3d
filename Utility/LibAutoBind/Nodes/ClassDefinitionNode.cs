using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class ClassDefinitionNode : Node
    {
        private string p_Class;
        private string p_Alias;

        internal ClassDefinitionNode(string cls, string alias)
        {
            this.p_Class = cls.Trim();
            this.p_Alias = alias.Trim();
        }

        internal override string Content
        {
            get { return this.p_Class + " AS " + this.p_Alias; }
        }
    }
}
