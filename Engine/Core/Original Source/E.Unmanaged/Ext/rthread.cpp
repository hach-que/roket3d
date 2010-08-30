#if 0==1
#include "roket3d.h"
#include "rthread.h"

namespace ext
{
	int RThread::main()
	{
		lua_State* myLuaState;
		if (luaFile!="")
		{
			if (!fileExists(luaFile))
			{
				nonlua_debug(FILE_NOT_FOUND("spawn process",luaFile.c_str()).c_str(),LVL_ERR);
			}
			else
			{
				nonlua_debug("Spawning " << luaFile.c_str() << "...",LVL_INFO);
				myLuaState = lua_open();
				if (NULL == myLuaState)
				{
					nonlua_debug("Error initalizing Lua in spawned process.",LVL_ERR);
					return 1;
				}
				luaL_openlibs(myLuaState);
				initalizeRoket3D(myLuaState);
				lua_register(myLuaState,"__internal_luaErrorHandler_errOnly",luaErrorHandler_errOnly);
				lua_pushcfunction(myLuaState, luaErrorHandler);
				luaL_loadfile(myLuaState, (const char*)luaFile.c_str());
				if (lua_isstring(myLuaState,0))
				{
					// syntax error
					luaErrorHandler_syntaxErr(lua_tostring(myLuaState,0));
					cout << "A syntax error occured.  See above for information." << endl;
					lua_close(myLuaState);
					return 0;
				}
				int result = lua_pcall(myLuaState,0,0,-2);
				switch (result)
				{
					case LUA_ERRRUN:
							cout << "A runtime error occured.  See above for information." << endl;
							break;
					case LUA_ERRMEM:
							cout << "A memory allocation error occured." << endl;
							break;
					case LUA_ERRERR:
							nonlua_debug(INTERNAL_ERROR("call error handling function").c_str(),LVL_FTL);
							break;
				}
				lua_close(myLuaState);
			}
		}
		else
		{
			nonlua_debug("Could not spawn process because you have not specified a file.",LVL_ERR);
		}
		threadStarted=false;
		if (lost == true)
		{
			// we been lost from lua
			// so let's commit suicide
			delete this;
		}
		return 0;
	}
	
	const char *RThread::className = TYPE_REXTTHREAD;
	const Luna < RThread >::FunctionType RThread::Functions[] = {
		{"changeFile", &RThread::changeFile},
  {"start", &RThread::start},
  {"stop", &RThread::stop},
  {"isFinished", &RThread::isFinished},
  {"waitTillFinished", &RThread::waitTillFinished},
  {0}
	};
	const Luna < RThread >::PropertyType RThread::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RThread::RThread(lua_State* L, bool instantedByUser)
	{
		lost = false;
		luaFile=getoptionalargument(L,0,"");
		if (luaFile=="")
		{
			debug("Make sure you set a filename for this thread before attempting to start it.",LVL_WARN,L);
		}
	}
	
	int RThread::stop(lua_State* L)
	{
		ifistrue(threadStarted);
		
		debug("You should wait for processes to finish instead of killing them.",LVL_WARN,L);
		SDL_KillThread(thread);
		threadStarted=false;
		return 0;
		
		endifisvalid();
	}
	
	int RThread::changeFile(lua_State* L)
	{
		ifistrue(!threadStarted);
		
		luaFile=getmandatoryargument_constchar(L,1);
		return 0;
		
		endifisvalid();
	}
	
	int RThread::isFinished(lua_State* L)
	{
		lua_pushboolean(L,!threadStarted);
		return 1;
	}
	
	int RThread::waitTillFinished(lua_State* L)
	{
		if (threadStarted)
		{
			int timeOut;
			long i;
			timeOut=getoptionalargument(L,1,-1);
			if (timeOut==-1)
			{
				// wait till stopped
				int ret; // not used
				SDL_WaitThread(thread,&ret);
				lua_pushboolean(L,true);
				return 1;
			}
			else
			{
				// wait until either stopped or timeout
				for (i=0;i<timeOut;i++)
				{
					if (threadStarted)
					{
						SDL_Delay(1);
					}
					else
					{
						lua_pushboolean(L,true);
						return 1;
					}
				}
				lua_pushboolean(L,false);
				return 1;
			}
		}
		else
		{
			lua_pushboolean(L,true);
			return 1;
		}	
	}
}

template <> int Luna<ext::RThread>::gc_obj(lua_State *L)
{
	//we are RThread, so we need to let the thread run till it's finished
	ext::RThread **obj = static_cast < ext::RThread ** >(luaL_testudata (L, -1, ext::RThread::className));
	(*obj)->lost = true;
	return 0;
}
#endif