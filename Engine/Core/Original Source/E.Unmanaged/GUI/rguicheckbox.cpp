#include "roket3d.h"
#include "E.Unmanaged/GUI/rguicheckbox.h"

namespace gui
{
	const char *RGUICheckbox::className = TYPE_RGUICHECKBOX;
	const Luna < RGUICheckbox >::FunctionType RGUICheckbox::Functions[] = {
		{ "getID",  &RGUICheckbox::getID },
  { "getText",  &RGUICheckbox::getText },
  { "isEnabled",  &RGUICheckbox::isEnabled },
  { "isPointInside",  &RGUICheckbox::isPointInside },
  { "isVisible",  &RGUICheckbox::isVisible },
  { "setEnabled",  &RGUICheckbox::setEnabled },
  { "setID",  &RGUICheckbox::setID },
  { "setText",  &RGUICheckbox::setText },
  { "setPos",  &RGUICheckbox::setPos },
  { "setVisible",  &RGUICheckbox::setVisible },
  { "setName",  &RGUICheckbox::setName },
  { "getName",  &RGUICheckbox::getName },
  { "destroy",  &RGUICheckbox::destroy },
  // NOTE Begin custom functions
  { "setChecked",  &RGUICheckbox::setChecked },
  { "isChecked",  &RGUICheckbox::isChecked },
  {0}
	};
	const Luna < RGUICheckbox >::PropertyType RGUICheckbox::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUICheckbox::RGUICheckbox(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUICheckbox::isChecked(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,extended_self->isChecked());
		return 1;
		
		endifisvalid();
	}
	
	int RGUICheckbox::setChecked(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setChecked(((bool)lua_toboolean(L,2)));
		return 0;
		
		endifisvalid();
	}
	
	void RGUICheckbox::setElement(irr::gui::IGUICheckBox* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUICheckBox* RGUICheckbox::getElement()
	{
		return extended_self;
	}
}
