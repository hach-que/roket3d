#include "roket3d.h"
#include "rtriangle3d.h"

namespace core
{
	const char *RTriangle3D::className = TYPE_3DTRIANGLE;
	const Luna < RTriangle3D >::FunctionType RTriangle3D::Functions[] = {
		{"closestPointOnTriangle", &RTriangle3D::closestPointOnTriangle},
  {"getIntersectionOfPlaneWithLine", &RTriangle3D::getIntersectionOfPlaneWithLine},
  {"getIntersectionWithLimitedLine", &RTriangle3D::getIntersectionWithLimitedLine},
  {"getIntersectionWithLine", &RTriangle3D::getIntersectionWithLine},
  {"getPlane", &RTriangle3D::getPlane},
  {"isFrontFacing", &RTriangle3D::isFrontFacing},
  {"isPointInside", &RTriangle3D::isPointInside},
  {"isPointInsideFast", &RTriangle3D::isPointInsideFast},
  {"isTotalInsideBox", &RTriangle3D::isTotalInsideBox},
  
  {"copyFrom", &RTriangle3D::copyFrom},
  {"set", &RTriangle3D::set},

  // metaoperators
  {"__eq", &RTriangle3D::_equals},
  
  {0}
	};
	const Luna < RTriangle3D >::PropertyType RTriangle3D::Properties[] = {
		{"pointA", &RTriangle3D::getPointA, &RTriangle3D::setPointA},
  {"pointB", &RTriangle3D::getPointB, &RTriangle3D::setPointB},
  {"pointC", &RTriangle3D::getPointC, &RTriangle3D::setPointC},
  {0}
	};
	
	RTriangle3D::RTriangle3D(lua_State* L, bool instantedByUser)
	{
		// RTriangle3D can be initalized in different forms:
		//
		//  RTriangle3D();
		//  RTriangle3D(pointA,pointB,pointC);
		//
		if (lua_isnil(L,1))
		{
			// Form 1
			irr_triangle = irr::core::triangle3df();
			objectExists = true;
		}
		else
		{
			// Form 2
			
			// Get point A
			core::RVector3D *pA = Luna<core::RVector3D>::check(L, 0);
			
			// Get point B
			core::RVector3D *pB = Luna<core::RVector3D>::check(L, 1);
			
			// Get point C
			core::RVector3D *pC = Luna<core::RVector3D>::check(L, 2);
			
			pointA = pA->getObjectDouble();
			pointB = pB->getObjectDouble();
			pointC = pC->getObjectDouble();
			irr_triangle = irr::core::triangle3df(pA->getObject(),pB->getObject(),pC->getObject());
			objectExists = true;
		}
	}
	
	int RTriangle3D::setObject(irr::core::triangle3df object)
	{
		irr_triangle = object;
		objectExists = true;
		return 0;
	}
	
	irr::core::triangle3df RTriangle3D::getObject()
	{
		if (objectExists)
			return irr_triangle;
		else
			return irr::core::triangle3df();
	}
	
	int RTriangle3D::closestPointOnTriangle(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		
		// Return closest point
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_triangle.closestPointOnTriangle(othervector->getObject());
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RTriangle3D::getIntersectionOfPlaneWithLine(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line point
		core::RVector3D *origin = Luna<core::RVector3D>::check(L, 1);
		
		// Get the line direction
		core::RVector3D *direction = Luna<core::RVector3D>::check(L, 2);
		
		// Create the storage variable
		cIrrVector3D collision = cIrrVector3D();
		
		// Check intersection
		bool result = irr_triangle.getIntersectionOfPlaneWithLine(origin->getObject(),direction->getObject(),collision);
		
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
	
	int RTriangle3D::getIntersectionWithLimitedLine(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line point
		core::RRay3D *origin = Luna<core::RRay3D>::check(L, 1);
		
		// Create the storage variable
		cIrrVector3D collision = cIrrVector3D();
		
		// Check intersection
		bool result = irr_triangle.getIntersectionWithLimitedLine(origin->getObject(),collision);
		
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
	
	int RTriangle3D::getIntersectionWithLine(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line point
		core::RVector3D *origin = Luna<core::RVector3D>::check(L, 1);
		
		// Get the line direction
		core::RVector3D *direction = Luna<core::RVector3D>::check(L, 2);
		
		// Create the storage variable
		cIrrVector3D collision = cIrrVector3D();
		
		// Check intersection
		bool result = irr_triangle.getIntersectionWithLine(origin->getObject(),direction->getObject(),collision);
		
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
	
	int RTriangle3D::getPlane(lua_State* L)
	{
		ifistrue(objectExists);
		
		RPlane* returnplane = Luna < RPlane >::createNew(L);	
		irr::core::plane3df plane = irr_triangle.getPlane();
		returnplane->setObject(plane);
		return 1;

		endifisvalid();
	}
	
	int RTriangle3D::isFrontFacing(lua_State* L)
	{
		ifistrue(objectExists);

		// get comparison point
		core::RVector3D *compare = Luna<core::RVector3D>::check(L, 1);

		lua_pushboolean(L,irr_triangle.isFrontFacing(compare->getObject()));
		return 1;

		endifisvalid();
	}
	
	int RTriangle3D::isPointInside(lua_State* L)
	{
		ifistrue(objectExists);

		// get comparison point
		core::RVector3D *compare = Luna<core::RVector3D>::check(L, 1);

		lua_pushboolean(L,irr_triangle.isFrontFacing(compare->getObject()));
		return 1;

		endifisvalid();
	}
	
	int RTriangle3D::isPointInsideFast(lua_State* L)
	{
		ifistrue(objectExists);

		// get comparison point
		core::RVector3D *compare = Luna<core::RVector3D>::check(L, 1);

		lua_pushboolean(L,irr_triangle.isPointInsideFast(compare->getObject()));
		return 1;

		endifisvalid();
	}
	
	int RTriangle3D::isTotalInsideBox(lua_State* L)
	{
		ifistrue(objectExists);

		// get comparison point
		core::RCube3D *compare = Luna<core::RCube3D>::check(L, 1);

		lua_pushboolean(L,irr_triangle.isTotalInsideBox(compare->getObject()));
		return 1;

		endifisvalid();
	}
	
	int RTriangle3D::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RTriangle3D *set_line = Luna<core::RTriangle3D>::check(L, 1);
		
		// copy values from the other vector
		irr_triangle.set(set_line->getObject().pointA,set_line->getObject().pointB,set_line->getObject().pointC);
		
		return 0;
		
		endifisvalid();
	}
	
	int RTriangle3D::set(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get point A
		core::RVector3D *pA = Luna<core::RVector3D>::check(L, 1);
			
		// Get point B
		core::RVector3D *pB = Luna<core::RVector3D>::check(L, 2);
			
		// Get point C
		core::RVector3D *pC = Luna<core::RVector3D>::check(L, 3);
		
		pointA = pA->getObjectDouble();
		pointB = pB->getObjectDouble();
		pointC = pC->getObjectDouble();
		irr_triangle = irr::core::triangle3df(pA->getObject(),pB->getObject(),pC->getObject());
		objectExists = true;
		return 0;
		
		endifisvalid();
	}
	
	int RTriangle3D::_equals(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RTriangle3D *A = Luna<core::RTriangle3D>::check(L, 1);
		
		// Get B
		core::RTriangle3D *B = Luna<core::RTriangle3D>::check(L, 2);
		
		// Return result
		lua_pushboolean(L,(A->getObject()==B->getObject()));
		return 1;
		
		endifisvalid();
	}
	
	int RTriangle3D::setPointA(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the Start Vector
		core::RVector3D *pA = Luna<core::RVector3D>::check(L, -1);
		
		pointA = pA->getObjectDouble();
		irr_triangle.pointA = pA->getObject();
		return 0;

		endifisvalid();
	}
	
	int RTriangle3D::setPointB(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the Start Vector
		core::RVector3D *pB = Luna<core::RVector3D>::check(L, -1);
		
		pointB = pB->getObjectDouble();
		irr_triangle.pointB = pB->getObject();
		return 0;

		endifisvalid();
	}
	
	int RTriangle3D::setPointC(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the Start Vector
		core::RVector3D *pC = Luna<core::RVector3D>::check(L, -1);
		
		pointC = pC->getObjectDouble();
		irr_triangle.pointC = pC->getObject();
		return 0;

		endifisvalid();
	}
	
	int RTriangle3D::getPointA(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_triangle.pointA.X,(irr::f32)pointA.X) ||
			!irr::core::equals(irr_triangle.pointA.Y,(irr::f32)pointA.Y) ||
			!irr::core::equals(irr_triangle.pointA.Z,(irr::f32)pointA.Z))
		{
			pointA.X = irr_triangle.pointA.X;
			pointA.Y = irr_triangle.pointA.Y;
			pointA.Z = irr_triangle.pointA.Z;
		}
		// Get the Start Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		irr::core::vector3d<double> newvec = pointA;
		vector->setObjectDouble(newvec);
		return 1;

		endifisvalid();
	}
	
	int RTriangle3D::getPointB(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_triangle.pointB.X,(irr::f32)pointB.X) ||
			!irr::core::equals(irr_triangle.pointB.Y,(irr::f32)pointB.Y) ||
			!irr::core::equals(irr_triangle.pointB.Z,(irr::f32)pointB.Z))
		{
			pointB.X = irr_triangle.pointB.X;
			pointB.Y = irr_triangle.pointB.Y;
			pointB.Z = irr_triangle.pointB.Z;
		}
		// Get the Start Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		irr::core::vector3d<double> newvec = pointB;
		vector->setObjectDouble(newvec);
		return 1;

		endifisvalid();
	}
	
	int RTriangle3D::getPointC(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_triangle.pointC.X,(irr::f32)pointC.X) ||
			!irr::core::equals(irr_triangle.pointC.Y,(irr::f32)pointC.Y) ||
			!irr::core::equals(irr_triangle.pointC.Z,(irr::f32)pointC.Z))
		{
			pointC.X = irr_triangle.pointC.X;
			pointC.Y = irr_triangle.pointC.Y;
			pointC.Z = irr_triangle.pointC.Z;
		}
		// Get the Start Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		irr::core::vector3d<double> newvec = pointC;
		vector->setObjectDouble(newvec);
		return 1;

		endifisvalid();
	}
	
}
