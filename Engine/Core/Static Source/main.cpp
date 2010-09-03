// C entry point which simply calls the Program's entry point.

int main(int argc, char *argv[])
{
	return Roket3D::Program::Main(argc, argv);
}

/*#if defined(WIN32) || defined(WIN64)
#include "windows.h"
int main2(int argc, char *argv[]);

int WINAPI WinMain(HINSTANCE hInstance, //handle to current instance
                   HINSTANCE hPrevInstance, //pointer to the previous instance
                   LPSTR lpCmdLine,  //pointer to the command file
                   int nCmdShow)     //show state of the window
{
	main2(__argc,__argv);
}

int main2(int argc, char *argv[])
#else*/
int main(int argc, char *argv[])
//#endif
{
	cout << "Initalizing Roket3D..." << endl;
	cout << "This application uses the Newton Physics Library 2.00." << endl;
	cout << "See http://www.newtondynamics.com for more information." << endl;
	parseArguments(argc, argv);
	if (luaFile == "")
		return EXIT_FAILURE;
	if (!fileExists(luaFile))
	{
		nonlua_debug(FILE_NOT_FOUND("execute game",luaFile.c_str()).c_str(),LVL_FTL);
		return EXIT_FAILURE;
	}
	cout << " ... Executing " << ((const char*)luaFile.c_str()) << endl;

	int ret = EXIT_SUCCESS;
	
	if (luaFile!="")
	{
		RoketLUA = lua_open();
		if (NULL == RoketLUA)
		{
			nonlua_debug("Error initializing Lua.",LVL_FTL);
			return -1;
		}
		luaL_openlibs(RoketLUA);

		if (lua_checkstack(RoketLUA, 50) == false)
		{
			nonlua_debug("Unable to allocate enough stack space.",LVL_FTL);
			return -1;
		}

		initalizeRoket3D(RoketLUA, argc, argv);
		lua_register(RoketLUA,"__internal_luaErrorHandler_errOnly",luaErrorHandler_errOnly); // for errors in the event handler
		lua_pushcfunction(RoketLUA, luaErrorHandler);
		//lua_register(RoketLUA,"__internal_luaErrorHandler",luaErrorHandler);
		//lua_getfield(RoketLUA, LUA_GLOBALSINDEX, "__internal_luaErrorHandler");
		
		//showstack(RoketLUA);
		luaL_loadfile(RoketLUA, (const char*)luaFile.c_str());
		if (lua_isstring(RoketLUA,0))
		{
			// syntax error
			luaErrorHandler_syntaxErr(lua_tostring(RoketLUA,0));
			cout << "A syntax error occured.  See above for information." << endl;
			ret=EXIT_FAILURE;
			lua_close(RoketLUA);
			return ret;
		}
		int result = lua_pcall(RoketLUA,0,0,-2);
		switch (result)
		{
			case LUA_ERRRUN:
					cout << "A runtime error occured.  See above for information." << endl;
					ret=EXIT_FAILURE; break;
			case LUA_ERRMEM:
					cout << "A memory allocation error occured." << endl;
					ret=EXIT_FAILURE; break;
			case LUA_ERRERR:
					nonlua_debug(INTERNAL_ERROR("call error handling function").c_str(),LVL_FTL);
					ret=EXIT_FAILURE; break;
		}
		//showstack(RoketLUA);
		lua_close(RoketLUA);
	}
	else
	{
		ret=EXIT_FAILURE;
	}
	return ret;
}
