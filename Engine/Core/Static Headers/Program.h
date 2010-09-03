// The main entry point of the Roket3D engine.

#ifndef STATIC_Program
#define STATIC_Program

#include <string>

namespace Roket3D
{
	struct GameInfo
	{
		std::string EntryPointCall;
		std::string EntryPointFile;
		std::string CurrentWorkingDirectory;
		bool Debuggable;
	};

	class Program
	{
		public:
			static int Main(int argc, char *argv[]);
			static void ParseGameInfo(GameInfo & out);
	};
}

#endif