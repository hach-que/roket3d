#ifndef HEADER_ROKETPHYSICSJOINTHINGE
#define HEADER_ROKETPHYSICSJOINTHINGE

#include "roket_physicsbody.h"

namespace physics
{
	class RPhysicsJoint;

	class Roket_PhysicsJointHinge
	{
		NewtonJoint* joint;
		NewtonWorld* world;

		public:
			Roket_PhysicsJointHinge(NewtonWorld* worldd, 
				irr::core::vector3df point, 
				irr::core::vector3df pin, 
				Roket_PhysicsBody* child, 
				Roket_PhysicsBody* parent,
				RPhysicsJoint* rjoint);
			~Roket_PhysicsJointHinge();
			dFloat getStiffness();
			void setStiffness(dFloat stiffness);
			void destroy();

			dFloat getAngle();
			dFloat getOmega();
			irr::core::vector3df getForce();
	};
}
#endif
