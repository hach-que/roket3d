#include "roket3d.h"
#include "physics.h"

void PhysicsInitalize (lua_State * L)
{
	lua_newtable(L);
	lua_setglobal(L,"Physics");
	
	/* Register all classes here */
	//Luna < physics::RPhysicsMaterialHandler >::Register	(L,"Physics");
	Luna < physics::RPhysicsManager >::Register		(L,"Physics");
	Luna < physics::RPhysicsJoint >::Register		(L,"Physics");
}
