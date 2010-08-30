#include "roket3d.h"
#include "E.Unmanaged/GUI/rguiimage.h"

namespace gui
{
	const char *RGUIImage::className = TYPE_RGUIIMAGE;
	const Luna < RGUIImage >::FunctionType RGUIImage::Functions[] = {
		{ "getID",  &RGUIImage::getID },
  { "getText",  &RGUIImage::getText },
  { "isEnabled",  &RGUIImage::isEnabled },
  { "isPointInside",  &RGUIImage::isPointInside },
  { "isVisible",  &RGUIImage::isVisible },
  { "setEnabled",  &RGUIImage::setEnabled },
  { "setID",  &RGUIImage::setID },
  { "setText",  &RGUIImage::setText },
  { "setPos",  &RGUIImage::setPos },
  { "setVisible",  &RGUIImage::setVisible },
  { "setName",  &RGUIImage::setName },
  { "getName",  &RGUIImage::getName },
  { "destroy",  &RGUIImage::destroy },
  // NOTE Begin custom functions
  { "setImage",  &RGUIImage::setImage },
  {0}
	};
	const Luna < RGUIImage >::PropertyType RGUIImage::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIImage::RGUIImage(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUIImage::setImage(lua_State* L)
	{
		ifistrue(isElementSet);
		
		// TODO: Check argument check is correct.
		video::RTexture* tex = Luna<video::RTexture>::check(L,1);
		
		extended_self->setImage(tex->getTexture());
		return 0;
		
		endifisvalid();
	}
	
	void RGUIImage::setElement(irr::gui::IGUIImage* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIImage* RGUIImage::getElement()
	{
		return extended_self;
	}
}
