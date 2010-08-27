using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace LibAutoBind
{
    public class Machine
    {
        private string p_FilePath = "";
        private StreamReader p_InputFile = null;
        private StreamWriter p_OutputHFile = null;
        private StreamWriter p_OutputCFile = null;
        private Lexer p_Lexer = null;
        private Transformer p_Transformer = null;

        public Machine(string inpath, string outhpath, string outcpath)
        {
            this.p_InputFile = new StreamReader(inpath);
            this.p_OutputHFile = new StreamWriter(outhpath);
            this.p_OutputCFile = new StreamWriter(outcpath);
            this.p_FilePath = inpath;

            this.p_Lexer = new Lexer(this);
            this.p_Transformer = new Transformer(this);
        }

        /// <summary>
        /// Run the Lexer and output data using the Transformer.
        /// </summary>
        public void Run()
        {
            this.p_Lexer.Run();
        }

        /// <summary>
        /// The input file.
        /// </summary>
        public StreamReader InputFile
        {
            get { return this.p_InputFile; }
        }

        /// <summary>
        /// The output header (.h) file.
        /// </summary>
        public StreamWriter OutputHFile
        {
            get { return this.p_OutputHFile; }
        }

        /// <summary>
        /// The output code (.cpp) file.
        /// </summary>
        public StreamWriter OutputCFile
        {
            get { return this.p_OutputCFile; }
        }
    }
}
