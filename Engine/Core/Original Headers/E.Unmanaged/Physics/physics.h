#ifndef HEADER_RPHYSICS
#define HEADER_RPHYSICS

// These are the actual classes
//#include "roket_physicsmaterialhandler.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsmanager.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsjointball.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsjointhinge.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsjointslider.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsjointcorkscrew.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsjointuniversal.h"


// And these are the LUA wrappers
//#include "rphysicsmaterialhandler.h"
#include "E.Unmanaged/Physics/rphysicsmanager.h"
#include "E.Unmanaged/Physics/rphysicsjoint.h"

void PhysicsInitalize (lua_State * L);

#endif
