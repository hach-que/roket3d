/*
A list of classes defining exceptions to be thrown
in bound classes.
*/

#ifndef STATIC_Exceptions
#define STATIC_Exceptions

#include <string>
#include <vector>

namespace Roket3D { namespace Exceptions
{
	class Exception
	{
		public:
			static const char* Name;
			static const char* Message;
			int LineNumber;
			std::string FileName;
			std::vector<std::string> Arguments;

			std::string GetParsedMessage();
	};

	class ObjectNotValidException : public Exception
	{
		public:
			ObjectNotValidException();

			static const char* Name;
			static const char* Message;
	};

	class ReadOnlyPropertyException : public Exception
	{
		public:
			ReadOnlyPropertyException();

			static const char* Name;
			static const char* Message;
	};

	class InvalidArgumentTypeException : public Exception
	{
		public:
			InvalidArgumentTypeException(int arg);

			static const char* Name;
			static const char* Message;
	};

	class NoContextProvidedException : public Exception
	{
		public:
			NoContextProvidedException();

			static const char* Name;
			static const char* Message;
	};

	class InternalConstructorOnlyException : public Exception
	{
		public:
			InternalConstructorOnlyException();

			static const char* Name;
			static const char* Message;
	};

	class EntryPointNotFoundException : public Exception
	{
		public:
			EntryPointNotFoundException();

			static const char* Name;
			static const char* Message;
	};

	class LuaStateNotValidException : public Exception
	{
		public:
			LuaStateNotValidException();

			static const char* Name;
			static const char* Message;
	};

	class PathNotFoundException : public Exception
	{
		public:
			PathNotFoundException(std::string path);

			static const char* Name;
			static const char* Message;
	};

	class PermissionDeniedException : public Exception
	{
		public:
			PermissionDeniedException(std::string resource);

			static const char* Name;
			static const char* Message;
	};

	class DebuggerNotAttachedException : public Exception
	{
		public:
			DebuggerNotAttachedException();

			static const char* Name;
			static const char* Message;
	};

	class InvalidSyntaxException : public Exception
	{
		public:
			InvalidSyntaxException();

			static const char* Name;
			static const char* Message;
	};

	class OutOfMemoryException : public Exception
	{
		public:
			OutOfMemoryException();

			static const char* Name;
			static const char* Message;
	};
}
}

#endif