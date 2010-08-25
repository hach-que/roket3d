#include "roket3d.h"
#include "E.Unmanaged/GUI/rguielement.h"
#include "E.Unmanaged/GUI/rguimeshviewer.h"

namespace gui
{
	const char *RGUIMeshViewer::className = TYPE_RGUIMESHVIEWER;
	const Luna < RGUIMeshViewer >::FunctionType RGUIMeshViewer::Functions[] = {
		{ "getID",  &RGUIMeshViewer::getID },
  { "getText",  &RGUIMeshViewer::getText },
  { "isEnabled",  &RGUIMeshViewer::isEnabled },
  { "isPointInside",  &RGUIMeshViewer::isPointInside },
  { "isVisible",  &RGUIMeshViewer::isVisible },
  { "setEnabled",  &RGUIMeshViewer::setEnabled },
  { "setID",  &RGUIMeshViewer::setID },
  { "setText",  &RGUIMeshViewer::setText },
  { "setPos",  &RGUIMeshViewer::setPos },
  { "setVisible",  &RGUIMeshViewer::setVisible },
  { "setName",  &RGUIMeshViewer::setName },
  { "getName",  &RGUIMeshViewer::getName },
  { "destroy",  &RGUIMeshViewer::destroy },
  // NOTE Begin custom functions
  {0}
	};
	const Luna < RGUIMeshViewer >::PropertyType RGUIMeshViewer::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIMeshViewer::RGUIMeshViewer(lua_State *L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	void RGUIMeshViewer::setElement(irr::gui::IGUIMeshViewer* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIMeshViewer* RGUIMeshViewer::getElement()
	{
		return extended_self;
	}
}
