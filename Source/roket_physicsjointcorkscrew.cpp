#include "roket3d.h"
#include "roket_physicsjointcorkscrew.h"
#include "roket_physicscallbacks.h"

namespace physics
{
	Roket_PhysicsJointCorkscrew::Roket_PhysicsJointCorkscrew(
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

	Roket_PhysicsJointCorkscrew::~Roket_PhysicsJointCorkscrew()
	{
		//NewtonDestroyJoint(world, joint);
	}

	dFloat Roket_PhysicsJointCorkscrew::getStiffness()
	{
		return NewtonJointGetStiffness(joint);
	}

	void Roket_PhysicsJointCorkscrew::setStiffness(dFloat stiffness)
	{
		NewtonJointSetStiffness(joint,stiffness);
	}

	void Roket_PhysicsJointCorkscrew::destroy()
	{
		NewtonDestroyJoint(world, joint);
	}

	// joint-specific functions
	dFloat Roket_PhysicsJointCorkscrew::getAngle()
	{
		return NewtonCorkscrewGetJointAngle(joint);
	}

	dFloat Roket_PhysicsJointCorkscrew::getOmega()
	{
		return NewtonCorkscrewGetJointOmega(joint);
	}

	dFloat Roket_PhysicsJointCorkscrew::getPosition()
	{
		return NewtonCorkscrewGetJointPosit(joint);
	}

	dFloat Roket_PhysicsJointCorkscrew::getVelocity()
	{
		return NewtonCorkscrewGetJointVeloc(joint);
	}

	irr::core::vector3df Roket_PhysicsJointCorkscrew::getForce()
	{
		float angle[3];
		NewtonCorkscrewGetJointForce(joint,&angle[0]);
		return irr::core::vector3df(angle[0],angle[1],angle[2]);
	}
}
