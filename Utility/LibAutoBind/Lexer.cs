using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Tokens;
using LibAutoBind.Nodes;

namespace LibAutoBind
{
    class Lexer
    {
        private string p_TextCache = "";
        private char p_Char;

        private Machine m_Machine = null;
        private List<Token> m_Tokens = new List<Token>();
        private List<Token> m_ExcludedTokens = new List<Token>();
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
            this.m_Tokens.Add(new PreprocessorIgnoreToken());
            this.m_Tokens.Add(new UsingToken());
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
                    c = this.ConvertNewline(c);
                }
                else
                {
                    c = (char)this.m_Machine.InputFile.Read();
                    c = this.ConvertNewline(c);
                }
                this.p_Char = c;

                this.p_TextCache += c;

                if (this.m_TokenWithOwnership == null)
                {
                    // If all of the tokens have placed themselves in
                    // the excluded list, we add a new DirectNode (since
                    // we don't understand the content) and clear the
                    // text state.
                    if (this.m_Tokens.Count == this.m_ExcludedTokens.Count)
                    {
                        this.AddNode(new DirectNode(this.Text));
                        this.p_TextCache = "";
                        this.m_ExcludedTokens.Clear();
                        continue;
                    }

                    foreach (Token t in this.m_Tokens)
                    {
                        if (this.m_ExcludedTokens.Contains(t))
                            continue;

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
        /// Ensures that newlines are represented by single \n characters.
        /// </summary>
        private char ConvertNewline(char c)
        {
            if (c == '\r')
            {
                if (this.m_SeekCache.Length > 0)
                {
                    if (this.m_SeekCache[0] == '\n')
                        this.m_SeekCache = this.m_SeekCache.Substring(1);
                    c = '\n';
                }
                else
                {
                    if ((char)this.m_Machine.InputFile.Peek() == '\n')
                        c = (char)this.m_Machine.InputFile.Read();
                    else
                        c = '\n';
                }
            }

            return c;
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
            get { return this.p_TextCache.TrimStart(' '); }
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
            string res = "";
            for (int i = 0; i < str.Length; i += 1)
            {
                if (this.m_SeekCache.Length > i)
                    res += this.m_SeekCache[i];
                else
                {
                    char c = (char)this.m_Machine.InputFile.Read();
                    c = ConvertNewline(c);
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
            Console.WriteLine(node.GetType().ToString() + ": " + node.Content);
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
                this.m_ExcludedTokens.Clear();
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

        /// <summary>
        /// Forces the lexer to not include the current token in any further text
        /// state testing (i.e. Run() will not be called) until another Token
        /// successfully handles the current state.
        /// </summary>
        public void ForceExclude()
        {
            if (!this.m_ExcludedTokens.Contains(this.m_CurrentToken))
                this.m_ExcludedTokens.Add(this.m_CurrentToken);
        }
    }
}
