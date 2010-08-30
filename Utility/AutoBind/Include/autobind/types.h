/*
 * This header file is part of the AutoBind
 * toolset.  It typedef's generic types such
 * as string and numeric to their C++
 * equivilants.
 */

#ifndef AUTOBIND_types
#define AUTOBIND_types

// Include Lua.
#include <lua.hpp>

typedef lua_Number numeric;
typedef const char* string;
typedef void table;

// TODO: Replace the typedefs with actual classes
//       which properly handle the conversion and
//       can be handled properly by the Bindings<>
//       template.

#endif