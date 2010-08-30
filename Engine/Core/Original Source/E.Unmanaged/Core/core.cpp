#include "roket3d.h"
#include "E.Unmanaged/Core/core.h"

void CoreInitalize (lua_State * L)
{
	lua_newtable(L);
	lua_setglobal(L,"Core");
	
	/* Register all classes here */
	Luna < core::RColor >::Register			(L,"Core");
	Luna < core::RMatrix >::Register		(L,"Core");
	Luna < core::RRectangle >::Register		(L,"Core");
	Luna < core::RVector2D >::Register		(L,"Core");
	Luna < core::RVector3D >::Register		(L,"Core");
	Luna < core::RRay2D >::Register			(L,"Core");
	Luna < core::RRay3D >::Register			(L,"Core");
	Luna < core::RTriangle3D >::Register	(L,"Core");
	Luna < core::RCube3D >::Register		(L,"Core");
	Luna < core::RPlane >::Register			(L,"Core");
	Luna < core::RQuaternion >::Register	(L,"Core");
	Luna < core::RDevice >::Register		(L,"Core");
	Luna < core::RCursorControl >::Register	(L,"Core");
	Luna < RoketEventReceiver >::Register	(L,"Core");
}
