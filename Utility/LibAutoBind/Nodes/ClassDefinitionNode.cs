using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class ClassDefinitionNode : Node
    {
        private string p_Class;
        private string p_ClassOnly;
        private string p_Alias;

        internal ClassDefinitionNode(string cls, string alias)
        {
            this.p_Class = cls.Trim();
            if (this.p_Class.LastIndexOf('.') != -1)
                this.p_ClassOnly = this.p_Class.Substring(this.p_Class.LastIndexOf('.') + 1);
            else
                this.p_ClassOnly = this.p_Class;
            this.p_Alias = alias.Trim();
        }

        internal override string Content
        {
            get { return this.p_Class + " AS " + this.p_Alias; }
        }

        internal string Class
        {
            get { return this.p_Class; }
        }

        internal string ClassOnly
        {
            get { return this.p_ClassOnly; }
        }

        internal string Alias
        {
            get { return this.p_Alias; }
        }
    }
}
