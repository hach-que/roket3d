#ifndef HEADER_ROKETPHYSICSJOINTBALL
#define HEADER_ROKETPHYSICSJOINTBALL

#include "roket_physicsbody.h"

namespace physics
{
	class RPhysicsJoint;

	class Roket_PhysicsJointBall
	{
		NewtonJoint* joint;
		NewtonWorld* world;

		public:
			Roket_PhysicsJointBall(
				NewtonWorld* worldd,
				irr::core::vector3df point,
				Roket_PhysicsBody* child,
				Roket_PhysicsBody* parent,
				RPhysicsJoint* rjoint);
			~Roket_PhysicsJointBall();
			dFloat getStiffness();
			void setStiffness(dFloat stiffness);
			void destroy();

			irr::core::vector3df getAngle();
			irr::core::vector3df getOmega();
			irr::core::vector3df getForce();
			void setLimits(irr::core::vector3df conePin, double maxConeAngle, double maxTwistAngle);
	};
}
#endif
