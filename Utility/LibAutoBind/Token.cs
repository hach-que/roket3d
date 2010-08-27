using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    abstract class Token
    {
        abstract public void Run(Lexer l);
    }
}
