#include "roket3d.h"
#include "E.Unmanaged/Ext/ext.h"

void ExtInitalize (lua_State * L)
{
	lua_newtable(L);
	lua_setglobal(L,"Ext");
	
	/* Register all classes here */
	//Luna < ext::RThread >::Register (L, "Ext");
}
