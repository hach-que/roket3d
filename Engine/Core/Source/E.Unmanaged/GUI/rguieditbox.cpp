#include "roket3d.h"
#include "E.Unmanaged/GUI/rguielement.h"
#include "E.Unmanaged/GUI/rguieditbox.h"

namespace gui
{
	const char *RGUIEditBox::className = TYPE_RGUIEDITBOX;
// 	const char *RGUIEditBox::className = "RGUIEditBox";
	const Luna < RGUIEditBox >::FunctionType RGUIEditBox::Functions[] = {
		{ "getID",  &RGUIEditBox::getID },
  { "getText",  &RGUIEditBox::getText },
  { "isEnabled",  &RGUIEditBox::isEnabled },
  { "isPointInside",  &RGUIEditBox::isPointInside },
  { "isVisible",  &RGUIEditBox::isVisible },
  { "setEnabled",  &RGUIEditBox::setEnabled },
  { "setID",  &RGUIEditBox::setID },
  { "setText",  &RGUIEditBox::setText },
  { "setPos",  &RGUIEditBox::setPos },
  { "setVisible",  &RGUIEditBox::setVisible },
  { "setName",  &RGUIEditBox::setName },
  { "getName",  &RGUIEditBox::getName },
  { "destroy",  &RGUIEditBox::destroy },
  // NOTE Begin custom functions
  { "getMax",  &RGUIEditBox::getMax },
  { "isAutoScrollEnabled",  &RGUIEditBox::isAutoScrollEnabled },
  { "isMultiLineEnabled",  &RGUIEditBox::isMultiLineEnabled },
  { "isPasswordBox",  &RGUIEditBox::isPasswordBox },
  { "isWordWrapEnabled",  &RGUIEditBox::isWordWrapEnabled },
  { "setAutoScroll",  &RGUIEditBox::setAutoScroll },
  { "setMultiLine",  &RGUIEditBox::setMultiLine },
  { "setMax",  &RGUIEditBox::setMax },
  { "setPasswordBox",  &RGUIEditBox::setPasswordBox },
  { "setWordWrap",  &RGUIEditBox::setWordWrap },
  {0}
	};
	const Luna < RGUIEditBox >::PropertyType RGUIEditBox::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIEditBox::RGUIEditBox(lua_State *L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUIEditBox::isAutoScrollEnabled(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,extended_self->isAutoScrollEnabled());
		return 1;
		
		endifisvalid();
	}

	int RGUIEditBox::isMultiLineEnabled(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,extended_self->isMultiLineEnabled());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEditBox::isPasswordBox(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,extended_self->isPasswordBox());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEditBox::isWordWrapEnabled(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,extended_self->isWordWrapEnabled());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEditBox::getMax(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getMax());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEditBox::setMax(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setMax((irr::u32)getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIEditBox::setAutoScroll(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setAutoScroll(getmandatoryargument_bool(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIEditBox::setPasswordBox(lua_State* L)
	{
		ifistrue(isElementSet);
		
		// TODO: Check argument check is correct.
		extended_self->setPasswordBox(getmandatoryargument_bool(L,1),getmandatoryargument_cNumber(L,2));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIEditBox::setWordWrap(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setWordWrap(getmandatoryargument_bool(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIEditBox::setMultiLine(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setMultiLine(getmandatoryargument_bool(L,1));
		return 0;
		
		endifisvalid();
	}
	
	void RGUIEditBox::setElement(irr::gui::IGUIEditBox* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIEditBox* RGUIEditBox::getElement()
	{
		return extended_self;
	}
}
