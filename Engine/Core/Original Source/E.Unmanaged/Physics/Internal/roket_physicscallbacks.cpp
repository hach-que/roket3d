// Damn Newton wants static callbacks
// So we put our Roket_PhysicsBody in the NewtonBody's userdata
// and then call that function from the callback
// It then may in turn call RSceneNode->setPosition();

#include "roket3d.h"

namespace physics
{
	void _cdecl TransformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
	{
		// The position has been updated
		Roket_PhysicsBody* rbody = reinterpret_cast<Roket_PhysicsBody*>(NewtonBodyGetUserData(body));
		//matrix
		rbody->updateSelf(matrix);
	}

	void _cdecl ApplyForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
	{
		Roket_PhysicsBody* rbody = reinterpret_cast<Roket_PhysicsBody*>(NewtonBodyGetUserData(body));
		rbody->updateForce();
		rbody->updateTorque();
	}

	void _cdecl JointDestructor(const NewtonJoint* joint)
	{
		// Notifies the RPhysicsJoint that it is no longer valid
		RPhysicsJoint* rjoint = reinterpret_cast<RPhysicsJoint*>(NewtonJointGetUserData(joint));
		rjoint->setInvalid();
	}
}
		