#include "roket3d.h"
#include "rphysicsjoint.h"

namespace physics
{
	const char *RPhysicsJoint::className = "RPhysicsJoint";
	const Luna < RPhysicsJoint >::FunctionType RPhysicsJoint::Functions[] = {
		{"getType", &RPhysicsJoint::getType},
		{"getStiffness", &RPhysicsJoint::getStiffness},
		{"setStiffness", &RPhysicsJoint::setStiffness},
		{"destroy", &RPhysicsJoint::destroy},
		// ball object
		{"setLimits", &RPhysicsJoint::setLimits},
		{"getForce", &RPhysicsJoint::getForce},
		{"getAngle", &RPhysicsJoint::getAngle},
		{"getOmega", &RPhysicsJoint::getOmega},
		{0}
	};
	const Luna < RPhysicsJoint >::PropertyType RPhysicsJoint::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RPhysicsJoint::RPhysicsJoint(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RPhysicsJoints.  You must collect one from an RDevice.",LVL_ERR,L);
		jointType = E_JOINT_NONE;
	}
	
	// Returns the type of joint.
	//  _JOINT_BALL_AND_SOCKET
	//  _JOINT_BALL (same as _JOINT_BALL_AND_SOCKET)
	//  _JOINT_HINGE
	//  _JOINT_SLIDER
	//  _JOINT_CORKSCREW
	//  _JOINT_UNIVERSAL
	int RPhysicsJoint::getType(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,jointType);
		return 1;
		
		endifisvalid();
	}

	// C++ version
	ePhysicsJointType RPhysicsJoint::getType()
	{
		return jointType;
	}
	
	int RPhysicsJoint::setObjectBallAndSocket(Roket_PhysicsJointBall* object)
	{
		obj_ball=object;
		objectExists=true;
		jointType = E_JOINT_BALL_AND_SOCKET;
		return 0;
	}

	int RPhysicsJoint::setObjectHinge(Roket_PhysicsJointHinge* object)
	{
		obj_hinge=object;
		objectExists=true;
		jointType = E_JOINT_HINGE;
		return 0;
	}

	int RPhysicsJoint::setObjectSlider(Roket_PhysicsJointSlider* object)
	{
		obj_slider=object;
		objectExists=true;
		jointType = E_JOINT_SLIDER;
		return 0;
	}

	int RPhysicsJoint::setObjectCorkscrew(Roket_PhysicsJointCorkscrew* object)
	{
		obj_corkscrew=object;
		objectExists=true;
		jointType = E_JOINT_CORKSCREW;
		return 0;
	}

	int RPhysicsJoint::setObjectUniversal(Roket_PhysicsJointUniversal* object)
	{
		obj_universal=object;
		objectExists=true;
		jointType = E_JOINT_UNIVERSAL;
		return 0;
	}
	
	Roket_PhysicsJointBall* RPhysicsJoint::getObjectBallAndSocket()
	{
		if (!objectExists)
			return NULL;

		if (jointType == E_JOINT_BALL_AND_SOCKET)
			return obj_ball;
		else
			return NULL;
	}

	Roket_PhysicsJointHinge* RPhysicsJoint::getObjectHinge()
	{
		if (!objectExists)
			return NULL;

		if (jointType == E_JOINT_HINGE)
			return obj_hinge;
		else
			return NULL;
	}

	Roket_PhysicsJointSlider* RPhysicsJoint::getObjectSlider()
	{
		if (!objectExists)
			return NULL;

		if (jointType == E_JOINT_SLIDER)
			return obj_slider;
		else
			return NULL;
	}

	Roket_PhysicsJointCorkscrew* RPhysicsJoint::getObjectCorkscrew()
	{
		if (!objectExists)
			return NULL;

		if (jointType == E_JOINT_CORKSCREW)
			return obj_corkscrew;
		else
			return NULL;
	}

	Roket_PhysicsJointUniversal* RPhysicsJoint::getObjectUniversal()
	{
		if (!objectExists)
			return NULL;

		if (jointType == E_JOINT_UNIVERSAL)
			return obj_universal;
		else
			return NULL;
	}

	int RPhysicsJoint::getStiffness(lua_State* L)
	{
		ifistrue(objectExists);
		
		switch (jointType)
		{
			case E_JOINT_BALL_AND_SOCKET:	lua_pushnumber(L,obj_ball->getStiffness()); break;
			case E_JOINT_HINGE:				lua_pushnumber(L,obj_hinge->getStiffness()); break;
			case E_JOINT_SLIDER:			lua_pushnumber(L,obj_slider->getStiffness()); break;
			case E_JOINT_CORKSCREW:			lua_pushnumber(L,obj_corkscrew->getStiffness()); break;
			case E_JOINT_UNIVERSAL:			lua_pushnumber(L,obj_universal->getStiffness()); break;
			default:						lua_pushnil(L); break;
		}
		return 1;
		
		endifisvalid();
	}

	int RPhysicsJoint::destroy(lua_State* L)
	{
		ifistrue(objectExists);
		
		// We don't delete the obj_**** here because it will cause a seg fault.  It will be cleaned up
		// by the destructor callback, which in turn calls setInvalid (below) to clean it up.
		switch (jointType)
		{
			case E_JOINT_BALL_AND_SOCKET:	obj_ball->destroy(); break;
			case E_JOINT_HINGE:				obj_hinge->destroy(); break;
			case E_JOINT_SLIDER:			obj_slider->destroy(); break;
			case E_JOINT_CORKSCREW:			obj_corkscrew->destroy(); break;
			case E_JOINT_UNIVERSAL:			obj_universal->destroy(); break;
		}
		objectExists = false;
		return 0;
		
		endifisvalid();
	}

	int RPhysicsJoint::setStiffness(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber stiffness = getmandatoryargument_cNumber(L,1);
		switch (jointType)
		{
			case E_JOINT_BALL_AND_SOCKET:	obj_ball->setStiffness(stiffness); break;
			case E_JOINT_HINGE:				obj_hinge->setStiffness(stiffness); break;
			case E_JOINT_SLIDER:			obj_slider->setStiffness(stiffness); break;
			case E_JOINT_CORKSCREW:			obj_corkscrew->setStiffness(stiffness); break;
			case E_JOINT_UNIVERSAL:			obj_universal->setStiffness(stiffness); break;
		}
		return 0;
		
		endifisvalid();
	}

	void RPhysicsJoint::setInvalid()
	{
		objectExists = false;
		switch (jointType)
		{
			case E_JOINT_BALL_AND_SOCKET:	delete obj_ball; break;
			case E_JOINT_HINGE:				delete obj_hinge; break;
			case E_JOINT_SLIDER:			delete obj_slider; break;
			case E_JOINT_CORKSCREW:			delete obj_corkscrew; break;
			case E_JOINT_UNIVERSAL:			delete obj_universal; break;
		}
	}

	// Functions for ball object
	int RPhysicsJoint::setLimits(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (jointType == E_JOINT_BALL_AND_SOCKET)
		{
			// Ball
			core::RVector3D* rball = Luna<core::RVector3D>::check(L,1);
			cNumber maxConeAngle = getmandatoryargument_cNumber(L,2);
			cNumber maxTwistAngle = getmandatoryargument_cNumber(L,3);
			obj_ball->setLimits(rball->getObject(), maxConeAngle, maxTwistAngle);
		}
		else
		{
			// Incorrect Type, Warn
			debug("Operation not valid on this type of joint.",LVL_WARN,L);
		}
		return 0;
		
		endifisvalid();
	}

	int RPhysicsJoint::getAngle(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (jointType == E_JOINT_BALL_AND_SOCKET)
		{
			// Ball
			core::RVector3D* ret = Luna<core::RVector3D>::createNew(L);
			irr::core::vector3df irr_ret = obj_ball->getAngle();
			ret->setObject(irr_ret);
			return 1;
		}
		else if (jointType == E_JOINT_HINGE)
		{
			// Hinge
			lua_pushnumber(L,obj_hinge->getAngle());
			return 1;
		}
		else if (jointType == E_JOINT_CORKSCREW)
		{
			// Hinge
			lua_pushnumber(L,obj_corkscrew->getAngle());
			return 1;
		}
		else if (jointType == E_JOINT_UNIVERSAL)
		{
			// Universal
			int type = getmandatoryargument_cNumber(L,1);
			if (type == 1)
				lua_pushnumber(L,obj_universal->getAngle0());
			else if (type == 2)
				lua_pushnumber(L,obj_universal->getAngle1());
			else
			{
				debug("Invalid angle type specified. (You must provide 1 or 2).",LVL_ERR,L);
				return 0;
			}
			return 1;
		}
		else
		{
			// Incorrect Type, Warn
			debug("Operation not valid on this type of joint.",LVL_WARN,L);
			return 0;
		}
		
		endifisvalid();
	}

	int RPhysicsJoint::getOmega(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (jointType == E_JOINT_BALL_AND_SOCKET)
		{
			// Ball
			core::RVector3D* ret = Luna<core::RVector3D>::createNew(L);
			irr::core::vector3df irr_ret = obj_ball->getOmega();
			ret->setObject(irr_ret);
			return 1;
		}
		else if (jointType == E_JOINT_HINGE)
		{
			// Hinge
			lua_pushnumber(L,obj_hinge->getOmega());
			return 1;
		}
		else if (jointType == E_JOINT_CORKSCREW)
		{
			// Hinge
			lua_pushnumber(L,obj_corkscrew->getOmega());
			return 1;
		}
		else if (jointType == E_JOINT_UNIVERSAL)
		{
			// Universal
			int type = getmandatoryargument_cNumber(L,1);
			if (type == 1)
				lua_pushnumber(L,obj_universal->getOmega0());
			else if (type == 2)
				lua_pushnumber(L,obj_universal->getOmega1());
			else
			{
				debug("Invalid omega (angular force) type specified. (You must provide 1 or 2).",LVL_ERR,L);
				return 0;
			}
			return 1;
		}
		else
		{
			// Incorrect Type, Warn
			debug("Operation not valid on this type of joint.",LVL_WARN,L);
			return 0;
		}
		
		endifisvalid();
	}

	int RPhysicsJoint::getPosition(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (jointType == E_JOINT_SLIDER)
		{
			// Hinge
			lua_pushnumber(L,obj_slider->getPosition());
			return 1;
		}
		else if (jointType == E_JOINT_CORKSCREW)
		{
			// Hinge
			lua_pushnumber(L,obj_corkscrew->getPosition());
			return 1;
		}
		else
		{
			// Incorrect Type, Warn
			debug("Operation not valid on this type of joint.",LVL_WARN,L);
			return 0;
		}
		
		endifisvalid();
	}

	int RPhysicsJoint::getVelocity(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (jointType == E_JOINT_SLIDER)
		{
			// Hinge
			lua_pushnumber(L,obj_slider->getVelocity());
			return 1;
		}
		else if (jointType == E_JOINT_CORKSCREW)
		{
			// Hinge
			lua_pushnumber(L,obj_corkscrew->getVelocity());
			return 1;
		}
		else
		{
			// Incorrect Type, Warn
			debug("Operation not valid on this type of joint.",LVL_WARN,L);
			return 0;
		}
		
		endifisvalid();
	}

	int RPhysicsJoint::getForce(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (jointType == E_JOINT_BALL_AND_SOCKET)
		{
			// Ball
			core::RVector3D* ret = Luna<core::RVector3D>::createNew(L);
			irr::core::vector3df irr_ret = obj_ball->getForce();
			ret->setObject(irr_ret);
			return 1;
		}
		else if (jointType == E_JOINT_HINGE)
		{
			// Hinge
			core::RVector3D* ret = Luna<core::RVector3D>::createNew(L);
			irr::core::vector3df irr_ret = obj_hinge->getForce();
			ret->setObject(irr_ret);
			return 1;
		}
		else if (jointType == E_JOINT_SLIDER)
		{
			// Slider
			core::RVector3D* ret = Luna<core::RVector3D>::createNew(L);
			irr::core::vector3df irr_ret = obj_slider->getForce();
			ret->setObject(irr_ret);
			return 1;
		}
		else if (jointType == E_JOINT_CORKSCREW)
		{
			// Slider
			core::RVector3D* ret = Luna<core::RVector3D>::createNew(L);
			irr::core::vector3df irr_ret = obj_corkscrew->getForce();
			ret->setObject(irr_ret);
			return 1;
		}
		else if (jointType == E_JOINT_UNIVERSAL)
		{
			// Slider
			core::RVector3D* ret = Luna<core::RVector3D>::createNew(L);
			irr::core::vector3df irr_ret = obj_universal->getForce();
			ret->setObject(irr_ret);
			return 1;
		}
		else
		{
			// Incorrect Type, Warn
			debug("Operation not valid on this type of joint.",LVL_WARN,L);
			return 0;
		}
		
		endifisvalid();
	}
}
