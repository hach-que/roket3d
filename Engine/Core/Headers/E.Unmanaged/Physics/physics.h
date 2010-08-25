#ifndef HEADER_RPHYSICS
#define HEADER_RPHYSICS

// These are the actual classes
//#include "roket_physicsmaterialhandler.h"
#include "roket_physicsmanager.h"
#include "roket_physicsjointball.h"
#include "roket_physicsjointhinge.h"
#include "roket_physicsjointslider.h"
#include "roket_physicsjointcorkscrew.h"
#include "roket_physicsjointuniversal.h"


// And these are the LUA wrappers
//#include "rphysicsmaterialhandler.h"
#include "rphysicsmanager.h"
#include "rphysicsjoint.h"

void PhysicsInitalize (lua_State * L);

#endif
