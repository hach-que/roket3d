/*
A list of classes defining exceptions to be thrown
in bound classes.
*/

#include "Exceptions.h"
#include "Utility.h"
#include <sstream>

namespace Roket3D { namespace Exceptions
{
	std::string Exception::GetParsedMessage()
	{
		std::string ret = this->GetMessage();
		for (int i = 0; i < this->Arguments.size(); i++)
		{
			std::stringstream from;
			from << "${" << (i + 1) << "}";
			ret = Utility::ReplaceAll(ret, from.str(), this->Arguments[i]);
		}
		return ret;
	}

	// Exceptions with no arguments.
	Exception::Exception()
	{
		this->LineNumber = 0;
		this->FileName = "<unknown>";
	}
	ObjectNotValidException::ObjectNotValidException() { }
	ReadOnlyPropertyException::ReadOnlyPropertyException() { }
	NoContextProvidedException::NoContextProvidedException() { }
	InternalConstructorOnlyException::InternalConstructorOnlyException() { }
	EntryPointNotFoundException::EntryPointNotFoundException() { }
	LuaStateNotValidException::LuaStateNotValidException() { }
	DebuggerNotAttachedException::DebuggerNotAttachedException() { }
	InvalidSyntaxException::InvalidSyntaxException() { }
	OutOfMemoryException::OutOfMemoryException() { }

	// Exceptions with arguments.
	InvalidArgumentTypeException::InvalidArgumentTypeException(int narg)
	{
		std::string pos = "";
		pos += narg;
		this->Arguments.push_back(pos);
	}

	PathNotFoundException::PathNotFoundException(std::string path)
	{
		this->Arguments.push_back(path);
	}

	PermissionDeniedException::PermissionDeniedException(std::string resource)
	{
		this->Arguments.push_back(resource);
	}

	GeneralLuaException::GeneralLuaException(std::string message)
	{
		this->Arguments.push_back(message);
	}

	// Definitions of all of the exception names and messages.
	const char* Exception::Name = "Exception";
	const char* ObjectNotValidException::Name = "ObjectNotValidException";
	const char* ReadOnlyPropertyException::Name = "ReadOnlyPropertyException";
	const char* NoContextProvidedException::Name = "NoContextProvidedException";
	const char* InternalConstructorOnlyException::Name = "InternalConstructorOnlyException";
	const char* EntryPointNotFoundException::Name = "EntryPointNotFoundException";
	const char* LuaStateNotValidException::Name = "LuaStateNotValidException";
	const char* DebuggerNotAttachedException::Name = "DebuggerNotAttachedException";
	const char* InvalidArgumentTypeException::Name = "InvalidArgumentTypeException";
	const char* PathNotFoundException::Name = "PathNotFoundException";
	const char* PermissionDeniedException::Name = "PermissionDeniedException";
	const char* InvalidSyntaxException::Name = "InvalidSyntaxException";
	const char* OutOfMemoryException::Name = "OutOfMemoryException";
	const char* GeneralLuaException::Name = "GeneralLuaException";

	const char* Exception::Message
		= "A general exception was raised.";
	const char* ObjectNotValidException::Message
		= "The object you are calling is no longer valid.";
	const char* ReadOnlyPropertyException::Message
		= "The specified property is read-only.";
	const char* NoContextProvidedException::Message
		= "There was no context for this function provided.  Ensure that you use the ':' operator on member functions.";
	const char* InternalConstructorOnlyException::Message
		= "Attempted to construct an object whose constructor is for internal use only.";
	const char* EntryPointNotFoundException::Message
		= "The entry point for the game could not be found, due to the GameInfo.xml file missing or being invalid.";
	const char* LuaStateNotValidException::Message
		= "The Lua interpreter is in an invalid state or could otherwise not be initalized correctly.";
	const char* DebuggerNotAttachedException::Message
		= "An unhandled exception occurred within Lua and there is no debugger attached to the process.";
	const char* InvalidArgumentTypeException::Message
		= "The specified argument at position ${1} is not of the correct type.";
	const char* PathNotFoundException::Message
		= "The specified path '${1}' could not be found.";
	const char* PermissionDeniedException::Message
		= "The current user or application was not permitted to access the specified resource '${1}'.";
	const char* InvalidSyntaxException::Message
		= "The engine attempted to load a file which contains one or more syntax errors.";
	const char* OutOfMemoryException::Message
		= "There was not enough memory to perform the operation.";
	const char* GeneralLuaException::Message
		= "A general Lua exception was raised within the Lua API.  The message raised from the Lua API is '${1}'.";
}
}