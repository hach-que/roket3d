﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind.Nodes;
using System.Text.RegularExpressions;

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
            this.ValidityCheck(nodes);

            this.WriteHeaderLine(@"/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */");
            this.WriteHeaderLine();
            
            // Get the class node.
            ClassDefinitionNode cls = (ClassDefinitionNode)(this.GetNodesOfType(nodes, typeof(ClassDefinitionNode))[0]);
            this.WriteHeaderLine("#ifndef CLASS_" + cls.Class.Replace('.', '_'));
            this.WriteHeaderLine("#define CLASS_" + cls.Class.Replace('.', '_'));
            this.WriteHeaderLine();

            // Get all of the import nodes and convert them to
            // #includes.  We also predeclare any of the classes
            // we are importing since it's not feasable to detect
            // what classes reference what other classes.
            List<Node> imports = this.GetNodesOfType(nodes, typeof(ImportNode));
            this.WriteHeaderLine("/* Imports */");
            foreach (Node n in imports)
            {
                string[] components = n.Content.Split('.');
                for (int i = 0; i < components.Length; i++)
                {
                    if (i != components.Length - 1)
                        this.WriteHeader("namespace " + components[i] + " { ");
                    else
                        this.WriteHeader("class " + components[i] + "; ");
                }
                for (int i = 0; i < components.Length- 1; i++)
                {
                    this.WriteHeader("} ");
                }
                this.WriteHeaderLine();
            }
            foreach (Node n in imports)
            {
                this.WriteHeaderLine("#include \"" + ClassName.ResolveToHeaderFilename(n.Content.Trim()) + "\"");
            }
            this.WriteHeaderLine();

            // Now add all of the using declarations.
            this.WriteHeaderLine("/* Using declarations */");
            foreach (Node n in this.GetNodesOfType(nodes, typeof(UsingNode)))
            {
                this.WriteHeaderLine("using " + n.Content.Replace(".", "::") + ";");
            }
            this.WriteHeaderLine();

            // Begin declaring the class.
            string[] clscomponents = cls.Class.Split('.');
            for (int i = 0; i < clscomponents.Length; i++)
            {
                if (i < clscomponents.Length - 2)
                    this.WriteHeader("namespace " + clscomponents[i] + " { ");
                else if (i == clscomponents.Length - 2)
                    this.WriteHeader("namespace " + clscomponents[i]);
                else
                {
                    this.WriteHeaderLine();
                    this.WriteHeaderLine("{");
                    this.WriteHeaderLine("    /* Begin class declaration */");
                    this.WriteHeaderLine("    class " + clscomponents[i] + " : public RObject");
                    this.WriteHeaderLine("    {");
                }
            }
            
            // Declare all of the variables within the class.
            this.WriteHeaderLine("        /* Variable declarations */");
            foreach (ClassVariableDeclarationNode n in this.GetNodesOfType(nodes, typeof(ClassVariableDeclarationNode)))
            {
                string keys = "";
                foreach (string k in n.CPPKeywords)
                    keys += k + " ";
                this.WriteHeaderLine("        " + keys + n.Name + ";");
            }
            this.WriteHeaderLine();

            // Declare the methods that are in the class.
            this.WriteHeaderLine("        /* Method and constructor declarations */");

            List<string> fdecls = new List<string>();
            foreach (ClassFunctionDeclarationNode n in this.GetNodesOfType(nodes, typeof(ClassFunctionDeclarationNode)))
            {
                string keys = "";
                foreach (string k in n.CPPKeywords)
                {
                    if (!Keywords.CPPTypeKeywords.Contains(k) && !Keywords.LuaTypeKeywords.Contains(k))
                        keys += k + " ";
                }
                string decl = "";
                if (n.Name == clscomponents[clscomponents.Length - 1]) // Constructor
                    decl = keys + n.Name + "(lua_State * L, bool byuser);";
                else
                    decl = keys + "int " + n.Name + "(lua_State * L);";
                if (!fdecls.Contains(decl))
                    this.WriteHeaderLine("        " + decl);
                fdecls.Add(decl);
            }
            this.WriteHeaderLine();

            // Declare automatically generated methods for setting / getting properties.
            this.WriteHeaderLine("        /* Automatic property getter-setter declarations */");
            foreach (ClassPropertyDeclarationNode n in this.GetNodesOfType(nodes, typeof(ClassPropertyDeclarationNode)))
            {
                if (n.GetVal != null && !n.GetIsFunc)
                {
                    this.WriteHeaderLine("        int __autobind_property_get_" + n.Name + "(lua_State * L);");
                }
                if (n.SetVal != null && !n.SetIsFunc)
                {
                    this.WriteHeaderLine("        int __autobind_property_set_" + n.Name + "(lua_State * L);");
                }
            }

            // End the declaration of the class.
            this.WriteHeaderLine("    };");
            for (int i = 0; i < clscomponents.Length - 1; i++)
            {
                this.WriteHeaderLine("} ");
            }
            this.WriteHeaderLine();

            // Add the #endif.
            this.WriteHeaderLine("#endif");
        }

        public override void WriteCodeFile(List<Node> nodes)
        {
            this.ValidityCheck(nodes);

            this.WriteCodeLine(@"/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */");
            this.WriteCodeLine();

            // Get the class node.
            ClassDefinitionNode cls = (ClassDefinitionNode)(this.GetNodesOfType(nodes, typeof(ClassDefinitionNode))[0]);
            this.WriteCodeLine("#include \"autobind/types.h\"");
            this.WriteCodeLine("#include \"autobind/binding/lua.h\"");
            this.WriteCodeLine("#include \"" + ClassName.ResolveToHeaderFilename(cls.Class) + "\"");
            this.WriteCodeLine();

            // All of the import declarations will have been made in the header file,
            // so they aren't needed here.

            // All of the using declarations will have been made in the header file,
            // so they aren't needed here.

            // Begin defining the class.
            string[] clscomponents = cls.Class.Split('.');
            for (int i = 0; i < clscomponents.Length; i++)
            {
                if (i < clscomponents.Length - 2)
                    this.WriteCode("namespace " + clscomponents[i] + " { ");
                else if (i == clscomponents.Length - 2)
                    this.WriteCode("namespace " + clscomponents[i]);
                else
                {
                    this.WriteCodeLine();
                    this.WriteCodeLine("{");
                }
            }

            // Declare all of the variables within the class.
            this.WriteCodeLine("    /* Variable definitions */");
            foreach (ClassVariableDeclarationNode n in this.GetNodesOfType(nodes, typeof(ClassVariableDeclarationNode)))
            {
                string keys = "";
                foreach (string k in n.CPPKeywords)
                    keys += k + " ";
                this.WriteCodeLine("    " + keys + cls.ClassOnly + "::" + n.Name + ";");
            }
            this.WriteCodeLine();

            // Declare the methods that are in the class.
            this.WriteCodeLine("    /* Method and constructor definitions */");

            List<string> fdefs = new List<string>();
            bool funccontent = false;
            string functype = "";
            foreach (Node r in nodes)
            {
                if (r is ClassFunctionDeclarationNode)
                {
                    ClassFunctionDeclarationNode n = (ClassFunctionDeclarationNode)r;
                    string keys = "";
                    foreach (string k in n.CPPKeywords)
                    {
                        if (!Keywords.CPPTypeKeywords.Contains(k) && !Keywords.LuaTypeKeywords.Contains(k))
                            keys += k + " ";
                        if (Keywords.CPPTypeKeywords.Contains(k) || Keywords.LuaTypeKeywords.Contains(k))
                            functype = k;
                    }
                    string def = "";
                    if (n.Name == clscomponents[clscomponents.Length - 1]) // Constructor
                        def = keys + cls.ClassOnly + "::" + n.Name + "(lua_State * L, bool byuser)";
                    else
                        def = keys + "int " + cls.ClassOnly + "::" + n.Name + "(lua_State * L)";
                    if (!fdefs.Contains(def))
                    {
                        this.WriteCodeLine("    " + def);
                        this.WriteCodeLine("    {");
                        
                        // Add the argument bindings.
                        int ai = 0;
                        foreach (string a in n.Arguments)
                        {
                            Regex ar = new Regex("(?<Type>[a-zA-Z0-9_\\.]+)[ \r\n\t]+(?<Name>[a-zA-Z0-9_\\.]+)");
                            Match m = ar.Match(a);
                            if (m.Success)
                            {
                                string type = m.Groups["Type"].Value;
                                string name = m.Groups["Name"].Value;

                                this.WriteCodeLine("        Bindings<" + type + "> " + name + "(L, " + ai + ");");
                            }
                            ai += 1;
                        }
                        if (ai != 0) this.WriteCodeLine();
                        funccontent = true;
                    }
                    fdefs.Add(def);
                }
                else if (r is DirectNode && funccontent)
                {
                    // Search the content for any return statements, and use a Regex to
                    // replace them with the appropriate bindings.
                    Regex rr = new Regex("^([ \r\n\t]*)([^\\/][^\\/])([^/]*)([ \r\n\t]+)return[ \r\n\t]+(?<Val>[^\\;]+)\\;", RegexOptions.Multiline);
                    string res = rr.Replace(r.Content, "${1}${2}${3}${4}return Bindings<" + functype + ">::Result(${Val});");
                    Regex brr = new Regex("([ \r\n\t]*)([^\\/][^\\/])([^/]*)([ \r\n\t]+)return[ \r\n\t]*\\;", RegexOptions.Multiline);
                    res = brr.Replace(res, "${1}${2}${3}${4}return Bindings<" + functype + ">::EmptyResult;");
                    this.WriteCodeLine("        " + res.TrimStart());
                    this.WriteCodeLine();
                    funccontent = false;
                }
            }

            // Declare automatically generated methods for setting / getting properties.
            this.WriteCodeLine("    /* Automatic property getter-setter definitions */");
            foreach (ClassPropertyDeclarationNode n in this.GetNodesOfType(nodes, typeof(ClassPropertyDeclarationNode)))
            {
                if (n.GetVal != null && !n.GetIsFunc)
                {
                    this.WriteCodeLine("    int " + cls.ClassOnly + "::__autobind_property_get_" + n.Name + "(lua_State * L)");
                    this.WriteCodeLine("    {");
                    this.WriteCodeLine("        return Bindings<numeric>::Result(" + n.GetVal + ");");
                    this.WriteCodeLine("    }");
                    this.WriteCodeLine();
                }
                if (n.SetVal != null && !n.SetIsFunc)
                {
                    this.WriteCodeLine("    int " + cls.ClassOnly + "::__autobind_property_set_" + n.Name + "(lua_State * L)");
                    this.WriteCodeLine("    {");
                    this.WriteCodeLine("        " + n.SetVal + " = Bindings<numeric>::GetArgument(L, -1);");
                    this.WriteCodeLine("        return Bindings<numeric>::EmptyResult;");
                    this.WriteCodeLine("    }");
                    this.WriteCodeLine();
                }
            }

            // End the definition of the class.
            for (int i = 0; i < clscomponents.Length - 1; i++)
            {
                this.WriteCodeLine("} ");
            }
            this.WriteCodeLine();
        }

        private void WriteHeader(string str)
        {
            this.m_M.OutputHFile.Write(str.Replace("\r\n", "\n"));
        }

        private void WriteHeaderLine(string str)
        {
            this.m_M.OutputHFile.WriteLine(str.Replace("\r\n", "\n"));
        }

        private void WriteHeaderLine()
        {
            this.m_M.OutputHFile.WriteLine();
        }

        private void WriteCode(string str)
        {
            this.m_M.OutputCFile.Write(str.Replace("\r\n", "\n"));
        }

        private void WriteCodeLine(string str)
        {
            this.m_M.OutputCFile.WriteLine(str.Replace("\r\n", "\n"));
        }

        private void WriteCodeLine()
        {
            this.m_M.OutputCFile.WriteLine();
        }

        private void ValidityCheck(List<Node> nodes)
        {
            int classCount = 0;
            foreach (Node n in nodes)
            {
                if (n is ClassDefinitionNode)
                    classCount += 1;
            }

            if (classCount != 1)
                throw new InvalidOperationException("There must only be one class defined within a AutoBind source file.");
        }

        private List<Node> GetNodesOfType(List<Node> nodes, Type t)
        {
            List<Node> ret = new List<Node>();
            foreach (Node n in nodes)
            {
                if (n.GetType() == t)
                    ret.Add(n);
            }
            return ret;
        }
    }
}
