#include "roket3d.h"
#include "E.Collections/rquaternion.h"

namespace core
{
	const char *RQuaternion::className = TYPE_QUATERNION;
	const Luna < RQuaternion >::FunctionType RQuaternion::Functions[] = {
		{"dotProduct", &RQuaternion::dotProduct},
  {"fromAngleAxis", &RQuaternion::fromAngleAxis},
  {"makeIdentity", &RQuaternion::makeIdentity},
  {"makeInverse", &RQuaternion::makeInverse},
  {"normalize", &RQuaternion::normalize},
  {"rotationFromTo", &RQuaternion::rotationFromTo},
  {"slerp", &RQuaternion::slerp},
  {"toAngleAxis", &RQuaternion::toAngleAxis},
  {"toEuler", &RQuaternion::toEuler},

//  {"copyFrom", &RQuaternion::copyFrom},
//  {"set", &RQuaternion::set},
//  {"setEuler", &RQuaternion::setEuler},
//  {"setVector", &RQuaternion::setVector},

  // metaoperators
  {"__eq", &RQuaternion::_equals},
  {"__mul", &RQuaternion::_multiply},
  {"__add", &RQuaternion::_add},
  
  {0}
	};
	const Luna < RQuaternion >::PropertyType RQuaternion::Properties[] = {
		{"W", &RQuaternion::getW, &RQuaternion::setW},
  {"X", &RQuaternion::getX, &RQuaternion::setX},
  {"Y", &RQuaternion::getY, &RQuaternion::setY},
  {"Z", &RQuaternion::getZ, &RQuaternion::setZ},
  {0}
	};
	
	RQuaternion::RQuaternion(lua_State* L, bool instantedByUser)
	{
		// RQuaternion can be initalized in different forms:
		//
		//  RQuaternion();
		//  RQuaternion(w,x,y,z);
		//
		if (lua_isnil(L,1))
		{
			// Form 1
			irr_quaternion = irr::core::quaternion(0,0,0,0);
			objectExists = true;
		}
		else
		{
			// Form 2 
			
			// Get values
			cNumber Wp = getmandatoryargument_cNumber(L,0);
			cNumber Xp = getmandatoryargument_cNumber(L,1);
			cNumber Yp = getmandatoryargument_cNumber(L,2);
			cNumber Zp = getmandatoryargument_cNumber(L,3);
		
			W = Wp;
			X = Xp;
			Y = Yp;
			Z = Zp;

			irr_quaternion = irr::core::quaternion(X,Y,Z,W);
			objectExists = true;
		}
	}
	
	int RQuaternion::setObject(irr::core::quaternion object)
	{
		irr_quaternion = object;
		objectExists = true;
		return 0;
	}
	
	irr::core::quaternion RQuaternion::getObject()
	{
		if (objectExists)
			return irr_quaternion;
		else
			return irr::core::quaternion(0,0,0,0);
	}
	
	int RQuaternion::dotProduct(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the quaternion
		core::RQuaternion *otherquat = Luna<core::RQuaternion>::check(L, 1);
		
		// Return dot product
		cNumber result = irr_quaternion.dotProduct(otherquat->getObject());
		lua_pushnumber(L,result);

		return 1;
		
		endifisvalid();
	}

	int RQuaternion::fromAngleAxis(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get angle
		cNumber angle = getmandatoryargument_cNumber(L,1);

		// Get vector
		core::RVector3D *vec = Luna<core::RVector3D>::check(L, 1);

		// Return quaternion
		RQuaternion* returnquat = Luna < RQuaternion >::createNew(L);
		irr::core::quaternion quat = irr_quaternion.fromAngleAxis(angle,vec->getObject());
		returnquat->setObject(quat);

		return 1;
		
		endifisvalid();
	}

	int RQuaternion::makeIdentity(lua_State* L)
	{
		ifistrue(objectExists);

		// Return quaternion
		RQuaternion* returnquat = Luna < RQuaternion >::createNew(L);
		irr::core::quaternion quat = irr_quaternion.makeIdentity();
		returnquat->setObject(quat);

		return 1;
		
		endifisvalid();
	}

	int RQuaternion::makeInverse(lua_State* L)
	{
		ifistrue(objectExists);

		// Return quaternion
		RQuaternion* returnquat = Luna < RQuaternion >::createNew(L);
		irr::core::quaternion quat = irr_quaternion.makeInverse();
		returnquat->setObject(quat);

		return 1;
		
		endifisvalid();
	}

	int RQuaternion::normalize(lua_State* L)
	{
		ifistrue(objectExists);

		// Return quaternion
		RQuaternion* returnquat = Luna < RQuaternion >::createNew(L);
		irr::core::quaternion quat = irr_quaternion.normalize();
		returnquat->setObject(quat);

		return 1;
		
		endifisvalid();
	}

	int RQuaternion::rotationFromTo(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get from vector
		core::RVector3D *fromvec = Luna<core::RVector3D>::check(L, 1);

		// Get to vector
		core::RVector3D *tovec = Luna<core::RVector3D>::check(L, 2);

		// Return quaternion
		RQuaternion* returnquat = Luna < RQuaternion >::createNew(L);
		irr::core::quaternion quat = irr_quaternion.rotationFromTo(fromvec->getObject(),tovec->getObject());
		returnquat->setObject(quat);

		return 1;
		
		endifisvalid();
	}

	int RQuaternion::slerp(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get quaternion 1
		core::RQuaternion *quat1 = Luna<core::RQuaternion>::check(L, 1);

		// Get quaternion 2
		core::RQuaternion *quat2 = Luna<core::RQuaternion>::check(L, 2);

		// Get interpolation
		cNumber interpolate = getmandatoryargument_cNumber(L,3);	

		// Return quaternion (interpolation)
		RQuaternion* returnquat = Luna < RQuaternion >::createNew(L);
		irr::core::quaternion quat = irr_quaternion.slerp(quat1->getObject(), quat2->getObject(), interpolate);
		returnquat->setObject(quat);

		return 1;
		
		endifisvalid();
	}

	int RQuaternion::toAngleAxis(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get angle
		irr::f32 angle = getmandatoryargument_cNumber(L,1);

		// Get vector
		core::RVector3D *vec = Luna<core::RVector3D>::check(L, 2);

		// Call function
		RVector3D* resultvec = Luna < RVector3D >::createNew(L);
		irr::core::vector3df resultivec = vec->getObject();
		irr_quaternion.toAngleAxis(angle, resultivec);
		resultvec->setObject(resultivec);
		return 1;
		
		endifisvalid();
	}

	int RQuaternion::toEuler(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get vector
		core::RVector3D *vec = Luna<core::RVector3D>::check(L, 1);

		// Call function
		RVector3D* resultvec = Luna < RVector3D >::createNew(L);
		irr::core::vector3df resultivec = vec->getObject();
		irr_quaternion.toEuler(resultivec);
		resultvec->setObject(resultivec);
		return 1;
		
		endifisvalid();
	}

	int RQuaternion::_equals(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RQuaternion *A = Luna<core::RQuaternion>::check(L, 1);
		
		// Get B
		core::RQuaternion *B = Luna<core::RQuaternion>::check(L, 2);
		
		// Return result
		lua_pushboolean(L,(A->getObject()==B->getObject()));
		return 1;
		
		endifisvalid();
	}

	int RQuaternion::_multiply(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RQuaternion *A = Luna<core::RQuaternion>::check(L, 1);
		
		// Get B
		core::RQuaternion *B = Luna<core::RQuaternion>::check(L, 2);
		
		// Return result
		RQuaternion* returnquat = Luna < RQuaternion >::createNew(L);
		irr::core::quaternion quat = (A->getObject()) * (B->getObject());
		returnquat->setObject(quat);
		return 1;
		
		endifisvalid();
	}

	int RQuaternion::_add(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RQuaternion *A = Luna<core::RQuaternion>::check(L, 1);
		
		// Get B
		core::RQuaternion *B = Luna<core::RQuaternion>::check(L, 2);
		
		// Return result
		RQuaternion* returnquat = Luna < RQuaternion >::createNew(L);
		irr::core::quaternion quat = (A->getObject()) + (B->getObject());
		returnquat->setObject(quat);
		return 1;
		
		endifisvalid();
	}

	int RQuaternion::setW(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber p = lua_tonumber(L, -1);
		
		W = p;
		irr_quaternion.W = p;
		return 0;
		
		endifisvalid();
	}

	int RQuaternion::getW(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_quaternion.W,(irr::f32)W))
			W = irr_quaternion.W;
		lua_pushnumber(L,W);
		return 1;

		endifisvalid();
	}

	int RQuaternion::setX(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber p = lua_tonumber(L, -1);
		
		X = p;
		irr_quaternion.X = p;
		return 0;

		endifisvalid();
	}

	int RQuaternion::getX(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_quaternion.X,(irr::f32)X))
			X = irr_quaternion.X;
		lua_pushnumber(L,X);
		return 1;

		endifisvalid();
	}

	int RQuaternion::setY(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber p = lua_tonumber(L, -1);
		
		Y = p;
		irr_quaternion.Y = p;
		return 0;

		endifisvalid();
	}

	int RQuaternion::getY(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_quaternion.Y,(irr::f32)Y))
			Y = irr_quaternion.Y;
		lua_pushnumber(L,Y);
		return 1;

		endifisvalid();
	}

	int RQuaternion::setZ(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber p = lua_tonumber(L, -1);

		Z = p;
		irr_quaternion.Z = p;
		return 0;

		endifisvalid();
	}

	int RQuaternion::getZ(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_quaternion.Z,(irr::f32)Z))
			Z = irr_quaternion.Z;
		lua_pushnumber(L,Z);
		return 1;

		endifisvalid();
	}
	
}
