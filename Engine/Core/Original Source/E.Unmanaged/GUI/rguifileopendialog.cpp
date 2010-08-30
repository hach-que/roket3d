#include "roket3d.h"
#include "E.Unmanaged/GUI/rguielement.h"
#include "E.Unmanaged/GUI/rguifileopendialog.h"

namespace gui
{
	const char *RGUIFileOpenDialog::className = TYPE_RGUIFILEOPENDIALOG;
	const Luna < RGUIFileOpenDialog >::FunctionType RGUIFileOpenDialog::Functions[] = {
		{ "getID",  &RGUIFileOpenDialog::getID },
  { "getText",  &RGUIFileOpenDialog::getText },
  { "isEnabled",  &RGUIFileOpenDialog::isEnabled },
  { "isPointInside",  &RGUIFileOpenDialog::isPointInside },
  { "isVisible",  &RGUIFileOpenDialog::isVisible },
  { "setEnabled",  &RGUIFileOpenDialog::setEnabled },
  { "setID",  &RGUIFileOpenDialog::setID },
  { "setText",  &RGUIFileOpenDialog::setText },
  { "setPos",  &RGUIFileOpenDialog::setPos },
  { "setVisible",  &RGUIFileOpenDialog::setVisible },
  { "setName",  &RGUIFileOpenDialog::setName },
  { "getName",  &RGUIFileOpenDialog::getName },
  { "destroy",  &RGUIFileOpenDialog::destroy },
		// NOTE Begin custom functions
  { "getFilename",  &RGUIFileOpenDialog::getFilename },
  {0}
	};
	const Luna < RGUIFileOpenDialog >::PropertyType RGUIFileOpenDialog::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIFileOpenDialog::RGUIFileOpenDialog(lua_State *L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create " << className << "s.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
		isElementSet=false;
	}
	
	int RGUIFileOpenDialog::getFilename(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushstring(L,ConvertToChar(extended_self->getFileName()));
		return 1;
		
		endifisvalid();
	}
	
	void RGUIFileOpenDialog::setElement(irr::gui::IGUIFileOpenDialog* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIFileOpenDialog* RGUIFileOpenDialog::getElement()
	{
		return extended_self;
	}
}
