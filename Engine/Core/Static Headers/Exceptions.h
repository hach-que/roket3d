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
			Exception();

			static const char* Name;
			static const char* Message;
			int LineNumber;
			std::string FileName;
			std::vector<std::string> Arguments;

			virtual std::string GetParsedMessage();
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class ObjectNotValidException : public Exception
	{
		public:
			ObjectNotValidException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class ReadOnlyPropertyException : public Exception
	{
		public:
			ReadOnlyPropertyException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class InvalidArgumentTypeException : public Exception
	{
		public:
			InvalidArgumentTypeException(int arg);

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class NoContextProvidedException : public Exception
	{
		public:
			NoContextProvidedException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class InternalConstructorOnlyException : public Exception
	{
		public:
			InternalConstructorOnlyException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class EntryPointNotFoundException : public Exception
	{
		public:
			EntryPointNotFoundException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class LuaStateNotValidException : public Exception
	{
		public:
			LuaStateNotValidException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class PathNotFoundException : public Exception
	{
		public:
			PathNotFoundException(std::string path);

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class PermissionDeniedException : public Exception
	{
		public:
			PermissionDeniedException(std::string resource);

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class DebuggerNotAttachedException : public Exception
	{
		public:
			DebuggerNotAttachedException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class InvalidSyntaxException : public Exception
	{
		public:
			InvalidSyntaxException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class OutOfMemoryException : public Exception
	{
		public:
			OutOfMemoryException();

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};

	class GeneralLuaException : public Exception
	{
		public:
			GeneralLuaException(std::string message);

			static const char* Name;
			static const char* Message;
			virtual inline const char* GetName() { return this->Name; }
			virtual inline const char* GetMessage() { return this->Message; }
	};
}
}

#endif