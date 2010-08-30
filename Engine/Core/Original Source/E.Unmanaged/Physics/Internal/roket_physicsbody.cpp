#include "roket3d.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsbody.h"
#include "E.Unmanaged/Physics/Internal/roket_physicscallbacks.h"

namespace physics
{
	Roket_PhysicsBody::Roket_PhysicsBody(NewtonCollision* collision, NewtonWorld* physworld, ePhysicsType type, int materialid)
	{
		body = NewtonCreateBody(physworld,collision);
		NewtonReleaseCollision(physworld,collision);
		NewtonBodySetUserData(body,this);
		NewtonBodySetTransformCallback(body,physics::TransformCallback);
		NewtonBodySetForceAndTorqueCallback(body,physics::ApplyForceAndTorqueCallback);
		affectedByGravity = true;
		objectExists = true;

		// apply initial force
		/*float omega[3];
		omega[0] = 10.0f;
		omega[1] = 10.0f;
		omega[2] = 10.0f;
		NewtonBodySetOmega (body, &omega[0]);*/

		/*
		float force[3];
		force[0] = 0.0f;
		force[1] = -9.8f;
		force[2] = 0.0f;
		NewtonBodyAddForce( body, &force[0] );
		*/

		world = physworld;
	}
	
	Roket_PhysicsBody::~Roket_PhysicsBody()
	{
		cout << "Attempting to destroy physics body...";
		if (objectExists)
		{
			cout << " Destroyed." << endl;
			NewtonDestroyBody(world,body);
			objectExists = false;
		}
		else
			cout << " Already Destroyed." << endl;
	}
	
	void Roket_PhysicsBody::setPosition(irr::core::vector3df vec)
	{
		irr::core::matrix4 matrix;
		NewtonBodyGetMatrix(body, matrix.pointer());
		matrix.setTranslation(vec);
		NewtonBodySetMatrix(body, matrix.pointer());
	}

	void Roket_PhysicsBody::setRotation(irr::core::vector3df vec)
	{
		irr::core::matrix4 matrix;
		NewtonBodyGetMatrix(body, matrix.pointer());
		matrix.setRotationDegrees(vec);
		NewtonBodySetMatrix(body, matrix.pointer());
	}

	// ############## Physics functions ##################
	void Roket_PhysicsBody::setPhysicsSettings(dFloat mass, irr::core::vector3df inertia)
	{
		NewtonBodySetMassMatrix(body, mass, inertia.X, inertia.Y, inertia.Z);
	}

	dFloat Roket_PhysicsBody::getPhysicsMass()
	{
		dFloat mass;
		irr::core::vector3d<dFloat> inertia;
		NewtonBodyGetMassMatrix(body, &mass, &inertia.X, &inertia.Y, &inertia.Z);
		return mass;
	}

	irr::core::vector3d<dFloat> Roket_PhysicsBody::getPhysicsInertia()
	{
		dFloat mass;
		irr::core::vector3d<dFloat> inertia;
		NewtonBodyGetMassMatrix(body, &mass, &inertia.X, &inertia.Y, &inertia.Z);
		return inertia;
	}

	void Roket_PhysicsBody::attachNode(scene::RSceneNode* nn)
	{
		node = nn;
	}

	// ################ PHYSICS UPDATE FUNCTIONS #######################
	void Roket_PhysicsBody::updateSelf(const dFloat* matrix_old)
	{
		irr::core::matrix4 mat;
		NewtonBodyGetMatrix(body, mat.pointer());
		//for (int i=0;i<16;i+=1)
		//	mat[i] = matrix[i];
		//mat.pointer() = matrix;
		irr::core::vector3df pos = mat.getTranslation();
		irr::core::vector3df rot = mat.getRotationDegrees();
		// outputs too much :P
		//nonlua_debug("Physics body located at: " << pos.X << "," << pos.Y << "," << pos.Z << ".",LVL_INFO);
		//nonlua_debug("Physics body rotated by: " << rot.X << "," << rot.Y << "," << rot.Z << ".",LVL_INFO);
		if (node->isNodeSet)
		{
			node->self()->setPosition(pos);
			node->self()->setRotation(rot);
			irr::SEvent myevent;
			myevent.EventType = irr::EET_ROKET_EVENT;
			myevent.Roket3DEvent.subType = "Physics";
			myevent.Roket3DEvent.additionalInfo = "Transform";
			myevent.Roket3DEvent.userData = reinterpret_cast<void*>(node);
			node->roketdevice->getIrrlichtDevice()->postEventFromUser(myevent);
		}
		else
		{
			// clean up time
			this->deletePhysics();
			delete this;
		}
		/*}
		else
		{
			nonlua_debug("Internal inconsistancy detected.  Don't overwrite variables that hold scene nodes.",LVL_WARN);
			delete this;
		}*/
		//node->self()->setScale(mat.getScale());
	}

	void Roket_PhysicsBody::updateForce()
	{
		if (affectedByGravity)
		{
			float force[3];
			force[0] = 0.0f;
			force[1] = -9.8f;
			force[2] = 0.0f;
			NewtonBodyAddForce( body, &force[0] );
			//nonlua_debug("Applying gravity.",LVL_INFO);
		}

		// apply any forces we need to
		for (int i = 0;i < forcesToApply.size(); i++)
		{
			float force[3];
			force[0] = forcesToApply[i].X;
			force[1] = forcesToApply[i].Y;
			force[2] = forcesToApply[i].Z;
			NewtonBodyAddForce( body, &force[0] );
			// outputs too much :P
			//nonlua_debug("Applying force " << i << ".",LVL_INFO);
		}
		forcesToApply.clear();
	}

	void Roket_PhysicsBody::applyForce(irr::core::vector3df force)
	{
		irr::core::vector3df f2(force);
		forcesToApply.push_back(f2);
		nonlua_debug("Force is loaded into stack.",LVL_INFO);
	}

	void Roket_PhysicsBody::updateTorque()
	{
		// apply any torque we need to
		for (int i = 0;i < torqueToApply.size(); i++)
		{
			float force[3];
			force[0] = torqueToApply[i].X;
			force[1] = torqueToApply[i].Y;
			force[2] = torqueToApply[i].Z;
			NewtonBodyAddTorque( body, &force[0] );
			// outputs too much :P
			//nonlua_debug("Applying torque " << i << ".",LVL_INFO);
		}
		torqueToApply.clear();
	}

	void Roket_PhysicsBody::applyTorque(irr::core::vector3df torque)
	{
		irr::core::vector3df t2(torque);
		torqueToApply.push_back(t2);
		nonlua_debug("Torque is loaded into stack.",LVL_INFO);
	}

	void Roket_PhysicsBody::deletePhysics()
	{
		cout << "(deletePhysics) Attempting to destroy physics body...";
		if (objectExists)
		{
			cout << " Destroyed." << endl;
			NewtonDestroyBody(world,body);
			objectExists = false;
		}
		else
			cout << " Already Destroyed." << endl;
	}
}
