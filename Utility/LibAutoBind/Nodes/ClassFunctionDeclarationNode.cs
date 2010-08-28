using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class ClassFunctionDeclarationNode : Node
    {
        private List<string> m_Keywords;
        private string m_Name;

        internal ClassFunctionDeclarationNode(List<string> keywords, string name)
        {
            this.m_Keywords = keywords;
            this.m_Name = name;

            string keys = "";
            foreach (string s in keywords)
                keys += s + " ";
            keys = keys.Trim();
            this.p_Content = keys + " " + name;
        }
    }
}
