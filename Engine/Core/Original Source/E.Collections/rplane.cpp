#include "roket3d.h"
#include "E.Collections/rplane.h"

namespace core
{
	const char *RPlane::className = TYPE_3DPLANE;
	const Luna < RPlane >::FunctionType RPlane::Functions[] = {
		{"classifyPointRelation", &RPlane::classifyPointRelation},
  {"existsIntersection", &RPlane::existsIntersection},
  {"getDistanceTo", &RPlane::getDistanceTo},
  {"getIntersectionWithLimitedLine", &RPlane::getIntersectionWithLimitedLine},
  {"getIntersectionWithLine", &RPlane::getIntersectionWithLine},
  {"getIntersectionWithPlane", &RPlane::getIntersectionWithPlane},
  {"getIntersectionWithPlanes", &RPlane::getIntersectionWithPlanes},
  {"getKnownIntersectionWithLine", &RPlane::getKnownIntersectionWithLine},
  {"getMemberPoint", &RPlane::getMemberPoint},
  {"isFrontFacing", &RPlane::isFrontFacing},
  {"recalculateD", &RPlane::recalculateD},

  {"copyFrom", &RPlane::copyFrom},
  {"set", &RPlane::set},

  // metaoperators
  {"__eq", &RPlane::_equals},
  
  {0}
	};
	const Luna < RPlane >::PropertyType RPlane::Properties[] = {
		{"D", &RPlane::getD, &RPlane::setD},
		{"Normal", &RPlane::getNormal, &RPlane::setNormal},
  {0}
	};
	
	RPlane::RPlane(lua_State* L, bool instantedByUser)
	{
		// RPlane can be initalized in different forms:
		//
		//  RPlane();
		//  RPlane(normal,distance);
		//
		if (lua_isnil(L,1))
		{
			// Form 1
			irr_plane = irr::core::plane3df();
			objectExists = true;
		}
		else
		{
			// Form 2
			
			// Get normal
			core::RVector3D *pN = Luna<core::RVector3D>::check(L, 0);
			
			// Get distance
			cNumber dist = getmandatoryargument_cNumber(L,1);
		
			irr_plane = irr::core::plane3df();
			irr_plane.Normal = pN->getObject();
			irr_plane.D = dist;
			//irr::core::plane3df(,dist);
			objectExists = true;
		}
	}
	
	int RPlane::setObject(irr::core::plane3df object)
	{
		irr_plane = object;
		objectExists = true;
		return 0;
	}
	
	irr::core::plane3df RPlane::getObject()
	{
		if (objectExists)
			return irr_plane;
		else
			return irr::core::plane3df();
	}
	
	int RPlane::classifyPointRelation(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *vec = Luna<core::RVector3D>::check(L, 1);
		
		// Return closest point
		cNumber result = irr_plane.classifyPointRelation(vec->getObject());
		lua_pushnumber(L,result);

		return 1;
		
		endifisvalid();
	}

	int RPlane::existsIntersection(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RPlane *otherplane = Luna<core::RPlane>::check(L, 1);
		
		// Return closest point
		bool result = irr_plane.existsIntersection(otherplane->getObject());
		lua_pushboolean(L,result);

		return 1;
		
		endifisvalid();
	}

	int RPlane::getDistanceTo(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *vec = Luna<core::RVector3D>::check(L, 1);
		
		// Return closest point
		cNumber result = irr_plane.getDistanceTo(vec->getObject());
		lua_pushnumber(L,result);

		return 1;
		
		endifisvalid();
	}
	
	int RPlane::getIntersectionWithLimitedLine(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line point
		core::RRay3D *origin = Luna<core::RRay3D>::check(L, 1);
		
		// Create the storage variable
		cIrrVector3D collision = cIrrVector3D();
		
		// Check intersection
		bool result = irr_plane.getIntersectionWithLimitedLine(origin->getObject().start,origin->getObject().end,collision);
		
		lua_pushboolean(L,result);
		
		if (result)
		{
			RVector3D* returnvector = Luna < RVector3D >::createNew(L);
			returnvector->setObject(collision);
		}
		else
		{
			lua_pushnil(L);
		}
		
		return 2;
		endifisvalid();
	}
	
	int RPlane::getIntersectionWithLine(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line point
		core::RVector3D *origin = Luna<core::RVector3D>::check(L, 1);
		
		// Get the line direction
		core::RVector3D *direction = Luna<core::RVector3D>::check(L, 2);
		
		// Create the storage variable
		cIrrVector3D collision = cIrrVector3D();
		
		// Check intersection
		bool result = irr_plane.getIntersectionWithLine(origin->getObject(),direction->getObject(),collision);
		
		lua_pushboolean(L,result);
		
		if (result)
		{
			RVector3D* returnvector = Luna < RVector3D >::createNew(L);
			returnvector->setObject(collision);
		}
		else
		{
			lua_pushnil(L);
		}
		return 2;
		
		endifisvalid();
	}

	int RPlane::getIntersectionWithPlane(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line point
		core::RPlane *otherplane = Luna<core::RPlane>::check(L, 1);
		
		// Create the storage variable
		cIrrVector3D collisionp = cIrrVector3D();
		cIrrVector3D collisionv = cIrrVector3D();
		
		// Check intersection
		bool result = irr_plane.getIntersectionWithPlane(otherplane->getObject(),collisionp,collisionv);
		
		lua_pushboolean(L,result);
		
		if (result)
		{
			RVector3D* returnvector = Luna < RVector3D >::createNew(L);
			returnvector->setObject(collisionp);
			RVector3D* returnvector2 = Luna < RVector3D >::createNew(L);
			returnvector->setObject(collisionv);
		}
		else
		{
			lua_pushnil(L);
			lua_pushnil(L);
		}
		return 3;
		
		endifisvalid();
	}
	
	int RPlane::isFrontFacing(lua_State* L)
	{
		ifistrue(objectExists);

		// get comparison point
		core::RVector3D *compare = Luna<core::RVector3D>::check(L, 1);

		lua_pushboolean(L,irr_plane.isFrontFacing(compare->getObject()));
		return 1;

		endifisvalid();
	}
	
	int RPlane::getIntersectionWithPlanes(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line point
		core::RPlane *otherplane = Luna<core::RPlane>::check(L, 1);

		// Get the line point
		core::RPlane *otherplane2 = Luna<core::RPlane>::check(L, 2);
		
		// Create the storage variable
		cIrrVector3D collisionp = cIrrVector3D();
		
		// Check intersection
		bool result = irr_plane.getIntersectionWithPlanes(otherplane->getObject(),otherplane2->getObject(),collisionp);
		
		lua_pushboolean(L,result);
		
		if (result)
		{
			RVector3D* returnvector = Luna < RVector3D >::createNew(L);
			returnvector->setObject(collisionp);
		}
		else
		{
			lua_pushnil(L);
		}
		return 2;
		
		endifisvalid();
	}
	
	int RPlane::getKnownIntersectionWithLine(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line
		core::RRay3D *rayline = Luna<core::RRay3D>::check(L, 1);
		
		// Check intersection
		irr::f32 result = irr_plane.getKnownIntersectionWithLine(rayline->getObject().start, rayline->getObject().end);
		
		lua_pushnumber(L,result);
		return 1;
		
		endifisvalid();
	}
	
	int RPlane::getMemberPoint(lua_State* L)
	{
		ifistrue(objectExists);
		
		RVector3D* returnvector = Luna < RVector3D >::createNew(L);
		returnvector->setObject(irr_plane.getMemberPoint());
		return 1;
		
		endifisvalid();
	}
	
	int RPlane::recalculateD(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line
		core::RVector3D *vector = Luna<core::RVector3D>::check(L, 1);
		irr_plane.recalculateD(vector->getObject());
		return 0;
		
		endifisvalid();
	}

	int RPlane::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RPlane *set_plane = Luna<core::RPlane>::check(L, 1);
		
		// copy values from the other vector
		irr_plane.setPlane(set_plane->getObject().Normal,set_plane->getObject().D);
		
		return 0;
		
		endifisvalid();
	}
	
	int RPlane::set(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get normal
		core::RVector3D *pN = Luna<core::RVector3D>::check(L, 1);
			
		// Get distance
		cNumber dist = getmandatoryargument_cNumber(L,2);
			
		irr_plane = irr::core::plane3df();
		irr_plane.Normal = pN->getObject();
		irr_plane.D = dist;
		objectExists = true;
		
		endifisvalid();
	}
	
	int RPlane::_equals(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RPlane *A = Luna<core::RPlane>::check(L, 1);
		
		// Get B
		core::RPlane *B = Luna<core::RPlane>::check(L, 2);
		
		// Return result
		lua_pushboolean(L,(A->getObject()==B->getObject()));
		return 1;
		
		endifisvalid();
	}
	
	int RPlane::setNormal(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the Start Vector
		core::RVector3D *pA = Luna<core::RVector3D>::check(L, -1);
		
		normal = pA->getObjectDouble();
		irr_plane.Normal = pA->getObject();
		
		endifisvalid();
	}
	
	int RPlane::setD(lua_State* L)
	{
		ifistrue(objectExists);
		
		D = getmandatoryargument_cNumber(L,1);
		irr_plane.D = getmandatoryargument_cNumber(L,1);
		
		endifisvalid();
	}
	
	int RPlane::getNormal(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_plane.Normal.X,(irr::f32)normal.X) ||
			!irr::core::equals(irr_plane.Normal.Y,(irr::f32)normal.Y) ||
			!irr::core::equals(irr_plane.Normal.Z,(irr::f32)normal.Z))
		{
			normal.X = irr_plane.Normal.X;
			normal.Y = irr_plane.Normal.Y;
			normal.Z = irr_plane.Normal.Z;
		}
		// Get the Start Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		vector->setObjectDouble(normal);
		
		endifisvalid();
	}
	
	int RPlane::getD(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_plane.D,(irr::f32)D))
			D = irr_plane.D;
		lua_pushnumber(L,D);
		
		endifisvalid();
	}
	
}
