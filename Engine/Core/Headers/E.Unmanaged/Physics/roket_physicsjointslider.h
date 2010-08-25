#ifndef HEADER_ROKETPHYSICSJOINTSLIDER
#define HEADER_ROKETPHYSICSJOINTSLIDER

#include "roket_physicsbody.h"

namespace physics
{
	class RPhysicsJoint;

	class Roket_PhysicsJointSlider
	{
		NewtonJoint* joint;
		NewtonWorld* world;

		public:
			Roket_PhysicsJointSlider(NewtonWorld* worldd, 
				irr::core::vector3df point, 
				irr::core::vector3df pin, 
				Roket_PhysicsBody* child, 
				Roket_PhysicsBody* parent,
				RPhysicsJoint* rjoint);
			~Roket_PhysicsJointSlider();
			dFloat getStiffness();
			void setStiffness(dFloat stiffness);
			void destroy();

			dFloat getPosition();
			dFloat getVelocity();
			irr::core::vector3df getForce();
	};
}
#endif
