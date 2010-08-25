#include "roket3d.h"
#include "roket_physicsjointslider.h"
#include "roket_physicscallbacks.h"

namespace physics
{
	Roket_PhysicsJointSlider::Roket_PhysicsJointSlider(
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
			joint = NewtonConstraintCreateSlider(world, &pointD[0], &pointP[0], child->body, NULL);
		else
			joint = NewtonConstraintCreateSlider(world, &pointD[0], &pointP[0], child->body, parent->body);
		NewtonJointSetUserData(joint, &rjoint);
		NewtonJointSetDestructor(joint, &JointDestructor);
	}

	Roket_PhysicsJointSlider::~Roket_PhysicsJointSlider()
	{
		//NewtonDestroyJoint(world, joint);
	}

	dFloat Roket_PhysicsJointSlider::getStiffness()
	{
		return NewtonJointGetStiffness(joint);
	}

	void Roket_PhysicsJointSlider::setStiffness(dFloat stiffness)
	{
		NewtonJointSetStiffness(joint,stiffness);
	}

	void Roket_PhysicsJointSlider::destroy()
	{
		NewtonDestroyJoint(world, joint);
	}

	// joint-specific functions
	dFloat Roket_PhysicsJointSlider::getPosition()
	{
		return NewtonSliderGetJointPosit(joint);
	}

	dFloat Roket_PhysicsJointSlider::getVelocity()
	{
		return NewtonSliderGetJointVeloc(joint);
	}

	irr::core::vector3df Roket_PhysicsJointSlider::getForce()
	{
		float angle[3];
		NewtonSliderGetJointForce(joint,&angle[0]);
		return irr::core::vector3df(angle[0],angle[1],angle[2]);
	}
}
