// The main entry point of the Roket3D engine.
#include "Program.h"
#include "Utility.h"
#include "Debugger.h"
#include "Exceptions.h"
#include <iostream>

namespace Roket3D
{
	int Program::Main(int argc, char *argv[])
	{
		// Print some required licensing information.
		std::cout << "This application uses the Newton Physics Library 2.00." << std::endl;
		std::cout << "See http://www.newtondynamics.com for more information." << std::endl;

		// Search for GameInfo.xml in the current working directory.
		if (Utility::FileExists("./GameInfo.xml"))
		{
			Debugger::RaiseException(new Exceptions::EntryPointNotFoundException());
			return;
		}
	}
}