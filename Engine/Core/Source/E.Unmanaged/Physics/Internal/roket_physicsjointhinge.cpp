#include "roket3d.h"
#include "roket_physicsjointhinge.h"
#include "roket_physicscallbacks.h"

namespace physics
{
	Roket_PhysicsJointHinge::Roket_PhysicsJointHinge(
		NewtonWorld* worldd, 
		irr::core::vector3df point, 
		irr::core::vector3df pin, 
		Roket_PhysicsBody* child, 
		Roket_PhysicsBody* parent,
		RPhysicsJoint* rjoint)
	{
		world = worldd;
		float pointD[3];
		pointD[0] = point.X;
		pointD[1] = point.Y;
		pointD[2] = point.Z;
		float pointP[3];
		pointP[0] = pin.X;
		pointP[1] = pin.Y;
		pointP[2] = pin.Z;
		if (parent == NULL)
			joint = NewtonConstraintCreateHinge(world, &pointD[0], &pointP[0], child->body, NULL);
		else
			joint = NewtonConstraintCreateHinge(world, &pointD[0], &pointP[0], child->body, parent->body);
		NewtonJointSetUserData(joint, &rjoint);
		NewtonJointSetDestructor(joint, &JointDestructor);
	}

	Roket_PhysicsJointHinge::~Roket_PhysicsJointHinge()
	{
		//NewtonDestroyJoint(world, joint);
	}

	dFloat Roket_PhysicsJointHinge::getStiffness()
	{
		return NewtonJointGetStiffness(joint);
	}

	void Roket_PhysicsJointHinge::setStiffness(dFloat stiffness)
	{
		NewtonJointSetStiffness(joint,stiffness);
	}

	void Roket_PhysicsJointHinge::destroy()
	{
		NewtonDestroyJoint(world, joint);
	}

	// joint-specific functions
	dFloat Roket_PhysicsJointHinge::getAngle()
	{
		return NewtonHingeGetJointAngle(joint);
	}

	dFloat Roket_PhysicsJointHinge::getOmega()
	{
		return NewtonHingeGetJointOmega(joint);
	}

	irr::core::vector3df Roket_PhysicsJointHinge::getForce()
	{
		float angle[3];
		NewtonHingeGetJointForce(joint,&angle[0]);
		return irr::core::vector3df(angle[0],angle[1],angle[2]);
	}
}
