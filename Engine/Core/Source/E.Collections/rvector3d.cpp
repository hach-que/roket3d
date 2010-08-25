#include "roket3d.h"
#include "rvector3d.h"

/*
	TypedValue.__index
	TypedValue.__add
	TypedValue.__sub = M.boxed_op(function(a, b) return a - b end)
	TypedValue.__mul = M.boxed_op(function(a, b) return a * b end)
	TypedValue.__div = M.boxed_op(function(a, b) return a / b end)
	TypedValue.__pow = M.boxed_op(function(a, b) return a ^ b end)
	TypedValue.__mod = M.boxed_op(function(a, b) return a % b end)
	TypedValue.__concat = M.boxed_op(function(a, b) return a .. b end)
	-- TypedValue.__le -- not going to work? (metafunction returns Boolean)
	-- TypedValue.__lt -- not going to work? (metafunction returns Boolean)
	-- TypedValue.__eq -- not going to work? (metafunction returns Boolean)
	TypedValue.__tostring = function(self)
	local str = "[" .. tostring(self.v) .. " in "
	for i,v in ipairs(self) do
	if i ~= 1 then str = str .. ", " end
	str = str .. v
	end
	str = str .. "]"
	return str 
	end
*/

namespace core
{
	const char *RVector3D::className = TYPE_3DVECTOR;
	const Luna < RVector3D >::FunctionType RVector3D::Functions[] = {
		{"crossProduct", &RVector3D::crossProduct},
  {"dotProduct", &RVector3D::dotProduct},
  {"equals", &RVector3D::equals},
  {"getAs4Values", &RVector3D::getAs4Values},
  {"getDistanceFrom", &RVector3D::getDistanceFrom},
  {"getDistanceFromSQ", &RVector3D::getDistanceFromSQ},
  {"getHorizontalAngle", &RVector3D::getHorizontalAngle},
  {"getInterpolated", &RVector3D::getInterpolated},
  {"getInterpolatedQuadratic", &RVector3D::getInterpolatedQuadratic},
  {"getLength", &RVector3D::getLength},
  {"getLengthSQ", &RVector3D::getLengthSQ},
  {"invert", &RVector3D::invert},
  {"isBetweenPoints", &RVector3D::isBetweenPoints},
  {"normalize", &RVector3D::normalize},
  {"rotateXYBy", &RVector3D::rotateXYBy},
  {"rotateXZBy", &RVector3D::rotateXZBy},
  {"rotateYZBy", &RVector3D::rotateYZBy},
  {"copyFrom", &RVector3D::copyFrom},
  {"set", &RVector3D::set},
  {"setLength", &RVector3D::setLength},
  // metafunctions like __add, __sub, etc...
  {"__eq", &RVector3D::equals},
  {0}
	};
	const Luna < RVector3D >::PropertyType RVector3D::Properties[] = {
		{"X", &RVector3D::getX, &RVector3D::setX},
  {"Y", &RVector3D::getY, &RVector3D::setY},
  {"Z", &RVector3D::getZ, &RVector3D::setZ},
  {0}
	};
	
	
	RVector3D::RVector3D(lua_State* L, bool instantedByUser)
	{
		X = getoptionalargument(L,0,0);
		Y = getoptionalargument(L,1,0);
		Z = getoptionalargument(L,2,0);
		irr_vector = cIrrVector3D((irr::f32)X,(irr::f32)Y,(irr::f32)Z);
		objectExists = true;
	}
	
	int RVector3D::setObject(cIrrVector3D object)
	{
		irr_vector = object;
		objectExists = true;
		return 0;
	}

	int RVector3D::setObjectDouble(irr::core::vector3d<double> object)
	{
		X = object.X;
		Y = object.Y;
		Z = object.Z;
		irr_vector = cIrrVector3D((irr::f32)X,(irr::f32)Y,(irr::f32)Z);
		objectExists = true;
		return 0;
	}
	
	cIrrVector3D RVector3D::getObject()
	{
		if (objectExists)
			return irr_vector;
		else
			return cIrrVector3D(0,0,0);
	}

	irr::core::vector3d<double> RVector3D::getObjectDouble()
	{
		if (objectExists)
			return irr::core::vector3d<double>(X,Y,Z);
		else
			return irr::core::vector3d<double>(0,0,0);
	}
	
	int RVector3D::crossProduct(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		
		// Cross product
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_vector.crossProduct(othervector->getObject());
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::dotProduct(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		
		// Dot product
		lua_pushnumber(L,irr_vector.dotProduct(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::equals(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		
		// Check equals
		lua_pushboolean(L,irr_vector.equals(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getAs4Values(lua_State* L)
	{
		ifistrue(objectExists);
		
		cIrrNumber array[4];
		irr_vector.getAs4Values(array);
		lua_pushnumber(L,array[0]);
		lua_pushnumber(L,array[1]);
		lua_pushnumber(L,array[2]);
		lua_pushnumber(L,array[3]);
		
		return 4;
		
		endifisvalid();
	}
	
	int RVector3D::getDistanceFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other object
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		
		// Perform operations
		lua_pushnumber(L,irr_vector.getDistanceFrom(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getDistanceFromSQ(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other object
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		
		// Perform operations
		lua_pushnumber(L,irr_vector.getDistanceFromSQ(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getHorizontalAngle(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Perform operations
		core::RVector3D* vector = Luna < core::RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_vector.getHorizontalAngle();
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getInterpolated(lua_State* L)
	{
		ifistrue(objectExists);
		
		double d = getoptionalargument(L,2,0);
		
		// Get the other object
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		
		// Perform operations
		core::RVector3D* vector = Luna < core::RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_vector.getInterpolated(othervector->getObject(),d);
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getInterpolatedQuadratic(lua_State* L)
	{
		ifistrue(objectExists);
		
		double d = getoptionalargument(L,3,0);
		
		// Get the other objects
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		core::RVector3D *othervector2 = Luna<core::RVector3D>::check(L, 2);
		
		// Perform operations
		core::RVector3D* vector = Luna < core::RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_vector.getInterpolated_quadratic(othervector->getObject(),othervector2->getObject(),d);
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getLength(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_vector.getLength());
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getLengthSQ(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_vector.getLengthSQ());
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::invert(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr_vector.invert();
		return 0;
		
		endifisvalid();
	}
	
	int RVector3D::isBetweenPoints(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other objects
		core::RVector3D *othervector = Luna<core::RVector3D>::check(L, 1);
		core::RVector3D *othervector2 = Luna<core::RVector3D>::check(L, 2);
		
		lua_pushboolean(L,irr_vector.isBetweenPoints(othervector->getObject(),othervector2->getObject()));
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::normalize(lua_State* L)
	{
		ifistrue(objectExists);
		
		// normalize
		RVector3D* vector = Luna < RVector3D >::createNew(L);
		cIrrVector3D newvec = irr_vector.normalize();
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::rotateXYBy(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr::f64 degrees = getoptionalargument(L,1,0);
		core::RVector3D *center_vector = Luna<core::RVector3D>::check(L, 2);
		
		// rotate vector
		irr_vector.rotateXYBy(degrees,center_vector->getObject());
		
		return 0;
		
		endifisvalid();
	}
	
	int RVector3D::rotateXZBy(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr::f64 degrees = getoptionalargument(L,1,0);
		core::RVector3D *center_vector = Luna<core::RVector3D>::check(L, 2);
		
		// rotate vector
		irr_vector.rotateXZBy(degrees,center_vector->getObject());
		
		return 0;
		
		endifisvalid();
	}
	
	int RVector3D::rotateYZBy(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr::f64 degrees = getoptionalargument(L,1,0);
		core::RVector3D *center_vector = Luna<core::RVector3D>::check(L, 2);
		
		// rotate vector
		irr_vector.rotateYZBy(degrees,center_vector->getObject());
		
		return 0;
		
		endifisvalid();
	}
	
	int RVector3D::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RVector3D *set_vector = Luna<core::RVector3D>::check(L, 1);
		
		// copy values from the other vector
		irr_vector.set(set_vector->getObject());
		
		return 0;
		
		endifisvalid();
	}
	
	int RVector3D::set(lua_State* L)
	{
		ifistrue(objectExists);

		// TODO: Check argument check is correct.
		X = getmandatoryargument_cNumber(L,1);
		Y = getmandatoryargument_cNumber(L,2);
		Z = getmandatoryargument_cNumber(L,3);

		irr_vector.X = X;
		irr_vector.Y = Y;
		irr_vector.Z = Z;

		// set values
		//irr_vector.set(setX,setY,setZ);
		
		return 0;
		
		endifisvalid();
	}
	
	int RVector3D::setLength(lua_State* L)
	{
		ifistrue(objectExists);
		
		// TODO: Check argument check is correct.
		cIrrNumber length = getmandatoryargument_cNumber(L,1);

		// set values
		irr_vector.setLength(length);
		
		return 0;
		
		endifisvalid();
	}
	
	// **************** BEGIN PROPERTY FUNCTIONS ***************************
	
	int RVector3D::getX(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_vector.X,(irr::f32)X))
			X = irr_vector.X;
		lua_pushnumber(L,X);

		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getY(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_vector.Y,(irr::f32)Y))
			Y = irr_vector.Y; 
		lua_pushnumber(L,Y);
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::getZ(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_vector.Z,(irr::f32)Z))
			Z = irr_vector.Z;
		lua_pushnumber(L,Z);
		return 1;
		
		endifisvalid();
	}
	
	int RVector3D::setX(lua_State* L)
	{
		ifistrue(objectExists);
		
		X = lua_tonumber(L,-1);
		irr_vector.X = lua_tonumber(L,-1);
		return 0;
		
		endifisvalid();
	}
	
	int RVector3D::setY(lua_State* L)
	{
		ifistrue(objectExists);
		
		Y = lua_tonumber(L,-1);
		irr_vector.Y = lua_tonumber(L,-1);
		return 0;
		
		endifisvalid();
	}
	
	int RVector3D::setZ(lua_State* L)
	{
		ifistrue(objectExists);
		
		Z = lua_tonumber(L,-1);
		irr_vector.Z = lua_tonumber(L,-1);
		return 0;
		
		endifisvalid();
	}
}
