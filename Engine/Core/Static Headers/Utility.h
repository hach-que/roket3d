// Utility functions used by the engine, such as checking
// whether files exist.

#ifndef STATIC_Utility
#define STATIC_Utility

#include <string>

namespace Roket3D
{
	class Utility
	{
		public:
			static bool FileExists(std::string path);
	};
}

#endif