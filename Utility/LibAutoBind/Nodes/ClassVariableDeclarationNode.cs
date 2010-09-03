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
        private string m_Type;

        internal ClassVariableDeclarationNode(List<string> keywords, string type, string name)
        {
            this.m_Keywords = keywords;
            this.m_Name = name;
            this.m_Type = type;

            string keys = "";
            foreach (string s in keywords)
                keys += s + " ";
            if (type == "")
            {
                // Must a built-in type.
                foreach (string s in keywords)
                    if (Keywords.CPPTypeKeywords.Contains(s) && Keywords.LuaTypeKeywords.Contains(s))
                        this.m_Type = s;
            }
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

        internal string Type
        {
            get { return this.m_Type; }
        }
    }
}
