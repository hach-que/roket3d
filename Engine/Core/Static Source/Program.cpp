// The main entry point of the Roket3D engine.
#include "Program.h"
#include "Utility.h"
#include "Debugger.h"
#include "Exceptions.h"
#include "ClassRegistration.h"
#include <iostream>
#include "irrXML.h"

namespace Roket3D
{
	int Program::Main(int argc, char *argv[])
	{
		// Print some required licensing information.
		std::cout << "This application uses the Newton Physics Library 2.00." << std::endl;
		std::cout << "See http://www.newtondynamics.com for more information." << std::endl;

		// Search for GameInfo.xml in the current working directory.
		if (!Utility::FileExists("./GameInfo.xml"))
		{
			Debugger::RaiseException(new Exceptions::EntryPointNotFoundException());
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
			Debugger::RaiseException(new Exceptions::EntryPointNotFoundException());
			return -1;
		}

		// Now create a Lua instance, and set up the Lua environment for
		// use within a game.
		lua_State * L = lua_open();
		if (L == NULL)
		{
			Debugger::RaiseException(new Exceptions::LuaStateNotValidException());
			return -1;
		}
		luaL_openlibs(L);

		// Ensure there is enough stack space available for Lua.
		if (lua_checkstack(L, 50) == false)
		{
			Debugger::RaiseException(new Exceptions::LuaStateNotValidException());
			return -1;
		}

		// Initalize all of the classes.
		Roket3D::RegisterAllClasses(L);

		// Load the entry point file up and run it.
		int ret = luaL_loadfile(L, info.EntryPointFile.c_str());
		if (ret == LUA_ERRSYNTAX)
		{
			Debugger::RaiseException(new Exceptions::InvalidSyntaxException());
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(new Exceptions::OutOfMemoryException());
			return -1;
		}

		ret = lua_pcall(L, 0, 0, 0);
		if (ret == LUA_ERRRUN)
		{
			// TODO: Implement a function to retrieve the exception from the Lua stack.
			//Exception * e = Bindings<Exceptions::Exception>::GetArgument(-1);
			//Debugger::RaiseException(e);
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(new Exceptions::OutOfMemoryException());
			return -1;
		}

		// Load the entry point call string into Lua, and then run it.
		ret = luaL_loadstring(L, info.EntryPointCall.c_str());
		if (ret == LUA_ERRSYNTAX)
		{
			Debugger::RaiseException(new Exceptions::InvalidSyntaxException());
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(new Exceptions::OutOfMemoryException());
			return -1;
		}

		ret = lua_pcall(L, 0, 1, 0);
		if (ret == LUA_ERRRUN)
		{
			// TODO: Implement a function to retrieve the exception from the Lua stack.
			//Exception * e = Bindings<Exceptions::Exception>::GetArgument(-1);
			//Debugger::RaiseException(e);
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(new Exceptions::OutOfMemoryException());
			return -1;
		}

		// Return the value from the lua_pcall if it's a numeric
		// value, otherwise ignore it and return 0.
		if (lua_isnumber(L, -1))
			return lua_tonumber(L, -1);
		else
			return 0;
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
					else
					{			
						Debugger::RaiseException(new Exceptions::EntryPointNotFoundException());
						return;
					}
					break;
				case irr::io::EXN_ELEMENT:
					nodes.insert(nodes.begin(), xml->getNodeName());
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