#include "roket3d.h"
#include "roket_physicsjointuniversal.h"
#include "roket_physicscallbacks.h"

namespace physics
{
	Roket_PhysicsJointUniversal::Roket_PhysicsJointUniversal(
		NewtonWorld* worldd, 
		irr::core::vector3df point, 
		irr::core::vector3df pin0, 
		irr::core::vector3df pin1, 
		Roket_PhysicsBody* child, 
		Roket_PhysicsBody* parent,
		RPhysicsJoint* rjoint)
	{
		world = worldd;
		float pointD[3];
		pointD[0] = point.X;
		pointD[1] = point.Y;
		pointD[2] = point.Z;
		float pointP0[3];
		pointP0[0] = pin0.X;
		pointP0[1] = pin0.Y;
		pointP0[2] = pin0.Z;
		float pointP1[3];
		pointP1[0] = pin1.X;
		pointP1[1] = pin1.Y;
		pointP1[2] = pin1.Z;
		if (parent == NULL)
			joint = NewtonConstraintCreateUniversal(world, &pointD[0], &pointP0[0], &pointP1[0], child->body, NULL);
		else
			joint = NewtonConstraintCreateUniversal(world, &pointD[0], &pointP0[0], &pointP1[0], child->body, parent->body);
		NewtonJointSetUserData(joint, &rjoint);
		NewtonJointSetDestructor(joint, &JointDestructor);
	}

	Roket_PhysicsJointUniversal::~Roket_PhysicsJointUniversal()
	{
		//NewtonDestroyJoint(world, joint);
	}

	dFloat Roket_PhysicsJointUniversal::getStiffness()
	{
		return NewtonJointGetStiffness(joint);
	}

	void Roket_PhysicsJointUniversal::setStiffness(dFloat stiffness)
	{
		NewtonJointSetStiffness(joint,stiffness);
	}

	void Roket_PhysicsJointUniversal::destroy()
	{
		NewtonDestroyJoint(world, joint);
	}

	// joint-specific functions
	dFloat Roket_PhysicsJointUniversal::getAngle0()
	{
		return NewtonUniversalGetJointAngle0(joint);
	}

	dFloat Roket_PhysicsJointUniversal::getAngle1()
	{
		return NewtonUniversalGetJointAngle1(joint);
	}

	dFloat Roket_PhysicsJointUniversal::getOmega0()
	{
		return NewtonUniversalGetJointOmega0(joint);
	}

	dFloat Roket_PhysicsJointUniversal::getOmega1()
	{
		return NewtonUniversalGetJointOmega1(joint);
	}

	irr::core::vector3df Roket_PhysicsJointUniversal::getForce()
	{
		float angle[3];
		NewtonUniversalGetJointForce(joint,&angle[0]);
		return irr::core::vector3df(angle[0],angle[1],angle[2]);
	}
}
