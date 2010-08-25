#include "roket3d.h"
#include "E.Unmanaged/GUI/rguicombobox.h"

namespace gui
{
	const char *RGUICombobox::className = TYPE_RGUICOMBOBOX;
	const Luna < RGUICombobox >::FunctionType RGUICombobox::Functions[] = {
		{ "getID",  &RGUICombobox::getID },
  { "getText",  &RGUICombobox::getText },
  { "isEnabled",  &RGUICombobox::isEnabled },
  { "isPointInside",  &RGUICombobox::isPointInside },
  { "isVisible",  &RGUICombobox::isVisible },
  { "setEnabled",  &RGUICombobox::setEnabled },
  { "setID",  &RGUICombobox::setID },
  { "setText",  &RGUICombobox::setText },
  { "setPos",  &RGUICombobox::setPos },
  { "setVisible",  &RGUICombobox::setVisible },
  { "setName",  &RGUICombobox::setName },
  { "getName",  &RGUICombobox::getName },
  { "destroy",  &RGUICombobox::destroy },
  // NOTE Begin custom functions
  { "addItem",  &RGUICombobox::addItem },
  { "clear",  &RGUICombobox::clear },
  { "getItem",  &RGUICombobox::getItem },
  { "getItemCount",  &RGUICombobox::getItemCount },
  { "getSelected",  &RGUICombobox::getSelected },
  { "removeItem",  &RGUICombobox::removeItem },
  { "setSelected",  &RGUICombobox::setSelected },
  {0}
	};
	const Luna < RGUICombobox >::PropertyType RGUICombobox::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};

	RGUICombobox::RGUICombobox(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUICombobox::getItem(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushstring(L,ConvertToChar(extended_self->getItem((irr::u32)lua_tonumber(L,2))));
		return 1;
		
		endifisvalid();
	}
	
	int RGUICombobox::getItemCount(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getItemCount());
		return 1;
		
		endifisvalid();
	}
	
	int RGUICombobox::getSelected(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getSelected());
		return 1;
		
		endifisvalid();
	}
	
	int RGUICombobox::addItem(lua_State* L)
	{
		ifistrue(isElementSet);

		// TODO: Check argument check is correct.
		extended_self->addItem(ConvertToWideChar(getmandatoryargument_constchar(L,1)));
		return 0;
		
		endifisvalid();
	}
	
	int RGUICombobox::removeItem(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->removeItem((irr::s32)lua_tonumber(L,2));
		return 0;
		
		endifisvalid();
	}
	
	int RGUICombobox::setSelected(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setSelected((irr::s32)lua_tonumber(L,2));
		return 0;
		
		endifisvalid();
	}
	
	int RGUICombobox::clear(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->clear();
		return 0;
		
		endifisvalid();
	}
	
	void RGUICombobox::setElement(irr::gui::IGUIComboBox* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIComboBox* RGUICombobox::getElement()
	{
		return extended_self;
	}
}
