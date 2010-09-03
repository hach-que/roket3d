// This file provides a debugging interface for the engine
// to report errors and warnings back to.

#ifndef STATIC_Debugger
#define STATIC_Debugger

#include <lua.hpp>
#include "Exceptions.h"

namespace Roket3D
{
	class Debugger
	{
		public:
			static void RaiseException(Exceptions::Exception * err);			

		private:
			static bool m_IsConnected;
	};
}

#endif