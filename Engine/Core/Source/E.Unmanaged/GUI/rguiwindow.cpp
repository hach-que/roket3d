#include "roket3d.h"
#include "rguielement.h"
#include "rguiwindow.h"

namespace gui
{
	const char *RGUIWindow::className = TYPE_RGUIWINDOW;
	const Luna < RGUIWindow >::FunctionType RGUIWindow::Functions[] = {
		{ "getID",  &RGUIWindow::getID },
  { "getText",  &RGUIWindow::getText },
  { "isEnabled",  &RGUIWindow::isEnabled },
  { "isPointInside",  &RGUIWindow::isPointInside },
  { "isVisible",  &RGUIWindow::isVisible },
  { "setEnabled",  &RGUIWindow::setEnabled },
  { "setID",  &RGUIWindow::setID },
  { "setText",  &RGUIWindow::setText },
  { "setPos",  &RGUIWindow::setPos },
  { "setVisible",  &RGUIWindow::setVisible },
  { "setName",  &RGUIWindow::setName },
  { "getName",  &RGUIWindow::getName },
  { "destroy",  &RGUIWindow::destroy },
  // NOTE Begin custom functions
//   { "setPressed",  &RGUIWindow::setPressed },
//   { "isPressed",  &RGUIWindow::isPressed },
  {0}
	};
	const Luna < RGUIWindow >::PropertyType RGUIWindow::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIWindow::RGUIWindow(lua_State *L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
		isCloseButtonSet=false;
	}
	
// 	int RGUIWindow::isPressed(lua_State* L)
// 	{
// 		ifistrue(isElementSet);
// 		
// 		lua_pushboolean(L,extended_self->isPressed());
// 		return 1;
// 		
// 		endifisvalid();
// 	}
// 
// 	int RGUIWindow::setPressed(lua_State* L)
// 	{
// 		ifistrue(isElementSet);
// 		
// 		extended_self->setPressed(((bool)lua_toboolean(L,2)));
// 		return 0;
// 		
// 		endifisvalid();
// 	}
	
	void RGUIWindow::setElement(irr::gui::IGUIWindow* elem)
	{
		extended_self = elem;
		extended_self->RoketSetUserData(extended_self);
		isElementSet = true;
		closeButton = extended_self->getCloseButton();
	}
	
	void RGUIWindow::windowSetClosed()
	{
		isElementSet = false;
	}
	
	irr::gui::IGUIWindow* RGUIWindow::getElement()
	{
		return extended_self;
	}
}
