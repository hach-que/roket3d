using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind;

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

            string cppbase = args[0];
            string hbase = args[1];
            string input = args[2];
            string basename = input.Substring(input.LastIndexOf('/') + 1, input.Length - (input.LastIndexOf('/') + 1) - input.IndexOf('.'));

            Machine m = new Machine(input, hbase + '/' + basename + ".h", cppbase + '/' + basename + ".cpp");
            m.Run();
            m.Close();

            return 0;
        }
    }
}
