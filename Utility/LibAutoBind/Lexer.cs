using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Tokens;

namespace LibAutoBind
{
    class Lexer
    {
        private string p_TextCache = "";
        private char p_Char;

        private Machine m_Machine = null;
        private List<Token> m_Tokens = new List<Token>();
        private Token m_TokenWithOwnership = null;
        private Token m_CurrentToken = null;
        private List<Node> m_LexerList = new List<Node>();
        private string m_SeekCache = "";
        private bool m_ShouldResetText = false;

        public Lexer(Machine m)
        {
            this.m_Machine = m;

            // Add all the tokens we should recognise.
            this.m_Tokens.Add(new PreprocessorImportToken());
        }

        /// <summary>
        /// Runs the lexer on the machine's input file.
        /// </summary>
        public void Run()
        {
            while (!this.m_Machine.InputFile.EndOfStream || this.m_SeekCache.Length > 0)
            {
                // Retrieve a character from the seek cache (used because
                // sometimes we need to read ahead multiple characters and
                // we can only peek the next character) or alternatively
                // the file if the seek cache is empty.
                char c;
                if (this.m_SeekCache.Length > 0)
                {
                    c = this.m_SeekCache[0];
                    this.m_SeekCache = this.m_SeekCache.Substring(1);
                }
                else
                    c = (char)this.m_Machine.InputFile.Read();
                this.p_Char = c;

                this.p_TextCache += c;

                if (this.m_TokenWithOwnership == null)
                {
                    foreach (Token t in this.m_Tokens)
                    {
                        this.m_CurrentToken = t;
                        t.Run(this);
                        if (this.m_ShouldResetText)
                        {
                            this.p_TextCache = "";
                            this.m_ShouldResetText = false;
                            break; // Skip the rest of the tokens.
                        }
                    }
                }
                else
                {
                    this.m_CurrentToken = this.m_TokenWithOwnership;
                    this.m_TokenWithOwnership.Run(this);
                    if (this.m_ShouldResetText)
                    {
                        this.p_TextCache = "";
                        this.m_ShouldResetText = false;
                    }
                }

                this.m_CurrentToken = null;
            }
        }

        /// <summary>
        /// Check the text before the current text state to see if
        /// it matches the specified string argument.
        /// </summary>
        public bool MatchPrev(string str)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// The current block of text that is being evaluated.  The characters
        /// are continually added to the text until a Token takes ownership and
        /// handles the block of text.
        /// </summary>
        public string Text
        {
            get { return this.p_TextCache; }
        }

        /// <summary>
        /// The current character that was last read.
        /// </summary>
        public char Char
        {
            get { return this.p_Char; }
        }

        /// <summary>
        /// Check the text after the current text state (peek ahead) to see if
        /// it matches the specified string argument.  Useful to check whether
        /// or not there is whitespace (i.e. end of a token) before taking
        /// ownership of it.
        /// </summary>
        public bool MatchNext(string str)
        {
            if (str.Length == 1)
            {
                // No need to deal with the seek cache, we can just use Peek().
                return (str[0] == (char)this.m_Machine.InputFile.Peek());
            }

            string res = "";
            for (int i = 0; i < str.Length; i += 1)
            {
                if (this.m_SeekCache.Length > i)
                    res += this.m_SeekCache[i];
                else
                {
                    char c = (char)this.m_Machine.InputFile.Read();
                    res += c;
                    this.m_SeekCache += c;
                }
            }

            return (str == res);
        }

        /// <summary>
        /// Take ownership of the current text state.  After a Token calls this
        /// function, it is the only Token that will recieve Run() calls.  This
        /// ensures that other tokens don't add nodes when they are not meant to.
        /// </summary>
        public void TakeOwnership()
        {
            this.m_TokenWithOwnership = this.m_CurrentToken;
        }

        /// <summary>
        /// Adds a node to the lexer list.  Nodes are then passed to the Transformer
        /// once lexing is complete.
        /// </summary>
        public void AddNode(Node node)
        {
            this.m_LexerList.Add(node);
        }

        /// <summary>
        /// Ends ownership of the current text state.  Once this has been called by
        /// a Token that has previously taken ownership, the lexer clears the text
        /// state and moves onto reading the next set of characters.
        /// </summary>
        public void EndOwnership()
        {
            if (this.m_TokenWithOwnership == this.m_CurrentToken)
            {
                this.m_TokenWithOwnership = null;
                this.p_TextCache = "";
            }
        }

        /// <summary>
        /// Returns whether or not the Token currently has ownership of the text
        /// state.
        /// </summary>
        public bool HasOwnership()
        {
            return (this.m_TokenWithOwnership == this.m_CurrentToken);
        }
    }
}
