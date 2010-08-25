#include "roket3d.h"
#include "E.Unmanaged/Video/rvideomaterial.h"

namespace video
{
	const char *RMaterial::className = TYPE_RVIDEOMATERIAL;
	const Luna < RMaterial >::FunctionType RMaterial::Functions[] = {
		{"applyTexture", &RMaterial::applyTexture},
  {"applyTextureMatrix", &RMaterial::applyTextureMatrix},
  {"applyFlag", &RMaterial::applyFlag},
 /* {"getEdges", &RCube3D::getEdges},
  {"getExtent", &RCube3D::getExtent},
  {"getInterpolated", &RCube3D::getInterpolated},
  {"intersectsWithBox", &RCube3D::intersectsWithBox},
  {"intersectsWithLine", &RCube3D::intersectsWithLine},
  {"isEmpty", &RCube3D::isEmpty},
  {"isFullInside", &RCube3D::isFullInside},
  {"isPointInside", &RCube3D::isPointInside},
  {"isPointTotalInside", &RCube3D::isPointTotalInside},
  {"repair", &RCube3D::repair},*/
  
  {0}
	};
	const Luna < RMaterial >::PropertyType RMaterial::Properties[] = {
  {0}
	};
	
	RMaterial::RMaterial(lua_State* L, bool instantedByUser)
	{
		// RMatrix is initalized with an identity matrix.
		if (instantedByUser)
		{
			irr_material = irr::video::SMaterial();
			objectExists = true;
		}
	}
	
	int RMaterial::setObject(irr::video::SMaterial object)
	{
		irr_material = object;
		objectExists = true;
		return 0;
	}
	
	irr::video::SMaterial RMaterial::getObject()
	{
		if (objectExists)
			return irr_material;
		else
			return irr::video::SMaterial();
	}

	int RMaterial::applyTexture(lua_State* L)
	{
		ifistrue(objectExists);
		
		video::RTexture *tex = Luna<video::RTexture>::check(L, 1);
		cNumber d = getmandatoryargument_cNumber(L,2);
		irr_material.setTexture(d,tex->getTexture());
		return 0;
		
		endifisvalid();
	}

	int RMaterial::applyTextureMatrix(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RMatrix *matrix = Luna<core::RMatrix>::check(L, 1);
		cNumber d = getmandatoryargument_cNumber(L,2);
		irr_material.setTextureMatrix(d,matrix->getObject());
		return 0;
		
		endifisvalid();
	}
	
	int RMaterial::applyFlag(lua_State* L)
	{
		ifistrue(objectExists);
		
		const char* flagType = getmandatoryargument_constchar(L,1);
		bool flagSet = getmandatoryargument_bool(L,2);
		irr::video::E_MATERIAL_FLAG flagIrrType = irr::video::EMF_MATERIAL_FLAG_COUNT;
		
		if (strcmp(flagType,"WIREFRAME")==0)
			flagIrrType = irr::video::EMF_WIREFRAME;
		if (strcmp(flagType,"POINTCLOUD")==0)
			flagIrrType = irr::video::EMF_POINTCLOUD;
		if (strcmp(flagType,"GOURAUDSHADING")==0)
			flagIrrType = irr::video::EMF_GOURAUD_SHADING;
		if (strcmp(flagType,"ZBUFFER")==0)
			flagIrrType = irr::video::EMF_ZBUFFER;
		if (strcmp(flagType,"ZWRITEENABLE")==0)
			flagIrrType = irr::video::EMF_ZWRITE_ENABLE;
		if (strcmp(flagType,"BACKFACECULLING")==0)
			flagIrrType = irr::video::EMF_BACK_FACE_CULLING;
		if (strcmp(flagType,"FRONTFACECULLING")==0)
			flagIrrType = irr::video::EMF_FRONT_FACE_CULLING;
		if (strcmp(flagType,"BILINEARFILTER")==0)
			flagIrrType = irr::video::EMF_BILINEAR_FILTER;
		if (strcmp(flagType,"TRILINEARFILTER")==0)
			flagIrrType = irr::video::EMF_TRILINEAR_FILTER;
		if (strcmp(flagType,"ANISOTROPICFILTER")==0)
			flagIrrType = irr::video::EMF_ANISOTROPIC_FILTER;
		if (strcmp(flagType,"FOGENABLE")==0)
			flagIrrType = irr::video::EMF_FOG_ENABLE;
		if (strcmp(flagType,"NORMALIZENORMALS")==0)
			flagIrrType = irr::video::EMF_NORMALIZE_NORMALS;
		if (strcmp(flagType,"TEXTUREWRAP")==0)
			flagIrrType = irr::video::EMF_TEXTURE_WRAP;
		if (strcmp(flagType,"LIGHTING")==0)
			flagIrrType = irr::video::EMF_LIGHTING;

		if (flagIrrType == irr::video::EMF_MATERIAL_FLAG_COUNT)
		{
			debug("Invalid flag passed to applyFlag.",LVL_ERR,L);
			return 0;
		}
		
		irr_material.setFlag(flagIrrType, flagSet);
		return 0;
		
		endifisvalid();
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
	
}
