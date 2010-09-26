// This file registers all of the available built-in classes
// with the engine.
#include "ClassRegistration.h"
#include "autobind/types.h"
#include "autobind/binding/lua.h"

// Include all of the classes to be registered.
#include "E.Collections/RColor.h"
#include "E.Collections/RVector3D.h"
#include "E.Unmanaged/Audio/RAudioSound.h"

#include "AccessViolationException.h"
#include "ArgumentTypeNotValidException.h"
#include "ContextNotProvidedException.h"
#include "DataMisalignedException.h"
#include "DebuggerNotAttachedException.h"
#include "DivideByZeroException.h"
#include "EngineException.h"
#include "EntryPointNotFoundException.h"
#include "Exception.h"
#include "IllegalInstructionException.h"
#include "InheritedClassNotFoundException.h"
#include "InternalConstructorOnlyException.h"
#include "InterpreterException.h"
#include "InterpreterStateNotValidException.h"
#include "InvalidOperationException.h"
#include "InvalidSyntaxException.h"
#include "KeyNotFoundException.h"
#include "LowLevelException.h"
#include "NilReferenceException.h"
#include "ObjectDisposedException.h"
#include "OutOfMemoryException.h"
#include "OverflowException.h"
#include "PathNotFoundException.h"
#include "PermissionDeniedException.h"
#include "PrivilegedInstructionException.h"
#include "ReadOnlyPropertyException.h"
#include "UnderflowException.h"

namespace Roket3D
{
	void RegisterAllClasses(lua_State * L)
	{
		Bindings<Engine::Collections::RColor>::Register(L);
		Bindings<Engine::Collections::RVector3D>::Register(L);
		Bindings<Engine::Unmanaged::Audio::RAudioSound>::Register(L);

		// Register the exception classes.  Please note that if they
		// aren't registered when they are fired, they won't be properly
		// accessible because the RaiseException function of the Lua
		// bindings relies on the entry being in the Lua registry.
		Bindings<Engine::AccessViolationException>::Register(L);
		Bindings<Engine::ArgumentTypeNotValidException>::Register(L);
		Bindings<Engine::ContextNotProvidedException>::Register(L);
		Bindings<Engine::DataMisalignedException>::Register(L);
		Bindings<Engine::DebuggerNotAttachedException>::Register(L);
		Bindings<Engine::DivideByZeroException>::Register(L);
		Bindings<Engine::EngineException>::Register(L);
		Bindings<Engine::EntryPointNotFoundException>::Register(L);
		Bindings<Engine::Exception>::Register(L);
		Bindings<Engine::IllegalInstructionException>::Register(L);
		Bindings<Engine::InheritedClassNotFoundException>::Register(L);
		Bindings<Engine::InternalConstructorOnlyException>::Register(L);
		Bindings<Engine::InterpreterException>::Register(L);
		Bindings<Engine::InterpreterStateNotValidException>::Register(L);
		Bindings<Engine::InvalidOperationException>::Register(L);
		Bindings<Engine::InvalidSyntaxException>::Register(L);
		Bindings<Engine::KeyNotFoundException>::Register(L);
		Bindings<Engine::LowLevelException>::Register(L);
		Bindings<Engine::NilReferenceException>::Register(L);
		Bindings<Engine::ObjectDisposedException>::Register(L);
		Bindings<Engine::OutOfMemoryException>::Register(L);
		Bindings<Engine::OverflowException>::Register(L);
		Bindings<Engine::PathNotFoundException>::Register(L);
		Bindings<Engine::PermissionDeniedException>::Register(L);
		Bindings<Engine::PrivilegedInstructionException>::Register(L);
		Bindings<Engine::ReadOnlyPropertyException>::Register(L);
		Bindings<Engine::UnderflowException>::Register(L);
	}
}