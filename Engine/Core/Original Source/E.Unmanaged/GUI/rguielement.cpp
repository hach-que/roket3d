#include "roket3d.h"
#include "E.Unmanaged/GUI/rguielement.h"

namespace gui
{
	const char *RGUIElement::className = TYPE_RGUIELEMENT;
	const Luna < RGUIElement >::FunctionType RGUIElement::Functions[] = {
		{ "getID",  &RGUIElement::getID },
  { "getText",  &RGUIElement::getText },
  { "isEnabled",  &RGUIElement::isEnabled },
  { "isPointInside",  &RGUIElement::isPointInside },
  { "isVisible",  &RGUIElement::isVisible },
  { "setEnabled",  &RGUIElement::setEnabled },
  { "setID",  &RGUIElement::setID },
  { "setText",  &RGUIElement::setText },
  { "setPos",  &RGUIElement::setPos },
  { "setVisible",  &RGUIElement::setVisible },
  { "setName",  &RGUIElement::setName },
  { "getName",  &RGUIElement::getName },
  { "destroy",  &RGUIElement::destroy },
		{0}
	};
	const Luna < RGUIElement >::PropertyType RGUIElement::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIElement::RGUIElement()
	{
		isElementSet=false;
	}
	
	RGUIElement::RGUIElement(lua_State *L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUIElement::getID(lua_State* L)
	{
		ifistrue(isElementSet);
			
		lua_pushnumber(L,self()->getID());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIElement::destroy(lua_State* L)
	{
		ifistrue(isElementSet);
		
		self()->remove(); // remove from GUI environment
		isElementSet=false;
		return 1;
		
		endifisvalid();
	}
	
	int RGUIElement::setID(lua_State* L)
	{
		ifistrue(isElementSet);
		
		self()->setID((int)getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIElement::setPos(lua_State* L)
	{
		ifistrue(isElementSet);
		
		// TODO: Check argument check is correct.
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);

		self()->setRelativePosition(rect->getObjectS32());
		return 0;
		
		endifisvalid();
	}
	
	int RGUIElement::setText(lua_State* L)
	{
		ifistrue(isElementSet);
		
		const char* text = getmandatoryargument_constchar(L,1);
		self()->setText(ConvertToWideChar(text));
		return 0;
		
		endifisvalid();
	}

	int RGUIElement::getText(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushstring(L,ConvertToChar(self()->getText()));
		return 1;
		
		endifisvalid();
	}

	int RGUIElement::isEnabled(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,self()->isEnabled());
		return 1;
		
		endifisvalid();
	}

	int RGUIElement::isPointInside(lua_State* L)
	{
		ifistrue(isElementSet);
		
		core::RVector2D* point = Luna<core::RVector2D>::check(L,1);

		irr::core::position2d<irr::s32> point_pos;
		point_pos.X = point->getObjectAsPosition().X;
		point_pos.Y = point->getObjectAsPosition().Y;

		lua_pushboolean(L,self()->isPointInside(point_pos));
		return 1;
		
		endifisvalid();
	}

	int RGUIElement::isVisible(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,self()->isVisible());
		return 1;
		
		endifisvalid();
	}

	int RGUIElement::setEnabled(lua_State* L)
	{
		ifistrue(isElementSet);
		
		bool enabled = getmandatoryargument_bool(L,1);
		self()->setEnabled(enabled);
		return 0;
		
		endifisvalid();
	}

	int RGUIElement::setVisible(lua_State* L)
	{
		ifistrue(isElementSet);
		
		bool visible = getmandatoryargument_bool(L,1);
		self()->setVisible(visible);
		return 0;
		
		endifisvalid();
	}

	int RGUIElement::setName(lua_State* L)
	{
		ifistrue(isElementSet);
		
		const char* newname = getmandatoryargument_constchar(L,1);

		irr::core::stringc mynewname;
		mynewname = newname;
		self()->RoketSetName(mynewname);
		return 0;
		
		endifisvalid();
	}

	int RGUIElement::getName(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushstring(L,self()->RoketGetName().c_str());
		return 1;
		
		endifisvalid();
	}
}
