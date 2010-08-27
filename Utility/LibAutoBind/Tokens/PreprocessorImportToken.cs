using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;

namespace LibAutoBind.Tokens
{
    class PreprocessorImportToken : Token
    {
        private bool m_KeepOwnership = false;

        public PreprocessorImportToken() { }

        public override void Run(Lexer l)
        {
            if (l.Text == "#import")
                l.TakeOwnership();

            if (l.HasOwnership())
            {
                if (l.Char == '\\' && l.MatchNext("\n"))
                {
                    this.m_KeepOwnership = true;
                }
                else if (l.Char == '\n' && !this.m_KeepOwnership)
                {
                    l.AddNode(new ImportNode(l.Text));
                    l.EndOwnership();
                }
                else if (this.m_KeepOwnership)
                    this.m_KeepOwnership = false;
            }
        }
    }
}
