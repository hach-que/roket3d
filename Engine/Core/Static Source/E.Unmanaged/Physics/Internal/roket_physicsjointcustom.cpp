#ifdef INCLUDECUSTOMJOINT
#include "roket3d.h"
#include "roket_physicsjointcustom.h"
#include "roket_physicscallbacks.h"

namespace physics
{
	Roket_PhysicsJointCustom::Roket_PhysicsJointCustom(
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
			joint = NewtonConstraintCreateCorkscrew(world, &pointD[0], &pointP[0], child->body, NULL);
		else
			joint = NewtonConstraintCreateCorkscrew(world, &pointD[0], &pointP[0], child->body, parent->body);
		NewtonJointSetUserData(joint, &rjoint);
		NewtonJointSetDestructor(joint, &JointDestructor);
	}

	Roket_PhysicsJointCustom::~Roket_PhysicsJointCustom()
	{
		//NewtonDestroyJoint(world, joint);
	}

	dFloat Roket_PhysicsJointCustom::getStiffness()
	{
		return NewtonJointGetStiffness(joint);
	}

	void Roket_PhysicsJointCustom::setStiffness(dFloat stiffness)
	{
		NewtonJointSetStiffness(joint,stiffness);
	}

	void Roket_PhysicsJointCustom::destroy()
	{
		NewtonDestroyJoint(world, joint);
	}
}

#endif