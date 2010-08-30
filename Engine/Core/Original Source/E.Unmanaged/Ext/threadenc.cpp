#include "roket3d.h"
#include "E.Unmanaged/Ext/threadenc.h"

namespace ext
{
	ThreadEnc::ThreadEnc()
	{
		threadStarted=false;
	}
	
	int ThreadEnc::start(lua_State* L)
	{
		ifistrue(!threadStarted);
		
		thread = SDL_CreateThread(&ThreadEnc::proxy, this);
		if (thread==NULL)
		{
			debug("Unable to spawn process.", LVL_ERR,L);
			return 1;
		}
		threadStarted=true;
		return 0;
		
		endifisvalid();
	}
	
	ThreadEnc::~ThreadEnc()
	{
	}
	
	int ThreadEnc::proxy(void *pInst)
	{
		// converts the pointer passed through SDL to
		// a pointer to SDLThread, calls the derived
		// implementation virtually
		return reinterpret_cast<ThreadEnc*>(pInst)->main();
	}
}
