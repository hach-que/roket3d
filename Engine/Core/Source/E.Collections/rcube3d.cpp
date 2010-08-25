#include "roket3d.h"
#include "rcube3d.h"

namespace core
{
	const char *RCube3D::className = TYPE_CUBE;
	const Luna < RCube3D >::FunctionType RCube3D::Functions[] = {
		{"addInternalBox", &RCube3D::addInternalBox},
  {"addInternalPoint", &RCube3D::addInternalPoint},
  {"getCenter", &RCube3D::getCenter},
  {"getEdges", &RCube3D::getEdges},
  {"getExtent", &RCube3D::getExtent},
  {"getInterpolated", &RCube3D::getInterpolated},
  {"intersectsWithBox", &RCube3D::intersectsWithBox},
  {"intersectsWithLine", &RCube3D::intersectsWithLine},
  {"isEmpty", &RCube3D::isEmpty},
  {"isFullInside", &RCube3D::isFullInside},
  {"isPointInside", &RCube3D::isPointInside},
  {"isPointTotalInside", &RCube3D::isPointTotalInside},
  {"repair", &RCube3D::repair},
  
  {"copyFrom", &RCube3D::copyFrom},
  {"set", &RCube3D::set},

  // metaoperators
  {"__eq", &RCube3D::_equals},
  
  {0}
	};
	const Luna < RCube3D >::PropertyType RCube3D::Properties[] = {
		{"MinEdge", &RCube3D::getMin, &RCube3D::setMin},
  {"MaxEdge", &RCube3D::getMax, &RCube3D::setMax},
  {0}
	};
	
	RCube3D::RCube3D(lua_State* L, bool instantedByUser)
	{
		// RCube3D can be initalized in different forms:
		//
		//  RCube3D(minvector,maxvector);
		//  RCube3D(minx,miny,minz,maxx,maxy,maxz);
		//
		if (!lua_isnumber(L,1))
		{
			// Form 1
			
			// Get the Start Vector
			core::RVector3D *minvector = Luna<core::RVector3D>::check(L, 0);
			
			// Get the End Vector
			core::RVector3D *maxvector = Luna<core::RVector3D>::check(L, 1);
			
			irr_cube = irr::core::aabbox3df(minvector->getObject(),maxvector->getObject());
			objectExists = true;
		}
		else
		{
			// Form 2
			cNumber minX = getmandatoryargument_cNumber(L,0);
			cNumber minY = getmandatoryargument_cNumber(L,1);
			cNumber minZ = getmandatoryargument_cNumber(L,2);
			cNumber maxX = getmandatoryargument_cNumber(L,3);
			cNumber maxY = getmandatoryargument_cNumber(L,4);
			cNumber maxZ = getmandatoryargument_cNumber(L,5);
			
			irr_cube = irr::core::aabbox3df(minX,minY,minZ,maxX,maxY,maxZ);
			objectExists = true;
		}
	}
	
	int RCube3D::setObject(irr::core::aabbox3df object)
	{
		irr_cube = object;
		objectExists = true;
		return 0;
	}
	
	irr::core::aabbox3df RCube3D::getObject()
	{
		if (objectExists)
			return irr_cube;
		else
			return irr::core::aabbox3df(0,0,0,0,0,0);
	}
	
	int RCube3D::addInternalBox(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RCube3D *intbox = Luna<core::RCube3D>::check(L, 1);
		
		// Add box
		irr_cube.addInternalBox(intbox->getObject());

		return 0;
		
		endifisvalid();
	}
	
	int RCube3D::addInternalPoint(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *intpoint = Luna<core::RVector3D>::check(L, 1);
		
		// Add box
		irr_cube.addInternalPoint(intpoint->getObject());

		return 0;
		
		endifisvalid();
	}
	
	int RCube3D::getCenter(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Return the center point
		RVector3D* result = Luna < RVector3D >::createNew(L);
		cIrrVector3D newres = irr_cube.getCenter();
		result->setObject(newres);
		return 1;
		
		endifisvalid();
	}
	
	int RCube3D::getEdges(lua_State* L)
	{
		ifistrue(objectExists);
		
		//lua_pushnumber(L,irr_line.getLengthSQ());
		debug("getEdges not implemented.",LVL_WARN,L);
		return 0;
		
		endifisvalid();
	}
	
	int RCube3D::getExtent(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Return the extent
		RVector3D* result = Luna < RVector3D >::createNew(L);
		cIrrVector3D newres = irr_cube.getExtent();
		result->setObject(newres);
		return 1;
		
		endifisvalid();
	}
	
	int RCube3D::getInterpolated(lua_State* L)
	{
		ifistrue(objectExists);
	
		// Get the object
		core::RCube3D *interpolatedcube = Luna<core::RCube3D>::check(L, 1);
		cNumber d = getmandatoryargument_cNumber(L,2); // TODO: Check the argument check is correct.

		// Return the interpolated box
		RCube3D* result = Luna < RCube3D >::createNew(L);
		irr::core::aabbox3df newres = irr_cube.getInterpolated(interpolatedcube->getObject(),d);
		result->setObject(newres);
		
		return 1;
		
		endifisvalid();
	}

	int RCube3D::intersectsWithBox(lua_State* L)
	{
		ifistrue(objectExists);
	
		// Get the object
		core::RCube3D *intersectcube = Luna<core::RCube3D>::check(L, 1);

		// Return whether it intersects
		lua_pushboolean(L,irr_cube.intersectsWithBox(intersectcube->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RCube3D::intersectsWithLine(lua_State* L)
	{
		ifistrue(objectExists);
	
		// Get the object
		core::RRay3D *intersectline = Luna<core::RRay3D>::check(L, 1);

		// Return whether it intersects
		lua_pushboolean(L,irr_cube.intersectsWithLine(intersectline->getObject()));
		
		return 1;
		
		endifisvalid();
	}

	int RCube3D::isEmpty(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushboolean(L,irr_cube.isEmpty());
		return 1;
		
		endifisvalid();
	}

	int RCube3D::isFullInside(lua_State* L)
	{
		ifistrue(objectExists);
	
		// Get the object
		core::RCube3D *intersectcube = Luna<core::RCube3D>::check(L, 1);

		// Return whether it is inside
		lua_pushboolean(L,irr_cube.isFullInside(intersectcube->getObject()));
		
		return 1;
		
		endifisvalid();
	}

	int RCube3D::isPointInside(lua_State* L)
	{
		ifistrue(objectExists);
	
		// Get the object
		core::RVector3D *intersectpoint = Luna<core::RVector3D>::check(L, 1);

		// Return whether it is inside
		lua_pushboolean(L,irr_cube.isPointInside(intersectpoint->getObject()));
		
		return 1;
		
		endifisvalid();
	}

	int RCube3D::isPointTotalInside(lua_State* L)
	{
		ifistrue(objectExists);
	
		// Get the object
		core::RVector3D *intersectpoint = Luna<core::RVector3D>::check(L, 1);

		// Return whether it is inside
		lua_pushboolean(L,irr_cube.isPointTotalInside(intersectpoint->getObject()));
		
		return 1;
		
		endifisvalid();
	}

	int RCube3D::repair(lua_State* L)
	{
		ifistrue(objectExists);
	
		//Repair
		irr_cube.repair();
		
		return 0;
		
		endifisvalid();
	}

	int RCube3D::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RCube3D *set_cube = Luna<core::RCube3D>::check(L, 1);
		
		// copy values from the other cube
		irr_cube.MinEdge = set_cube->getObject().MinEdge;
		irr_cube.MaxEdge = set_cube->getObject().MaxEdge;
		
		return 0;
		
		endifisvalid();
	}
	
	int RCube3D::set(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!lua_isnumber(L,1))
		{
			// Form 1
			
			// Get the Start Vector
			core::RVector3D *minvector = Luna<core::RVector3D>::check(L, 1);
			
			// Get the End Vector
			core::RVector3D *maxvector = Luna<core::RVector3D>::check(L, 2);
			
			irr_cube = irr::core::aabbox3df(minvector->getObject(),maxvector->getObject());
			objectExists = true;
		}
		else
		{
			// Form 2
			// TODO: Check the argument check is correct
			cNumber minX = getmandatoryargument_cNumber(L,1);
			cNumber minY = getmandatoryargument_cNumber(L,2);
			cNumber minZ = getmandatoryargument_cNumber(L,3);
			cNumber maxX = getmandatoryargument_cNumber(L,4);
			cNumber maxY = getmandatoryargument_cNumber(L,5);
			cNumber maxZ = getmandatoryargument_cNumber(L,6);
			
			irr_cube = irr::core::aabbox3df(minX,minY,minZ,maxX,maxY,maxZ);
			objectExists = true;
		}

		endifisvalid();
	}
	
	int RCube3D::_equals(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RCube3D *A = Luna<core::RCube3D>::check(L, 1);
		
		// Get B
		core::RCube3D *B = Luna<core::RCube3D>::check(L, 2);
		
		// Return result
		lua_pushboolean(L,(A->getObject()==B->getObject()));
		return 1;
		
		endifisvalid();
	}
	
	int RCube3D::setMin(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Set the Min Vector
		core::RVector3D *minvector = Luna<core::RVector3D>::check(L, -1);
		
		minPoint = minvector->getObjectDouble();
		irr_cube.MinEdge = minvector->getObject();
		
		endifisvalid();
	}
	
	int RCube3D::getMin(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_cube.MinEdge.X,(irr::f32)minPoint.X) ||
			!irr::core::equals(irr_cube.MinEdge.Y,(irr::f32)minPoint.Y) ||
			!irr::core::equals(irr_cube.MinEdge.Z,(irr::f32)minPoint.Z))
		{
			minPoint.X = irr_cube.MinEdge.X;
			minPoint.Y = irr_cube.MinEdge.Y;
			minPoint.Z = irr_cube.MinEdge.Z;
		}
		// Get the Min Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		vector->setObjectDouble(minPoint);
		
		endifisvalid();
	}
	
	int RCube3D::setMax(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the Max Vector
		core::RVector3D *maxvector = Luna<core::RVector3D>::check(L, -1);
		
		maxPoint = maxvector->getObjectDouble();
		irr_cube.MaxEdge = maxvector->getObject();
		
		endifisvalid();
	}
	
	int RCube3D::getMax(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_cube.MaxEdge.X,(irr::f32)maxPoint.X) ||
			!irr::core::equals(irr_cube.MaxEdge.Y,(irr::f32)maxPoint.Y) ||
			!irr::core::equals(irr_cube.MaxEdge.Z,(irr::f32)maxPoint.Z))
		{
			maxPoint.X = irr_cube.MaxEdge.X;
			maxPoint.Y = irr_cube.MaxEdge.Y;
			maxPoint.Z = irr_cube.MaxEdge.Z;
		}
		// Set the Max Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		vector->setObjectDouble(maxPoint);
		
		endifisvalid();
	}
	
}
