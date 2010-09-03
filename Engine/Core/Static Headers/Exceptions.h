/*
A list of classes defining exceptions to be thrown
in bound classes.
*/

#ifndef STATIC_Exceptions
#define STATIC_Exceptions

namespace Roket3D { namespace Exceptions
{
	class Exception
	{
		public:
			virtual static const char* m_Name;
			virtual static const char* m_Message = 0;
			int m_LineNumber;
			std::string m_FileName;
	}

	class ObjectNotValidException : public Exception
	{
		public:
			ObjectNotValidException();

			static const char* m_Message = "The object you are calling is no longer valid.";
	};

	class ReadOnlyPropertyException : public Exception
	{
		public:
			ReadOnlyPropertyException();

			static const char* m_Message = "The specified property is read-only.";
	};

	class InvalidArgumentTypeException : public Exception
	{
		public:
			InvalidArgumentTypeException(int arg);

			static const char* m_Message = "The specified argument at position ${1} is not of the correct type.";
	};

	class NoContextProvidedException : public Exception
	{
		public:
			NoContextProvidedException();

			static const char* m_Message = "There was no context for this function provided.  Ensure that you use the ':' operator on member functions.";
	};

	class InternalConstructorOnlyException : public Exception
	{
		public:
			InternalConstructorOnlyException();

			static const char* m_Message = "Attempted to construct an object whose constructor is for internal use only.";
	};

	class EntryPointNotFoundException : public Exception
	{
		public:
			EntryPointNotFoundException();

			static const char* m_Message = "The entry point for the game could not be found, due to a missing GameInfo.xml file.";
	};
}

#endif