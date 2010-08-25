#include "roket3d.h"
#include "net.h"

bool NetInitalized = false;

void NetInitalize (lua_State * L)
{
	// First initalize SDL_net
	NetInitalized = true;
	if(SDL_Init(0)==-1) {
		printf("Unable to initalize SDL: %s\n", SDL_GetError());
		NetInitalized = false;
	}
	if(SDLNet_Init()==-1) {
		printf("Unable to initalize SDL_net: %s\n", SDLNet_GetError());
		NetInitalized = false;
	}

	lua_newtable(L);
	lua_setglobal(L,"Net");
	
	/* Register all classes here */
	Luna < net::RNetSocket >::Register			(L,"Net");
}