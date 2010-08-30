#include "roket3d.h"
#include "E.Collections/rmatrix.h"

namespace core
{
	const char *RMatrix::className = TYPE_MATRIX;
	const Luna < RMatrix >::FunctionType RMatrix::Functions[] = {
/*		{"addInternalBox", &RCube3D::addInternalBox},
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
  {"repair", &RCube3D::repair},*/
  
  {"copyFrom", &RMatrix::copyFrom},

  // metaoperators
  {"__eq", &RMatrix::_equals},
  {"__mul", &RMatrix::_multiply},
  {"__add", &RMatrix::_add},
  {"__sub", &RMatrix::_subtract},
  
  {0}
	};
	const Luna < RMatrix >::PropertyType RMatrix::Properties[] = {
		{"Transformation", &RMatrix::getTranslation, &RMatrix::setTranslation},
  {"Rotation", &RMatrix::getRotation, &RMatrix::setRotation},
  {"Scale", &RMatrix::getScale, &RMatrix::setScale},
  {0}
	};
	
	RMatrix::RMatrix(lua_State* L, bool instantedByUser)
	{
		// RMatrix is initalized with an identity matrix.
		irr_matrix = irr::core::matrix4();
		objectExists = true;
	}
	
	int RMatrix::setObject(irr::core::matrix4 object)
	{
		irr_matrix = object;
		objectExists = true;
		return 0;
	}
	
	irr::core::matrix4 RMatrix::getObject()
	{
		if (objectExists)
			return irr_matrix;
		else
			return irr::core::matrix4();
	}
	/*
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
	*/

	int RMatrix::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RMatrix *set_matrix = Luna<core::RMatrix>::check(L, 1);
		
		// copy values from the other cube
		irr_matrix.setM(set_matrix->getObject().pointer());
		
		return 0;
		
		endifisvalid();
	}
	
	/*
	int RMatrix::set(lua_State* L)
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
	*/
	
	int RMatrix::_equals(lua_State* L)		// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RMatrix *A = Luna<core::RMatrix>::check(L, 1);
		
		// Get B
		core::RMatrix *B = Luna<core::RMatrix>::check(L, 2);
		
		// Return result
		lua_pushboolean(L,(A->getObject()==B->getObject()));
		return 1;
		
		endifisvalid();
	}

	int RMatrix::_add(lua_State* L)			// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RMatrix *A = Luna<core::RMatrix>::check(L, 1);
		
		// Get B
		core::RMatrix *B = Luna<core::RMatrix>::check(L, 2);
		
		// Return result
		irr::core::matrix4 newmat = A->getObject() + B->getObject();

		core::RMatrix *N = Luna<core::RMatrix>::createNew(L);
		N->setObject(newmat);
		return 1;
		
		endifisvalid();
	}

	int RMatrix::_subtract(lua_State* L)	// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RMatrix *A = Luna<core::RMatrix>::check(L, 1);
		
		// Get B
		core::RMatrix *B = Luna<core::RMatrix>::check(L, 2);
		
		// Return result
		irr::core::matrix4 newmat = A->getObject() - B->getObject();

		core::RMatrix *N = Luna<core::RMatrix>::createNew(L);
		N->setObject(newmat);
		return 1;
		
		endifisvalid();
	}

	int RMatrix::_multiply(lua_State* L)	// Metaoperator
	{
		ifistrue(objectExists);
		
		// Get A
		core::RMatrix *A = Luna<core::RMatrix>::check(L, 1);
		
		// Get B
		core::RMatrix *B = Luna<core::RMatrix>::check(L, 2);
		
		// Return result
		irr::core::matrix4 newmat = A->getObject() * B->getObject();

		core::RMatrix *N = Luna<core::RMatrix>::createNew(L);
		N->setObject(newmat);
		return 1;
		
		endifisvalid();
	}
	
	int RMatrix::setTranslation(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Set the translation vector
		core::RVector3D *transvector = Luna<core::RVector3D>::check(L, -1);
		
		irr_matrix.setTranslation(transvector->getObject());
		
		endifisvalid();
	}
	
	int RMatrix::getTranslation(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the Min Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		vector->setObject(irr_matrix.getTranslation());
		
		endifisvalid();
	}

	int RMatrix::setRotation(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Set the translation vector
		core::RVector3D *rotvector = Luna<core::RVector3D>::check(L, -1);
		
		irr_matrix.setRotationDegrees(rotvector->getObject());
		
		endifisvalid();
	}
	
	int RMatrix::getRotation(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the Min Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		vector->setObject(irr_matrix.getRotationDegrees());
		
		endifisvalid();
	}

	int RMatrix::setScale(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Set the translation vector
		core::RVector3D *scalevector = Luna<core::RVector3D>::check(L, -1);
		
		irr_matrix.setScale(scalevector->getObject());
		
		endifisvalid();
	}
	
	int RMatrix::getScale(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the Min Vector
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		vector->setObject(irr_matrix.getScale());
		
		endifisvalid();
	}
	
}
