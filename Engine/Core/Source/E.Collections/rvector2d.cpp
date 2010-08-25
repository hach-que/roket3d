#include "roket3d.h"
#include "E.Collections/rvector2d.h"

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
	const char *RVector2D::className = TYPE_2DVECTOR;
	const Luna < RVector2D >::FunctionType RVector2D::Functions[] = {
  {"dotProduct", &RVector2D::dotProduct},
  {"equals", &RVector2D::equals},
  {"getAngle", &RVector2D::getAngle},
  {"getAngleTrig", &RVector2D::getAngleTrig},
  {"getAngleWith", &RVector2D::getAngleWith},
  {"getDistanceFrom", &RVector2D::getDistanceFrom},
  {"getDistanceFromSQ", &RVector2D::getDistanceFromSQ},
  {"getInterpolated", &RVector2D::getInterpolated},
  {"getInterpolatedQuadratic", &RVector2D::getInterpolatedQuadratic},
  {"getLength", &RVector2D::getLength},
  {"getLengthSQ", &RVector2D::getLengthSQ},
  {"isBetweenPoints", &RVector2D::isBetweenPoints},
  {"normalize", &RVector2D::normalize},
  {"rotateBy", &RVector2D::rotateBy},
  {"copy", &RVector2D::copyFrom},
  {"set", &RVector2D::set},
  // metafunctions like __add, __sub, etc...
  {"__eq", &RVector2D::equals},
  {0}
	};
	const Luna < RVector2D >::PropertyType RVector2D::Properties[] = {
		{"X", &RVector2D::getX, &RVector2D::setX},
  {"Y", &RVector2D::getY, &RVector2D::setY},
  {0}
	};
	
	
	RVector2D::RVector2D(lua_State* L, bool instantedByUser)
	{
		X = getoptionalargument(L,0,0);
		Y = getoptionalargument(L,1,0);
		irr_vector = cIrrVector2D((irr::f32)X,(irr::f32)Y);
		objectExists = true;
	}
	
	int RVector2D::setObject(cIrrVector2D object)
	{
		irr_vector = object;
		objectExists = true;
		return 0;
	}
	
	int RVector2D::setObjectDouble(irr::core::vector2d<double> object)
	{
		X = object.X;
		Y = object.Y;
		irr_vector = cIrrVector2D((irr::f32)X,(irr::f32)Y);
		objectExists = true;
		return 0;
	}

	irr::core::dimension2d<cIrrNumber> RVector2D::getObjectAsDimension()
	{
		if (objectExists)
		{
			irr::core::dimension2d<cIrrNumber> dim;
			dim.Height = irr_vector.Y;
			dim.Width = irr_vector.X;
			return dim;
		}
		else
			return irr::core::dimension2d<cIrrNumber>(0,0);
	}

	irr::core::position2d<cIrrNumber> RVector2D::getObjectAsPosition()
	{
		if (objectExists)
		{
			irr::core::position2d<cIrrNumber> pos;
			pos.Y = irr_vector.Y;
			pos.X = irr_vector.X;
			return pos;
		}
		else
			return irr::core::position2d<cIrrNumber>(0,0);
	}

	cIrrVector2D RVector2D::getObject()
	{
		if (objectExists)
			return irr_vector;
		else
			return cIrrVector2D(0,0);
	}

	irr::core::vector2d<double> RVector2D::getObjectDouble()
	{
		if (objectExists)
			return irr::core::vector2d<double>(X,Y);
		else
			return irr::core::vector2d<double>(0,0);
	}
	
	int RVector2D::dotProduct(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		
		// Dot product
		lua_pushnumber(L,irr_vector.dotProduct(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::equals(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the object
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		
		// Check equals
		lua_pushboolean(L,irr_vector.equals(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::getDistanceFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other object
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		
		// Perform operations
		lua_pushnumber(L,irr_vector.getDistanceFrom(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::getDistanceFromSQ(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other object
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		
		// Perform operations
		lua_pushnumber(L,irr_vector.getDistanceFromSQ(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::getInterpolated(lua_State* L)
	{
		ifistrue(objectExists);
		
		double d = getoptionalargument(L,2,0);
		
		// Get the other object
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		
		// Perform operations
		core::RVector2D* vector = Luna < core::RVector2D >::createNew(L);
		cIrrVector2D newvec = irr_vector.getInterpolated(othervector->getObject(),d);
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::getInterpolatedQuadratic(lua_State* L)
	{
		ifistrue(objectExists);
		
		double d = getoptionalargument(L,3,0);
		
		// Get the other objects
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		core::RVector2D *othervector2 = Luna<core::RVector2D>::check(L, 2);
		
		// Perform operations
		core::RVector2D* vector = Luna < core::RVector2D >::createNew(L);
		cIrrVector2D newvec = irr_vector.getInterpolated_quadratic(othervector->getObject(),othervector2->getObject(),d);
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::getLength(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_vector.getLength());
		return 1;
		
		endifisvalid();
	}

	int RVector2D::getAngle(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_vector.getAngle());
		return 1;
		
		endifisvalid();
	}

	int RVector2D::getAngleTrig(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_vector.getAngleTrig());
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::getAngleWith(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other object
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		
		// Perform operations
		lua_pushnumber(L,irr_vector.getAngleWith(othervector->getObject()));
		
		return 1;
		
		endifisvalid();
	}

	int RVector2D::getLengthSQ(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_vector.getLengthSQ());
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::isBetweenPoints(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other objects
		core::RVector2D *othervector = Luna<core::RVector2D>::check(L, 1);
		core::RVector2D *othervector2 = Luna<core::RVector2D>::check(L, 2);
		
		lua_pushboolean(L,irr_vector.isBetweenPoints(othervector->getObject(),othervector2->getObject()));
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::normalize(lua_State* L)
	{
		ifistrue(objectExists);
		
		// normalize
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		cIrrVector2D newvec = irr_vector.normalize();
		vector->setObject(newvec);
		
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::rotateBy(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr::f64 degrees = getoptionalargument(L,1,0);
		core::RVector2D *center_vector = Luna<core::RVector2D>::check(L, 2);
		
		// rotate vector
		irr_vector.rotateBy(degrees,center_vector->getObject());
		
		return 0;
		
		endifisvalid();
	}
	
	int RVector2D::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RVector2D *set_vector = Luna<core::RVector2D>::check(L, 1);
		
		// copy values from the other vector
		irr_vector.set(set_vector->getObject());
		
		return 0;
		
		endifisvalid();
	}
	
	int RVector2D::set(lua_State* L)
	{
		ifistrue(objectExists);
		
		// TODO: Check argument check is correct.
		X = getmandatoryargument_cNumber(L,1);
		Y = getmandatoryargument_cNumber(L,2);

		irr_vector.X = X;
		irr_vector.Y = Y;

		// set values
		//irr_vector.set(setX,setY);
		
		return 0;
		
		endifisvalid();
	}
	
	// **************** BEGIN PROPERTY FUNCTIONS ***************************
	
	int RVector2D::getX(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_vector.X,(irr::f32)X))
			X = irr_vector.X;
		lua_pushnumber(L,X);
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::getY(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_vector.Y,(irr::f32)Y))
			Y = irr_vector.Y;
		lua_pushnumber(L,Y);
		return 1;
		
		endifisvalid();
	}
	
	int RVector2D::setX(lua_State* L)
	{
		ifistrue(objectExists);
		
		X = lua_tonumber(L,-1);
		irr_vector.X = lua_tonumber(L,-1);
		return 0;
		
		endifisvalid();
	}
	
	int RVector2D::setY(lua_State* L)
	{
		ifistrue(objectExists);
		
		Y = lua_tonumber(L,-1);
		irr_vector.Y = lua_tonumber(L,-1);
		return 0;
		
		endifisvalid();
	}
}
