using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind;
using System.IO;

namespace AutoBind
{
    class Program
    {
        static int Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("usage: autobind CPPBASE HBASE INPUT");
                return 1;
            }

            string cppbase = args[0].Trim();
            string hbase = args[1].Trim();
            string input = args[2].Trim();
            int start = input.LastIndexOf('\\') + 1;
            string basename = input.Substring(start);
            int end = basename.IndexOf('.');
            basename = basename.Substring(0, end);

            if (!Directory.Exists(hbase))
                Directory.CreateDirectory(hbase);
            if (!Directory.Exists(cppbase))
                Directory.CreateDirectory(cppbase);
            Machine m = new Machine(input, hbase + '/' + basename + ".h", cppbase + '/' + basename + ".cpp");
            try
            {
                m.Run();
            }
            catch (LibAutoBind.InvalidClassDefinitionException e)
            {
                Console.WriteLine("Unable to automatically bind " + input + ".  Make sure there");
                Console.WriteLine("is only one class defined in the file.");
                return 1;
            }
            catch (LibAutoBind.InvalidCharacterException e)
            {
                return 1;
            }
            catch (LibAutoBind.LexingAbortedException e)
            {
                return 1;
            }
            m.Close();

            return 0;
        }
    }
}
