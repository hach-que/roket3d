#include "roket3d.h"
#include "E.Unmanaged/GUI/rguielement.h"
#include "E.Unmanaged/GUI/rguibutton.h"

namespace gui
{
	const char *RGUIButton::className = TYPE_RGUIBUTTON;
// 	const char *RGUIButton::className = "RGUIButton";
	const Luna < RGUIButton >::FunctionType RGUIButton::Functions[] = {
		{ "getID",  &RGUIButton::getID },
  { "getText",  &RGUIButton::getText },
  { "isEnabled",  &RGUIButton::isEnabled },
  { "isPointInside",  &RGUIButton::isPointInside },
  { "isVisible",  &RGUIButton::isVisible },
  { "setEnabled",  &RGUIButton::setEnabled },
  { "setID",  &RGUIButton::setID },
  { "setText",  &RGUIButton::setText },
  { "setPos",  &RGUIButton::setPos },
  { "setVisible",  &RGUIButton::setVisible },
  { "setName",  &RGUIButton::setName },
  { "getName",  &RGUIButton::getName },
  { "destroy",  &RGUIButton::destroy },
  // NOTE Begin custom functions
  { "setPressed",  &RGUIButton::setPressed },
  { "isPressed",  &RGUIButton::isPressed },
		{0}
	};
	const Luna < RGUIButton >::PropertyType RGUIButton::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIButton::RGUIButton(lua_State *L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUIButton::isPressed(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,extended_self->isPressed());
		return 1;
		
		endifisvalid();
	}

	int RGUIButton::setPressed(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setPressed(((bool)lua_toboolean(L,2)));
		return 0;
		
		endifisvalid();
	}
	
	void RGUIButton::setElement(irr::gui::IGUIButton* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIButton* RGUIButton::getElement()
	{
		return extended_self;
	}
}
