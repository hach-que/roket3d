#include "roket3d.h"
#include "rcolor.h"

namespace core
{
	const char *RColor::className = TYPE_COLOR;
	const Luna < RColor >::FunctionType RColor::Functions[] = {
		{"getInterpolated", &RColor::getInterpolated},
  {"getInterpolatedQuadratic", &RColor::getInterpolatedQuadratic},
  {"copyFrom", &RColor::copyFrom},
  {"set", &RColor::set},
  // metafunctions like __add, __sub, etc...
  //{"__eq", &RColor::equals},
  {0}
	};
	const Luna < RColor >::PropertyType RColor::Properties[] = {
		{"Alpha", &RColor::getAlpha, &RColor::setAlpha},
  {"Red", &RColor::getRed, &RColor::setRed},
  {"Blue", &RColor::getBlue, &RColor::setBlue},
  {"Green", &RColor::getGreen, &RColor::setGreen},
  {"Average", &RColor::getAverage, &RColor::setAverage},
  {"Luminance", &RColor::getLuminance, &RColor::setLuminance},
  {0}
	};
	
	RColor::RColor(lua_State* L, bool instantedByUser)
	{
		int Alpha = getmandatoryargument_cNumber(L,0);
		int Red = getmandatoryargument_cNumber(L,1);
		int Green = getmandatoryargument_cNumber(L,2);
		int Blue = getmandatoryargument_cNumber(L,3);
		irr_color = irr::video::SColor(Alpha,Red,Green,Blue);
		objectExists = true;
	}
	
	int RColor::setObject(irr::video::SColor object)
	{
		irr_color = object;
		objectExists = true;
		return 0;
	}
	
	irr::video::SColor RColor::getObject()
	{
		if (objectExists)
			return irr_color;
		else
			return irr::video::SColor(0,0,0,0);
	}
	
	irr::video::SColorf RColor::getObjectFloat()
	{
		irr::video::SColorf newcolor;
		if (objectExists)
		{
			newcolor.set(irr_color.getRed()/255,irr_color.getGreen()/255,irr_color.getBlue()/255,irr_color.getAlpha()/255);
			return newcolor;
		}
		else
		{
			newcolor.set(0,0,0,0);
			return newcolor;
		}
	}

	int RColor::getInterpolated(lua_State* L)
	{
		ifistrue(objectExists);
		
		double d = getmandatoryargument_cNumber(L,2);
		
		// Get the other object
		core::RColor *othercolor = Luna<core::RColor>::check(L, 1);
		
		// Perform operations
		core::RColor* color = Luna < core::RColor >::createNew(L);
		irr::video::SColor newcol = irr_color.getInterpolated(othercolor->getObject(),d);
		color->setObject(newcol);
		
		return 1;
		
		endifisvalid();
	}
	
	int RColor::getInterpolatedQuadratic(lua_State* L)
	{
		ifistrue(objectExists);
		
		double d = getmandatoryargument_cNumber(L,3);
		
		// Get the other objects
		core::RColor *othercolor = Luna<core::RColor>::check(L, 1);
		core::RColor *othercolor2 = Luna<core::RColor>::check(L, 2);
		
		// Perform operations
		core::RColor* color = Luna < core::RColor >::createNew(L);
		irr::video::SColor newcol = irr_color.getInterpolated_quadratic(othercolor->getObject(),othercolor2->getObject(),d);
		color->setObject(newcol);
		
		return 1;
		
		endifisvalid();
	}
	
	int RColor::copyFrom(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RColor *set_color = Luna<core::RColor>::check(L, 1);
		
		// copy values from the other vector
		irr_color = set_color->getObject();
		
		return 0;
		
		endifisvalid();
	}
	
	int RColor::set(lua_State* L)
	{
		ifistrue(objectExists);

		// TODO: Check argument check is correct.
		int Alpha = getmandatoryargument_cNumber(L,1);
		int Red = getmandatoryargument_cNumber(L,2);
		int Green = getmandatoryargument_cNumber(L,3);
		int Blue = getmandatoryargument_cNumber(L,4);

		irr_color.setRed(Red);
		irr_color.setGreen(Green);
		irr_color.setBlue(Blue);
		irr_color.setAlpha(Alpha);

		// set values
		//irr_vector.set(setX,setY,setZ);
		
		return 0;
		
		endifisvalid();
	}
	
	// **************** BEGIN PROPERTY FUNCTIONS ***************************
	
	int RColor::getRed(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_color.getRed());

		return 1;
		
		endifisvalid();
	}
	
	int RColor::getGreen(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_color.getGreen());
		return 1;
		
		endifisvalid();
	}
	
	int RColor::getBlue(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_color.getBlue());
		return 1;
		
		endifisvalid();
	}

	int RColor::getAlpha(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_color.getAlpha());
		return 1;
		
		endifisvalid();
	}

	int RColor::getAverage(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_color.getAverage());
		return 1;
		
		endifisvalid();
	}

	int RColor::getLuminance(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_color.getLuminance());
		return 1;
		
		endifisvalid();
	}
	
	int RColor::setRed(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr_color.setRed(lua_tonumber(L,-1));
		return 0;
		
		endifisvalid();
	}
	
	int RColor::setGreen(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr_color.setGreen(lua_tonumber(L,-1));
		return 0;
		
		endifisvalid();
	}

	int RColor::setBlue(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr_color.setBlue(lua_tonumber(L,-1));
		return 0;
		
		endifisvalid();
	}

	int RColor::setAlpha(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr_color.setAlpha(lua_tonumber(L,-1));
		return 0;
		
		endifisvalid();
	}

	int RColor::setAverage(lua_State* L)
	{
		ifistrue(objectExists);
		
		debug("Average is a readonly property.",LVL_ERR,L);
		return 0;
		
		endifisvalid();
	}

	int RColor::setLuminance(lua_State* L)
	{
		ifistrue(objectExists);
		
		debug("Luminance is a readonly property.",LVL_ERR,L);
		return 0;
		
		endifisvalid();
	}
}
