using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;

namespace LibAutoBind.Tokens
{
    class PreprocessorIgnoreToken : Token
    {
        private bool m_KeepOwnership = false;

        public PreprocessorIgnoreToken() { }

        public override void Run(Lexer l)
        {
            if (l.Text.StartsWith("#") && (l.MatchNext(" ") || l.MatchNext("\n")) && l.Text != "#import")
                l.TakeOwnership();
            else if (!l.Text.StartsWith("#") || l.Text == "#import")
                l.ForceExclude();

            if (l.HasOwnership())
            {
                if (l.Char == '\\' && l.MatchNext("\n"))
                {
                    this.m_KeepOwnership = true;
                }
                else if (l.Char == '\n' && !this.m_KeepOwnership)
                {
                    l.AddNode(new DirectNode(l.Text));
                    l.EndOwnership();
                }
                else if (this.m_KeepOwnership)
                    this.m_KeepOwnership = false;
            }
            
        }
    }
}
