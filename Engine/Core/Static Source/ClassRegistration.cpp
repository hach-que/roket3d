// This file registers all of the available built-in classes
// with the engine.
#include "ClassRegistration.h"
#include "autobind/types.h"
#include "autobind/binding/lua.h"

// Include all of the classes to be registered.
#include "E.Collections/RColor.h"
#include "E.Collections/RVector3D.h"
#include "E.Unmanaged/Audio/RAudioSound.h"

namespace Roket3D
{
	void RegisterAllClasses(lua_State * L)
	{
		Bindings<Engine::Collections::RColor>::Register(L);
		Bindings<Engine::Collections::RVector3D>::Register(L);
		Bindings<Engine::Unmanaged::Audio::RAudioSound>::Register(L);
	}
}