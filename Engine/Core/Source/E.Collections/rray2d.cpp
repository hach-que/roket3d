#include "roket3d.h"
#include "E.Collections/rray2d.h"

namespace core
{
	const char *RRay2D::className = TYPE_2DRAY;
	const Luna < RRay2D >::FunctionType RRay2D::Functions[] = {
		{"getClosestPoint", &RRay2D::getClosestPoint},
  {"getLength", &RRay2D::getLength},
  {"getLengthSQ", &RRay2D::getLengthSQ},
  {"getMiddle", &RRay2D::getMiddle},
  {"getVector", &RRay2D::getVector},
  {"isPointBetweenStartAndEnd", &RRay2D::isPointBetweenStartAndEnd},
  
  {"copyFrom", &RRay2D::copyFrom},
  {"set", &RRay2D::set},

  // metaoperators
  {"__eq", &RRay2D::_equals},
  {"__add", &RRay2D::_add},
  {"__sub", &RRay2D::_sub},
  
  {0}
	};
	const Luna < RRay2D >::PropertyType RRay2D::Properties[] = {
		{"Start", &RRay2D::getStart, &RRay2D::setStart},
  {"End", &RRay2D::getEnd, &RRay2D::setEnd},
  {0}
	};
	
	RRay2D::RRay2D(lua_State* L, bool instantedByUser)
	{
		// RRay2D can be initalized in different forms:
		//
		//  RRay2D(startvector,endvector);
		//  RRay2D(startx,starty,endx,endy);
		//
		if (!lua_isnumber(L,1))
		{
			// Form 1
			
			// Get the Start Vector
			core::RVector2D *startvector = Luna<core::RVector2D>::check(L, 0);
			
			// Get the End Vector
			core::RVector2D *endvector = Luna<core::RVector2D>::check(L, 1);
			
			start = startvector->getObjectDouble();
			end = endvector->getObjectDouble();

			irr_line = irr::core::line2df(startvector->getObject(),endvector->getObject());
			objectExists = true;
		}
		else
		{
			// Form 2
			cNumber startX = getmandatoryargument_cNumber(L,0);
			cNumber startY = getmandatoryargument_cNumber(L,1);
			cNumber endX = getmandatoryargument_cNumber(L,2);
			cNumber endY = getmandatoryargument_cNumber(L,3);
			
			start.X = startX;
			start.Y = startY;
			end.X = endX;
			end.Y = endY;

			irr_line = irr::core::line2df(startX,startY,endX,endY);
			objectExists = true;
		}
	}
	
	int RRay2D::setObject(irr::core::line2df object)
	{
		irr_line = object;
		objectExists = true;
		return 0;
	}
	
	irr::core::line2df RRay2D::getObject()
	{
		if (objectExists)
			return irr_line;
		else
			return irr::core::line2df(0,0,1,1);
	}
	
	int RRay2D::getClosestPoint(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		
		// Return closest point
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		cIrrVector2D newvec = irr_line.getClosestPoint(othervector->getObject());
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::getLength(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_line.getLength());
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::getLengthSQ(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_line.getLengthSQ());
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::getMiddle(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Return the middle point
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		cIrrVector2D newvec = irr_line.getMiddle();
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::getVector(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Return the middle point
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		cIrrVector2D newvec = irr_line.getVector();
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::isPointBetweenStartAndEnd(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector2D *checkvector = Luna<core::RVector2D>::check(L, 1);
		
		// Check point is between
		bool result = irr_line.isPointBetweenStartAndEnd(checkvector->getObject());
		lua_pushboolean(L,result);
		
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RRay2D *set_line = Luna<core::RRay2D>::check(L, 1);
		
		// copy values from the other vector
		irr_line.setLine(set_line->getObject());
		
		return 0;
		
		endifisvalid();
	}
	
	int RRay2D::set(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Set function can be called the same way as the constructor
		if (!lua_isnumber(L,1))
		{
			// Form 1
			
			// Get the Start Vector
			core::RVector2D *startvector = Luna<core::RVector2D>::check(L, 1);
			
			// Get the End Vector
			core::RVector2D *endvector = Luna<core::RVector2D>::check(L, 2);
			
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
			cNumber endX = getmandatoryargument_cNumber(L,3);
			cNumber endY = getmandatoryargument_cNumber(L,4);
			
			start.X = startX;
			start.Y = startY;
			end.X = endX;
			end.Y = endY;

			irr_line.setLine(startX,startY,endX,endY);
		}
		
		endifisvalid();
	}
	
	int RRay2D::_equals(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RRay2D *A = Luna<core::RRay2D>::check(L, 1);
		
		// Get B
		core::RRay2D *B = Luna<core::RRay2D>::check(L, 2);
		
		// Return result
		lua_pushboolean(L,(A->getObject()==B->getObject()));
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::_add(lua_State* L)			// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RRay2D *A = Luna<core::RRay2D>::check(L, 1);
		
		// Get B
		core::RVector2D *B = Luna<core::RVector2D>::check(L, 2);
		
		// Return result
		RRay2D* Res = Luna < RRay2D >::createNew(L);
		irr::core::line2df newvec = A->getObject() + B->getObject();
		Res->setObject(newvec);
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::_sub(lua_State* L)			// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RRay2D *A = Luna<core::RRay2D>::check(L, 1);
		
		// Get B
		core::RVector2D *B = Luna<core::RVector2D>::check(L, 2);
		
		// Return result
		RRay2D* Res = Luna < RRay2D >::createNew(L);
		irr::core::line2df newvec = A->getObject() - B->getObject();
		Res->setObject(newvec);
		return 1;
		
		endifisvalid();
	}
	
	int RRay2D::setStart(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Set the Start Vector
		core::RVector2D *startvector = Luna<core::RVector2D>::check(L, -1);
		
		start = startvector->getObjectDouble();
		irr_line.start = startvector->getObject();
		
		endifisvalid();
	}
	
	int RRay2D::getStart(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_line.start.X,(irr::f32)start.X) ||
			!irr::core::equals(irr_line.start.Y,(irr::f32)start.Y))
		{
			start.X = irr_line.start.X;
			start.Y = irr_line.start.Y;
		}
		// Get the Start Vector
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		vector->setObjectDouble(start);
		
		endifisvalid();
	}
	
	int RRay2D::setEnd(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the End Vector
		core::RVector2D *endvector = Luna<core::RVector2D>::check(L, -1);
		
		end = endvector->getObjectDouble();
		irr_line.end = endvector->getObject();
		
		endifisvalid();
	}
	
	int RRay2D::getEnd(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_line.end.X,(irr::f32)end.X) ||
			!irr::core::equals(irr_line.end.Y,(irr::f32)end.Y))
		{
			end.X = irr_line.end.X;
			end.Y = irr_line.end.Y;
		}
		// Set the End Vector
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		vector->setObjectDouble(end);
		
		endifisvalid();
	}
	
}
