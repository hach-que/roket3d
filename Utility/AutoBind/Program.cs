using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibAutoBind;

namespace AutoBind
{
	class Program
	{
		static void Main(string[] args)
		{
            if (args.Length != 3)
            {
                Console.WriteLine("usage: autobind INPUT OUTPUT_H OUTPUT_C");
                return;
            }

            Machine m = new Machine(args[0], args[1], args[2]);
            m.Run();
            m.Close();
		}
	}
}
