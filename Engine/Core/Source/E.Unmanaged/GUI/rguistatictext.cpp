#include "roket3d.h"
#include "E.Unmanaged/GUI/rguistatictext.h"

namespace gui
{
	const char *RGUIStaticText::className = TYPE_RGUISTATICTEXT;
// 	const char *RGUIStaticText::className = "RGUIStaticText";
	const Luna < RGUIStaticText >::FunctionType RGUIStaticText::Functions[] = {
		{ "getID",  &RGUIStaticText::getID },
  { "getText",  &RGUIStaticText::getText },
  { "isEnabled",  &RGUIStaticText::isEnabled },
  { "isPointInside",  &RGUIStaticText::isPointInside },
  { "isVisible",  &RGUIStaticText::isVisible },
  { "setEnabled",  &RGUIStaticText::setEnabled },
  { "setID",  &RGUIStaticText::setID },
  { "setText",  &RGUIStaticText::setText },
  { "setPos",  &RGUIStaticText::setPos },
  { "setVisible",  &RGUIStaticText::setVisible },
  { "setName",  &RGUIStaticText::setName },
  { "getName",  &RGUIStaticText::getName },
  { "destroy",  &RGUIStaticText::destroy },
  // NOTE Begin custom functions
  { "setWordWrap",  &RGUIStaticText::setWordWrap },
  { "isWordWrapEnabled",  &RGUIStaticText::isWordWrapEnabled },
  {0}
	};
	const Luna < RGUIStaticText >::PropertyType RGUIStaticText::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIStaticText::RGUIStaticText(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUIStaticText::isWordWrapEnabled(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushboolean(L,extended_self->isWordWrapEnabled());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIStaticText::setWordWrap(lua_State* L)
	{
		ifistrue(isElementSet);
		
		bool wordwrap = getmandatoryargument_bool(L,1);
		extended_self->setWordWrap(wordwrap);
		return 0;
		
		endifisvalid();
	}
	
	void RGUIStaticText::setElement(irr::gui::IGUIStaticText* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIStaticText* RGUIStaticText::getElement()
	{
		return extended_self;
	}
}
