#include "roket3d.h"
#include "audio.h"

void AudioInitalize (lua_State * L)
{
	lua_newtable(L);
	lua_setglobal(L,"Audio");
	
	/* Register all classes here */
	Luna < audio::RAudioController >::Register	(L,"Audio");
	Luna < audio::RAudioSound >::Register		(L,"Audio");
}
