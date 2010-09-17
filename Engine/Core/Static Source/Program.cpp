// The main entry point of the Roket3D engine.
#include <lua.hpp>
#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "AccessViolationException.h"
#include "DivideByZeroException.h"
#include "EntryPointNotFoundException.h"
#include "OutOfMemoryException.h"
#include "InvalidSyntaxException.h"
#include "InterpreterStateNotValidException.h"

#include "Program.h"
#include "Utility.h"
#include "Debugger.h"
#include "ClassRegistration.h"
#include "LowLevel.h"
#include <iostream>
#include "irrXML.h"

int testFunc(lua_State * L)
{
	int * test = NULL;
	*test = 13;
	return 0;
}

int testFunc2(lua_State * L)
{
	int a = 4;
	int b = 0;
	int c = a / b;
	return 0;
}

namespace Roket3D
{
	int Program::Main(int argc, char *argv[])
	{
		for (int i = 0; i < argc; i += 1)
		{
			std::string arg = argv[i];
			if (argv[i] == "-v" || argv[i] == "--version" ||
				argv[i] == "-h" || argv[i] == "--help" ||
				argv[i] == "-l" || argv[i] == "--license")
			{
				// Print the licensing information
				std::cout << std::endl;
				std::cout << "/===================== GAME INFORMATION =====================\\" << std::endl;
				std::cout << "|                                                            |" << std::endl;
				std::cout << "|   Name: <unknown>                                          |" << std::endl;
				std::cout << "|   Version: <unknown>                                       |" << std::endl;
				std::cout << "|   Author: <unknown>                                        |" << std::endl;
				std::cout << "|   Build Target: Windows 7, 32-bit                          |" << std::endl;
				std::cout << "|   License: LGPL                                            |" << std::endl;
				std::cout << "|   Additional Information:                                  |" << std::endl;
				std::cout << "|     No additional information has been supplied for        |" << std::endl;
				std::cout << "|     this game.                                             |" << std::endl;
				std::cout << "|                                                            |" << std::endl;
				std::cout << "\\============================================================/" << std::endl;
				std::cout << std::endl;
				std::cout << "/==================== ENGINE INFORMATION ====================\\" << std::endl;
				std::cout << "|                                                            |" << std::endl;
				std::cout << "|   This application uses the open-source 3D game            |" << std::endl;
				std::cout << "|   engine, Roket3D.  More information can be found at:      |" << std::endl;
				std::cout << "|    * http://www.roket3d.com/            and                |" << std::endl;
				std::cout << "|    * http://code.google.com/p/roket3d                      |" << std::endl;
				std::cout << "|                                                            |" << std::endl;
				std::cout << "|   Name: Roket3D                                            |" << std::endl;
				std::cout << "|   Version: 2.0.0                                           |" << std::endl;
				std::cout << "|   Author: Roket Enterprises and contributors               |" << std::endl;
				std::cout << "|   Build Target: Windows 7, 32-bit                          |" << std::endl;
				std::cout << "|   Debugging is not available on this build.                |" << std::endl;
				std::cout << "|                                                            |" << std::endl;
				std::cout << "\\============================================================/" << std::endl;
				std::cout << std::endl;
				std::cout << "/=================== LICENSING INFORMATION ==================\\" << std::endl;
				std::cout << "|                                                            |" << std::endl;
				std::cout << "|   The following open source libraries are used within      |" << std::endl;
				std::cout << "|   this application:                                        |" << std::endl;
				std::cout << "|    * Irrlicht 1.6.1 (with modifications) [zlib]            |" << std::endl;
				std::cout << "|    * Lua 5.1.2 (with modifications) [public domain]        |" << std::endl;
				std::cout << "|    * Ogg                                                   |" << std::endl; // TODO: Find out version & license.
				std::cout << "|    * Vorbis                                                |" << std::endl;
				std::cout << "|    * ALUT                                                  |" << std::endl;
				std::cout << "|    * CAudio                                                |" << std::endl;
				std::cout << "|    * SDL                                                   |" << std::endl;
				std::cout << "|    * XEffects                                              |" << std::endl;
				std::cout << "|                                                            |" << std::endl;
				std::cout << "|   The following proprietory libraries are used within      |" << std::endl;
				std::cout << "|   this application:                                        |" << std::endl;
				std::cout << "|    * Newton Physics 2.00 Alpha [freeware w/ link]          |" << std::endl;
				std::cout << "|      http://www.newtondynamics.com                         |" << std::endl;
				std::cout << "|                                                            |" << std::endl;
				std::cout << "\\============================================================/" << std::endl;
				std::cout << std::endl;
				exit(0);
			}
		}

		// Test to see whether or not we can catch segmentation faults.
		try
		{
			__guard(&testFunc, NULL);
		}
		catch (Engine::AccessViolationException err)
		{
			std::cout << "Caught access violation!" << std::endl;
		}

		// Test to see whether or not we can catch division by zero faults.
		try
		{
			__guard(&testFunc2, NULL);
		}
		catch (Engine::DivideByZeroException err)
		{
			std::cout << "Caught division by zero!" << std::endl;
		}

		// Search for GameInfo.xml in the current working directory.
		if (!Utility::FileExists("./GameInfo.xml"))
		{
			Debugger::RaiseException(Engine::EntryPointNotFoundException());
			return -1;
		}

		// Get the game information from the GameInfo.xml file.
		GameInfo info;
		Program::ParseGameInfo(info);

		// Change the working directory.
		Utility::ChangeWorkingDirectory(info.CurrentWorkingDirectory);

		// Check to make sure the entry point file exists.
		if (!Utility::FileExists(info.EntryPointFile))
		{
			Debugger::RaiseException(Engine::EntryPointNotFoundException());
			return -1;
		}

		// Now create a Lua instance, and set up the Lua environment for
		// use within a game.
		lua_State * L = lua_open();
		if (L == NULL)
		{
			Debugger::RaiseException(Engine::InterpreterStateNotValidException());
			return -1;
		}
		luaL_openlibs(L);

		// Ensure there is enough stack space available for Lua.
		if (lua_checkstack(L, 50) == false)
		{
			Debugger::RaiseException(Engine::InterpreterStateNotValidException());
			return -1;
		}

		// Register the panic function.
		lua_atpanic(L, &Debugger::LuaPanicHandle);

		// Register the debugger hooks.
		Debugger::LuaHookInitalize(L);

		// Initalize all of the classes.
		Roket3D::RegisterAllClasses(L);

		// Load the entry point file up and run it.
		int ret = luaL_loadfile(L, info.EntryPointFile.c_str());
		if (ret == LUA_ERRSYNTAX)
		{
			std::string str = lua_tostring(L, -1);
			lua_pop(L, 1);
			Debugger::RaiseException(Engine::InvalidSyntaxException(str));
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(Engine::OutOfMemoryException());
			return -1;
		}

		ret = lua_pcall(L, 0, 0, 0);
		if (ret == LUA_ERRRUN)
		{
			Debugger::LuaExceptionHandle(L);
			// TODO: Implement a function to retrieve the exception from the Lua stack.
			//Exception * e = Bindings<Exceptions::Exception>::GetArgument(-1);
			//Debugger::RaiseException(e);
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(Engine::OutOfMemoryException());
			return -1;
		}

		// Load the entry point call string into Lua, and then run it.
		ret = luaL_loadstring(L, info.EntryPointCall.c_str());
		if (ret == LUA_ERRSYNTAX)
		{
			std::string str = lua_tostring(L, -1);
			lua_pop(L, 1);
			Debugger::RaiseException(Engine::InvalidSyntaxException(str));
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(Engine::OutOfMemoryException());
			return -1;
		}

		ret = lua_pcall(L, 0, 1, 0);
		if (ret == LUA_ERRRUN)
		{
			Debugger::LuaExceptionHandle(L);
			// TODO: Implement a function to retrieve the exception from the Lua stack.
			//Exception * e = Bindings<Exceptions::Exception>::GetArgument(-1);
			//Debugger::RaiseException(e);
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(Engine::OutOfMemoryException());
			return -1;
		}

		// Return the value from the lua_pcall if it's a numeric
		// value, otherwise ignore it and return 0.
		if (lua_isnumber(L, -1))
			ret = lua_tonumber(L, -1);
		else
			ret = 0;

		// Close the Lua interpreter.
		lua_close(L);

		return ret;
	}

	void Program::ParseGameInfo(GameInfo & out)
	{
		// Create an XML reader using irrXML.
		irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader("./GameInfo.xml");

		// Parse the file until EOF reached.
		std::vector<std::string> nodes;
		while (xml && xml->read())
		{
			switch (xml->getNodeType())
			{
				case irr::io::EXN_TEXT:
				case irr::io::EXN_CDATA:
					if (nodes.size() > 1 && nodes[nodes.size() - 1] == "cwd")
						out.CurrentWorkingDirectory = xml->getNodeData();
					else if (nodes.size() > 1 && nodes[nodes.size() - 1] == "debuggable")
						out.Debuggable = xml->getNodeData();
					else if (nodes.size() > 2 && nodes[nodes.size() - 2] == "entrypoint" && nodes[nodes.size() - 1] == "call")
						out.EntryPointCall = xml->getNodeData();
					else if (nodes.size() > 2 && nodes[nodes.size() - 2] == "entrypoint" && nodes[nodes.size() - 1] == "file")
						out.EntryPointFile = xml->getNodeData();
					else if (nodes.size() > 1 && nodes[nodes.size() - 1] == "entrypoint") { }
					else if (nodes.size() == 1 && nodes[0] == "game") { }
					else
					{			
						Debugger::RaiseException(Engine::EntryPointNotFoundException());
						return;
					}
					break;
				case irr::io::EXN_ELEMENT:
					nodes.insert(nodes.end(), xml->getNodeName());
					break;
				case irr::io::EXN_ELEMENT_END:
					nodes.pop_back();
					break;
			}
		}

		// Delete the XML parser after usage.
		delete xml;
	}
}