#include "roket3d.h"
#include "E.Unmanaged/Video/video.h"

void VideoInitalize (lua_State * L)
{
	lua_newtable(L);
	lua_setglobal(L,"Video");
	
	/* Register all classes here */
	Luna < video::RVideoDriver >::Register	(L,"Video");
	Luna < video::RTexture >::Register	(L,"Video");
	Luna < video::RMaterial >::Register	(L,"Video");
}
