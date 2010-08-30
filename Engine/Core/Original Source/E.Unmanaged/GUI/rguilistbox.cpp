#include "roket3d.h"
#include "E.Unmanaged/GUI/rguilistbox.h"

namespace gui
{
	const char *RGUIListbox::className = TYPE_RGUILISTBOX;
	const Luna < RGUIListbox >::FunctionType RGUIListbox::Functions[] = {
		{ "getID",  &RGUIListbox::getID },
  { "getText",  &RGUIListbox::getText },
  { "isEnabled",  &RGUIListbox::isEnabled },
  { "isPointInside",  &RGUIListbox::isPointInside },
  { "isVisible",  &RGUIListbox::isVisible },
  { "setEnabled",  &RGUIListbox::setEnabled },
  { "setID",  &RGUIListbox::setID },
  { "setText",  &RGUIListbox::setText },
  { "setPos",  &RGUIListbox::setPos },
  { "setVisible",  &RGUIListbox::setVisible },
  { "setName",  &RGUIListbox::setName },
  { "getName",  &RGUIListbox::getName },
  { "destroy",  &RGUIListbox::destroy },
  // NOTE Begin custom functions
  { "addItem",  &RGUIListbox::addItem },
  { "clear",  &RGUIListbox::clear },
  { "getItem",  &RGUIListbox::getItem },
  { "getItemCount",  &RGUIListbox::getItemCount },
  { "getSelected",  &RGUIListbox::getSelected },
  { "removeItem",  &RGUIListbox::removeItem },
  { "setSelected",  &RGUIListbox::setSelected },
  {0}
	};
	const Luna < RGUIListbox >::PropertyType RGUIListbox::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};

	RGUIListbox::RGUIListbox(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUIListbox::getItem(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushstring(L,ConvertToChar(extended_self->getListItem((irr::u32)getmandatoryargument_cNumber(L,1))));
		return 1;
		
		endifisvalid();
	}
	
	int RGUIListbox::getItemCount(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getItemCount());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIListbox::getSelected(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getSelected());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIListbox::addItem(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->addItem(ConvertToWideChar(getmandatoryargument_constchar(L,1)));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIListbox::removeItem(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->removeItem((irr::s32)getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIListbox::setSelected(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setSelected((irr::s32)getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIListbox::clear(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->clear();
		return 0;
		
		endifisvalid();
	}
	
	void RGUIListbox::setElement(irr::gui::IGUIListBox* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIListBox* RGUIListbox::getElement()
	{
		return extended_self;
	}
}
