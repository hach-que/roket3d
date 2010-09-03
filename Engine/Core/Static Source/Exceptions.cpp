/*
A list of classes defining exceptions to be thrown
in bound classes.
*/

#include "Exceptions.h"

namespace Roket3D { namespace Exceptions
{
	ObjectNotValidException::ObjectNotValidException()
	{
	}

	ReadOnlyPropertyException::ReadOnlyPropertyException()
	{
	}

	InvalidArgumentTypeException::InvalidArgumentTypeException(int arg)
	{
	}

	NoContextProvidedException::NoContextProvidedException()
	{
	}

	InternalConstructorOnlyException::InternalConstructorOnlyException()
	{
	}
}