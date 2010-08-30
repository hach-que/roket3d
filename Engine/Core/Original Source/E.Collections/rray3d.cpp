#include "roket3d.h"
#include "E.Collections/rray3d.h"

namespace core
{
	const char *RRay3D::className = TYPE_3DRAY;
	const Luna < RRay3D >::FunctionType RRay3D::Functions[] = {
		{"getClosestPoint", &RRay3D::getClosestPoint},
  {"getIntersectionWithSphere", &RRay3D::getIntersectionWithSphere},
  {"getLength", &RRay3D::getLength},
  {"getLengthSQ", &RRay3D::getLengthSQ},
  {"getMiddle", &RRay3D::getMiddle},
  {"getVector", &RRay3D::getVector},
  {"isPointBetweenStartAndEnd", &RRay3D::isPointBetweenStartAndEnd},
  
  {"copyFrom", &RRay3D::copyFrom},
  {"set", &RRay3D::set},

  // metaoperators
  {"__eq", &RRay3D::_equals},
  {"__add", &RRay3D::_add},
  {"__sub", &RRay3D::_sub},
  
  {0}
	};
	const Luna < RRay3D >::PropertyType RRay3D::Properties[] = {
		{"Start", &RRay3D::getStart, &RRay3D::setStart},
  {"End", &RRay3D::getEnd, &RRay3D::setEnd},
  {0}
	};
	
	RRay3D::RRay3D(lua_State* L, bool instantedByUser)
	{
		// RRay3D can be initalized in different forms:
		//
		//  RRay3D(startvector,endvector);
		//  RRay3D(startx,starty,startz,endx,endy,endz);
		//
		if (!lua_isnumber(L,1))
		{
			// Form 1
			
			// Get the Start Vector
			core::RVector3D *startvector = Luna<core::RVector3D>::check(L, 0);
			
			// Get the End Vector
			core::RVector3D *endvector = Luna<core::RVector3D>::check(L, 1);
			
			start = startvector->getObjectDouble();
			end = endvector->getObjectDouble();

			irr_line = irr::core::line3df(startvector->getObject(),endvector->getObject());
			objectExists = true;
		}
		else
		{
			// Form 2
			cNumber startX = getmandatoryargument_cNumber(L,0);
			cNumber startY = getmandatoryargument_cNumber(L,1);
			cNumber startZ = getmandatoryargument_cNumber(L,2);
			cNumber endX = getmandatoryargument_cNumber(L,3);
			cNumber endY = getmandatoryargument_cNumber(L,4);
			cNumber endZ = getmandatoryargument_cNumber(L,5);
			
			start.X = startX;
			start.Y = startY;
			start.Z = startZ;
			end.X = endX;
			end.Y = endY;
			end.Z = endZ;

			irr_line = irr::core::line3df(startX,startY,startZ,endX,endY,endZ);
			objectExists = true;
		}
	}
	
	int RRay3D::setObject(irr::core::line3df object)
	{
		irr_line = object;
		objectExists = true;
		return 0;
	}
	
	irr::core::line3df RRay3D::getObject()
	{
		if (objectExists)
			return irr_line;
		else
			return irr::core::line3df(0,0,0,1,1,1);
	}
	
	int RRay3D::getClosestPoint(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		
		// Return closest point
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_line.getClosestPoint(othervector->getObject());
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::getIntersectionWithSphere(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Check the other argument is there
		// TODO: Check argument check is correct.
		cNumber radius = getmandatoryargument_cNumber(L,1);
		
		// Create the storage variable
		irr::f64 distance = -1;
		
		// Get the object
		core::RVector3D *originvector = Luna<core::RVector3D>::check(L, 2);
		
		// Check intersection
		bool result = irr_line.getIntersectionWithSphere(originvector->getObject(),radius,distance);
		
		lua_pushboolean(L,result);
		lua_pushnumber(L,distance);
		
		return 2;
		
		endifisvalid();
	}
	
	int RRay3D::getLength(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_line.getLength());
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::getLengthSQ(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_line.getLengthSQ());
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::getMiddle(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Return the middle point
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_line.getMiddle();
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::getVector(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Return the middle point
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_line.getVector();
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::isPointBetweenStartAndEnd(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *checkvector = Luna<core::RVector3D>::check(L, 1);
		
		// Check point is between
		bool result = irr_line.isPointBetweenStartAndEnd(checkvector->getObject());
		lua_pushboolean(L,result);
		
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RRay3D *set_line = Luna<core::RRay3D>::check(L, 1);
		
		// copy values from the other vector
		irr_line.setLine(set_line->getObject());
		
		return 0;
		
		endifisvalid();
	}
	
	int RRay3D::set(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Set function can be called the same way as the constructor
		if (!lua_isnumber(L,1))
		{
			// Form 1
			
			// Get the Start Vector
			core::RVector3D *startvector = Luna<core::RVector3D>::check(L, 1);
			
			// Get the End Vector
			core::RVector3D *endvector = Luna<core::RVector3D>::check(L, 2);
			
			start = startvector->getObjectDouble();
			end = endvector->getObjectDouble();

			irr_line.setLine(startvector->getObject(),endvector->getObject());
		}
		else
		{
			// Form 2
			// TODO: Check argument check is correct.
			cNumber startX = getmandatoryargument_cNumber(L,1);
			cNumber startY = getmandatoryargument_cNumber(L,2);
			cNumber startZ = getmandatoryargument_cNumber(L,3);
			cNumber endX = getmandatoryargument_cNumber(L,4);
			cNumber endY = getmandatoryargument_cNumber(L,5);
			cNumber endZ = getmandatoryargument_cNumber(L,6);
			
			start.X = startX;
			start.Y = startY;
			start.Z = startZ;
			end.X = endX;
			end.Y = endY;
			end.Z = endZ;

			irr_line.setLine(startX,startY,startZ,endX,endY,endZ);
		}
		
		endifisvalid();
	}
	
	int RRay3D::_equals(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RRay3D *A = Luna<core::RRay3D>::check(L, 1);
		
		// Get B
		core::RRay3D *B = Luna<core::RRay3D>::check(L, 2);
		
		// Return result
		lua_pushboolean(L,(A->getObject()==B->getObject()));
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::_add(lua_State* L)			// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RRay3D *A = Luna<core::RRay3D>::check(L, 1);
		
		// Get B
		core::RVector3D *B = Luna<core::RVector3D>::check(L, 2);
		
		// Return result
		RRay3D* Res = Luna < RRay3D >::createNew(L);
		irr::core::line3df newvec = A->getObject() + B->getObject();
		Res->setObject(newvec);
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::_sub(lua_State* L)			// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RRay3D *A = Luna<core::RRay3D>::check(L, 1);
		
		// Get B
		core::RVector3D *B = Luna<core::RVector3D>::check(L, 2);
		
		// Return result
		RRay3D* Res = Luna < RRay3D >::createNew(L);
		irr::core::line3df newvec = A->getObject() - B->getObject();
		Res->setObject(newvec);
		return 1;
		
		endifisvalid();
	}
	
	int RRay3D::setStart(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Set the Start Vector
		core::RVector3D *startvector = Luna<core::RVector3D>::check(L, -1);
		
		start = startvector->getObjectDouble();
		irr_line.start = startvector->getObject();
		
		endifisvalid();
	}
	
	int RRay3D::getStart(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_line.start.X,(irr::f32)start.X) ||
			!irr::core::equals(irr_line.start.Y,(irr::f32)start.Y) ||
			!irr::core::equals(irr_line.start.Z,(irr::f32)start.Z))
		{
			start.X = irr_line.start.X;
			start.Y = irr_line.start.Y;
			start.Z = irr_line.start.Z;
		}
		// Get the Start Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		vector->setObjectDouble(start);
		
		endifisvalid();
	}
	
	int RRay3D::setEnd(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the End Vector
		core::RVector3D *endvector = Luna<core::RVector3D>::check(L, -1);
		
		end = endvector->getObjectDouble();
		irr_line.end = endvector->getObject();
		
		endifisvalid();
	}
	
	int RRay3D::getEnd(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_line.end.X,(irr::f32)end.X) ||
			!irr::core::equals(irr_line.end.Y,(irr::f32)end.Y) ||
			!irr::core::equals(irr_line.end.Z,(irr::f32)end.Z))
		{
			end.X = irr_line.end.X;
			end.Y = irr_line.end.Y;
			end.Z = irr_line.end.Z;
		}
		// Set the End Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		vector->setObjectDouble(end);
		
		endifisvalid();
	}
	
}
