#include "roket3d.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsjointball.h"
#include "E.Unmanaged/Physics/Internal/roket_physicscallbacks.h"

namespace physics
{
	Roket_PhysicsJointBall::Roket_PhysicsJointBall(
		NewtonWorld* worldd,
		irr::core::vector3df point,
		Roket_PhysicsBody* child,
		Roket_PhysicsBody* parent,
		RPhysicsJoint* rjoint)
	{
		world = worldd;
		float pointD[3];
		pointD[0] = point.X;
		pointD[1] = point.Y;
		pointD[2] = point.Z;
		if (parent == NULL)
			joint = NewtonConstraintCreateBall(world, &pointD[0], child->body, NULL);
		else
			joint = NewtonConstraintCreateBall(world, &pointD[0], child->body, parent->body);
		NewtonJointSetUserData(joint, &rjoint);
		NewtonJointSetDestructor(joint, &JointDestructor);
	}

	Roket_PhysicsJointBall::~Roket_PhysicsJointBall()
	{
		// We should not enable this as it will cause seg faults :(
		//NewtonDestroyJoint(world, joint);
	}

	dFloat Roket_PhysicsJointBall::getStiffness()
	{
		return NewtonJointGetStiffness(joint);
	}

	void Roket_PhysicsJointBall::setStiffness(dFloat stiffness)
	{
		NewtonJointSetStiffness(joint,stiffness);
	}

	void Roket_PhysicsJointBall::destroy()
	{
		NewtonDestroyJoint(world, joint);
	}

	// joint-specific functions
	irr::core::vector3df Roket_PhysicsJointBall::getAngle()
	{
		float angle[3];
		NewtonBallGetJointAngle(joint,&angle[0]);
		return irr::core::vector3df(angle[0],angle[1],angle[2]);
	}

	irr::core::vector3df Roket_PhysicsJointBall::getOmega()
	{
		float angle[3];
		NewtonBallGetJointOmega(joint,&angle[0]);
		return irr::core::vector3df(angle[0],angle[1],angle[2]);
	}

	irr::core::vector3df Roket_PhysicsJointBall::getForce()
	{
		float angle[3];
		NewtonBallGetJointForce(joint,&angle[0]);
		return irr::core::vector3df(angle[0],angle[1],angle[2]);
	}

	void Roket_PhysicsJointBall::setLimits(irr::core::vector3df conePin, double maxConeAngle, double maxTwistAngle)
	{
		float angle[3];
		angle[0] = conePin.X;
		angle[1] = conePin.Y;
		angle[2] = conePin.Z;
		NewtonBallSetConeLimits(joint,&angle[0],maxConeAngle,maxTwistAngle);
	}
}
