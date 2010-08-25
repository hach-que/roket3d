#include "roket3d.h"
#include "rcursorcontrol.h"

namespace core
{
	const char *RCursorControl::className = TYPE_RCURSORCONTROL;
	const Luna < RCursorControl >::FunctionType RCursorControl::Functions[] = {
		{"setPosition", &RCursorControl::setPosition},
//  {"setRelativePosition", &RCursorControl::setPosition},
  {"getPosition", &RCursorControl::getPosition},
  {"getRelativePosition", &RCursorControl::getRelativePosition},
		{0}
	};
	const Luna < RCursorControl >::PropertyType RCursorControl::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RCursorControl::RCursorControl(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RCursorControls.  You must collect one from an RDevice.",LVL_ERR,L);
	}
	
	int RCursorControl::setObject(irr::gui::ICursorControl* object)
	{
		obj=object;
		objectExists=true;
		return 0;
	}
	
	irr::gui::ICursorControl* RCursorControl::getObject()
	{
		if (objectExists)
			return obj;
		else
			return NULL;
	}
	
	int RCursorControl::setPosition(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get object
		core::RVector2D* position = Luna<core::RVector2D>::check(L, 1);

		obj->setPosition((irr::s32)position->getObject().X,(irr::s32)position->getObject().Y);
		return 0;
		
		endifisvalid();
	}
/*	
	int RCursorControl::setRelativePosition(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get object
		core::RVector2D *position = Luna<core::RVector2D>::check(L, 1);

		obj->setRelativePosition((irr::s32)position.X,(irr::s32)position.Y);
		return 0;
		
		endifisvalid();
	}
*/	
	int RCursorControl::getPosition(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RVector2D* vector = Luna < core::RVector2D >::createNew(L);
		irr::core::position2d<irr::s32> pos = obj->getPosition();
		cIrrVector2D newvec(pos.X,pos.Y);
		vector->setObject(newvec);

		return 1;
		
		endifisvalid();
	}
	
	int RCursorControl::getRelativePosition(lua_State* L)
	{
		ifistrue(objectExists);
		
		core::RVector2D* vector = Luna < core::RVector2D >::createNew(L);
		irr::core::position2d<cIrrNumber> pos = obj->getRelativePosition();
		cIrrVector2D newvec(pos.X,pos.Y);
		vector->setObject(newvec);
		return 1;
		
		endifisvalid();
	}
	
	/*
	lua_newtable(L);
	lua_pushstring(L, "x"); lua_pushnumber(L, point.X); lua_settable (L, -3); lua_pop(L, 2);
	lua_pushstring(L, "y"); lua_pushnumber(L, point.Y); lua_settable (L, -3); lua_pop(L, 2);
	*/
}
