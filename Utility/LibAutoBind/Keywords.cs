using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    class Keywords
    {
        // Keywords specific to C++, they are kept untouched.
        internal static string[] CPPVisibilityKeywords = { "private", "protected", "public", "friend" };
        internal static string[] CPPTypeKeywords = { "char", "bool", "short", "int", "long", "longlong", "float", "double" };

        // Keywords specific to the automatic binding system.  These are read
        // by the tokens and removed in the final source code.
        internal static string[] LuaVisibilityKeywords = { "bound", "unbound" };
        internal static string[] LuaTypeKeywords = { "string", "numeric", "table" };

        /// <summary>
        /// Detects keywords in the specified string and returns a list of them.  Stops
        /// scanning when the first non-keyword is read.
        /// </summary>
        /// <param name="text">The text to scan.</param>
        /// <returns>A list of keywords.</returns>
        internal static List<string> GetKeywords(string text)
        {
            string buf = "";
            List<string> ret = new List<string>();
            for (int i = 0; i < text.Length; i++)
            {
                if (text[i] == " ")
                {
                    // End of building up the buffer.
                    if (Keywords.GetKeyword(buf) != "")
                    {
                        ret.Add(buf);
                        buf = "";
                    }
                    else
                    {
                        // No more keywords.
                        return ret;
                    }
                }
                else
                    buf += text[i];
            }
            return ret;
        }

        /// <summary>
        /// Returns the keyword located in the text.  If the text is an exact match
        /// to one of the keywords specified in the static arrays, it is returned.
        /// Otherwise, a blank string is returned.
        /// </summary>
        /// <param name="text">The text to scan.</param>
        /// <returns>A string containing the keyword, or "".</returns>
        internal static string GetKeyword(string text)
        {
            foreach (string s in Keywords.CPPVisibilityKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.CPPTypeKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.LuaVisibilityKeywords)
                if (s == text) return s;
            foreach (string s in Keywords.LuaTypeKeywords)
                if (s == text) return s;
            return "";
        }
    }
}
