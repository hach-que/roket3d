// This file registers all of the available built-in classes
// with the engine.

#ifndef STATIC_ClassRegistration
#define STATIC_ClassRegistration

#include <lua.hpp>

namespace Roket3D
{
	void RegisterAllClasses(lua_State * L);
}

#endif