﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;
using System.Text.RegularExpressions;

namespace LibAutoBind.Tokens
{
    class ClassVariableDeclarationToken : Token
    {
        internal ClassVariableDeclarationToken() { }

        internal override void Detect(Lexer l)
        {
            if (!(l.Char == ' ' || (l.Char >= '0' && l.Char <= '9') || (l.Char >= 'a' && l.Char <= 'z') ||
                (l.Char >= 'A' && l.Char <= 'Z') || l.Char == '\n' || l.Char == '\t' || l.Char == '\r' ||
                l.Char == ';' || l.Char == '_'))
                l.ForceExclude(); // contains characters we can't accept.
            if (!(l.GetParent() is ClassDefinitionToken))
                l.ForceExclude(); // not within a class.

            KeywordResult res = Keywords.GetKeywords(l.Text);

            if (res.Keywords.Count > 0 && res.PossibleKeyword == false)
            {
                // Now use regular expression checking against the substring starting
                // at res.DeclIndex to determine whether it's a variable declaration.

                string decl = l.Text.Substring(res.DeclIndex).Trim();
                if (!decl.EndsWith(";") && !decl.EndsWith("{")) return; // Skip if we don't have a terminating character.
                Regex r = new Regex("[a-zA-Z][a-zA-z0-9_]*[ \t\r\n]*;");
                Match m = r.Match(decl);
                if (m.Success)
                {
                    // It's a variable declaration.
                    l.TakeOwnership();
                    l.AddNode(new ClassVariableDeclarationNode(res.Keywords, m.Value.Substring(0, m.Value.Length - 1).Trim()));
                    l.EndOwnership();
                }
                else
                {
                    // A different kind of declaration that we aren't handling...
                    l.ForceExclude();
                }
            }
        }
    }
}
