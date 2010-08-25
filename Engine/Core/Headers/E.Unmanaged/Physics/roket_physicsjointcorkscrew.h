#ifndef HEADER_ROKETPHYSICSJOINTCORKSCREW
#define HEADER_ROKETPHYSICSJOINTCORKSCREW

#include "roket_physicsbody.h"

namespace physics
{
	class RPhysicsJoint;

	class Roket_PhysicsJointCorkscrew
	{
		NewtonJoint* joint;
		NewtonWorld* world;

		public:
			Roket_PhysicsJointCorkscrew(
				NewtonWorld* worldd, 
				irr::core::vector3df point, 
				irr::core::vector3df pin, 
				Roket_PhysicsBody* child, 
				Roket_PhysicsBody* parent,
				RPhysicsJoint* rjoint);
			~Roket_PhysicsJointCorkscrew();
			dFloat getStiffness();
			void setStiffness(dFloat stiffness);
			void destroy();

			dFloat getAngle();
			dFloat getOmega();
			dFloat getPosition();
			dFloat getVelocity();
			irr::core::vector3df getForce();
	};
}
#endif
