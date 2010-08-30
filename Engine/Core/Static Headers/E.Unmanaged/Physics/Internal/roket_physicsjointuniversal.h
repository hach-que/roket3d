#ifndef HEADER_ROKETPHYSICSJOINTUNIVERSAL
#define HEADER_ROKETPHYSICSJOINTUNIVERSAL

#include "roket_physicsbody.h"

namespace physics
{
	class RPhysicsJoint;

	class Roket_PhysicsJointUniversal
	{
		NewtonJoint* joint;
		NewtonWorld* world;

		public:
			Roket_PhysicsJointUniversal(
				NewtonWorld* worldd, 
				irr::core::vector3df point,
				irr::core::vector3df pin0, 
				irr::core::vector3df pin1, 
				Roket_PhysicsBody* child, 
				Roket_PhysicsBody* parent,
				RPhysicsJoint* rjoint);
			~Roket_PhysicsJointUniversal();
			dFloat getStiffness();
			void setStiffness(dFloat stiffness);
			void destroy();

			dFloat getAngle0();
			dFloat getAngle1();
			dFloat getOmega0();
			dFloat getOmega1();
			irr::core::vector3df getForce();
	};
}
#endif
