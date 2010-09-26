// This file provides a debugging interface for the engine
// to report errors and warnings back to.

#include "Debugger.h"
#include "LowLevel.h"
#include "Exception.h"
#include "DebuggerNotAttachedException.h"
#include "InterpreterException.h"
#include "InvalidObjectThrownException.h"
#include <iostream>

namespace Roket3D
{
	bool Debugger::m_IsConnected = false;

	void Debugger::RaiseException(Engine::Exception & err)
	{
		if (Debugger::m_IsConnected)
		{
			// TODO: Send the exception over the network socket to the IDE.
		}
		else
		{
			std::cout << "An unhandled exception occurred in the game.  A" << std::endl;
			std::cout << "debugger was not attached to handle the exception." << std::endl;
			std::cout << "Information about the exception is outputted" << std::endl;
			std::cout << "below:" << std::endl;
			std::cout << std::endl;
			std::cout << err.GetName() << ": " << Engine::Exception::GetParsedMessage(err.GetMessage(), err.Arguments) << std::endl;
			std::cout << std::endl;
			std::cout << "occurred on line " << err.LineNumber << " in file '" << err.FileName << "'." << std::endl;
			std::cout << std::endl;
			std::cout << "Contact the game author with a copy of this message for" << std::endl;
			std::cout << "further assistance." << std::endl;
			throw Engine::DebuggerNotAttachedException();
		}
	}

	int Debugger::LuaExceptionHandle(lua_State * L)
	{
		// The error message is on top of the stack.
		// This is called when a lua_pcall() operation
		// fails.  We need to check whether the error
		// is a string or an Exception object just like
		// in LuaPanicHandle.
		if (lua_isstring(L, -1))
		{
			// It's a general Lua error.
			Debugger::RaiseException(Engine::InterpreterException(lua_tostring(L, -1)));
			return 0;
		}
		else
		{
			// Fetch the exception from the stack.  Check
			// to see whether the exception is of Engine::Exception
			// (i.e. we can safely pass it to Debugger::RaiseException,
			// or whether it is of another type, in which case
			// we should throw Engine::InvalidObjectThrownException
			// to indicate that we can't directly get the meaning
			// of the exception.

			// First push the type onto the stack.
			Bindings<Engine::Exception>::PushType(L);

			// Now repush the error object on top.
			lua_pushvalue(L, -2);   // TODO: This value is not valid when errors are raised by OP_RAISE

			// Check to see if it inherits from Engine::Exception.
			if (lua_is(L))
			{
				// Push the exception directly to Debugger::RaiseException.
				Engine::Exception & err = Bindings<Engine::Exception>::GetArgumentRef(L, -1);
				Debugger::RaiseException(err);
			}
			else
			{
				// We can't read the exception meaning directly, so throw
				// a Engine::InvalidObjectThrownException to inform the user
				// that it's the case.
				Debugger::RaiseException(Engine::InvalidObjectThrownException());
			}
			
			return 0;
		}
	}

	int Debugger::LuaPanicHandle(lua_State * L)
	{
		// The error message is on top of the stack, so we
		// check to see whether it's a string (Lua error)
		// or an Exception object.  If it's the former,
		// we use Debugger::RaiseException with a
		// GeneralLuaException.  If it's the latter, we
		// pass it as the parameter for the
		// Debugger::RaiseException call.
		if (lua_isstring(L, -1))
		{
			// It's a general Lua error.
			Debugger::RaiseException(Engine::InterpreterException(lua_tostring(L, -1)));
			return 0;
		}
		else
		{
			// Fetch the exception from the stack.  Check
			// to see whether the exception is of Engine::Exception
			// (i.e. we can safely pass it to Debugger::RaiseException,
			// or whether it is of another type, in which case
			// we should throw Engine::InvalidObjectThrownException
			// to indicate that we can't directly get the meaning
			// of the exception.

			// First push the type onto the stack.
			Bindings<Engine::Exception>::PushType(L);

			// Now repush the error object on top.
			lua_pushvalue(L, -2);

			// Check to see if it inherits from Engine::Exception.
			if (lua_is(L))
			{
				// Push the exception directly to Debugger::RaiseException.
				Engine::Exception & err = Bindings<Engine::Exception>::GetArgumentRef(L, -1);
				Debugger::RaiseException(err);
			}
			else
			{
				// We can't read the exception meaning directly, so throw
				// a Engine::InvalidObjectThrownException to inform the user
				// that it's the case.
				Debugger::RaiseException(Engine::InvalidObjectThrownException());
			}
			
			return 0;
		}
	}

	void Debugger::LuaHookInitalize(lua_State * L)
	{
		lua_sethook(L, &Debugger::LuaHookOnEvent, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);
	}

	void Debugger::LuaHookOnEvent(lua_State * L, lua_Debug * D)
	{
		if (Debugger::m_IsConnected)
		{
			// TODO: Send the debugging information back to the IDE over the socket.
			//       The IDE will send back data based on what the engine should do
			//       (i.e. pause at a breakpoint).
		}
	}

}