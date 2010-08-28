using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;

namespace LibAutoBind.Transformers
{
    class LuaTransformer : Transformer
    {
        public LuaTransformer(Machine m)
        {
            this.m_M = m;
        }

        public override void WriteHeaderFile(List<Node> nodes)
        {
            foreach (Node n in nodes)
            {
                if (n is ImportNode)
                {
                    this.m_M.OutputHFile.WriteLine(n.Content);
                }
                else if (n is UsingNode)
                {
                    this.m_M.OutputHFile.WriteLine(n.Content);
                }
                else if (n is GenericBracketNode)
                {
                    this.m_M.OutputHFile.WriteLine(n.Content);
                }
                else if (n is ClassVariableDeclarationNode)
                {
                    this.m_M.OutputHFile.WriteLine(n.Content + ";");
                }
                else if (n is ClassFunctionDeclarationNode)
                {
                    this.m_M.OutputHFile.WriteLine(n.Content);
                }
                else if (n is ClassPropertyDeclarationNode)
                {
                    this.m_M.OutputHFile.WriteLine(n.Content);
                }
                else if (n is ClassDefinitionNode)
                {
                    this.m_M.OutputHFile.WriteLine(n.Content);
                }
                else if (n is DirectNode)
                {
                    this.m_M.OutputHFile.WriteLine(n.Content);
                }
            }
        }

        public override void WriteCodeFile(List<Node> nodes)
        {
        }
    }
}
