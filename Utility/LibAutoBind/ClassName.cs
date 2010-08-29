using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind
{
    class ClassName
    {
        internal static string ResolveToHeaderFilename(string cls)
        {
            string path = cls.Replace('.', '/') + ".h";
            if (path.StartsWith("Engine/"))
                path = "E." + path.Substring("Engine/".Length);
            return path;
        }
    }
}
