using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class ClassVariableDeclarationNode : Node
    {
        private List<string> m_Keywords;
        private string m_Name;

        internal ClassVariableDeclarationNode(List<string> keywords, string name)
        {
            this.m_Keywords = keywords;
            this.m_Name = name;

            string keys = "";
            foreach (string s in keywords)
                keys += s + " ";
            keys = keys.Trim();
            this.p_Content = keys + " " + name;
        }

        internal List<string> AllKeywords
        {
            get { return this.m_Keywords; }
        }

        internal List<string> CPPKeywords
        {
            get
            {
                List<string> ret = new List<string>();
                foreach (string k in this.m_Keywords)
                {
                    if (!Keywords.LuaDeclKeywords.Contains(k) && !Keywords.LuaVisibilityKeywords.Contains(k))
                    {
                        ret.Add(k);
                    }
                    else if (k == "bound")
                        ret.Add("public");
                }
                return ret;
            }
        }

        internal string Name
        {
            get { return this.m_Name; }
        }
    }
}
