// This file provides a debugging interface for the engine
// to report errors and warnings back to.

#include "Debugger.h"
#include "Exceptions.h"
#include <iostream>

namespace Roket3D
{
	void Debugger::RaiseException(Exceptions::Exception * err, ...)
	{
		if (this->m_IsConnected)
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
			std::cout << 
		}
	}
}

#endif
