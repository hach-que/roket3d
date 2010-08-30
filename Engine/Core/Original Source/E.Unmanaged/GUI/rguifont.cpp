#include "roket3d.h"
#include "E.Unmanaged/GUI/rguifont.h"

namespace gui
{
	const char *RGUIFont::className = TYPE_RGUIFONT;
	const Luna < RGUIFont >::FunctionType RGUIFont::Functions[] = {
		//{"drawAll", &RGUIFont::drawAll},
		{0}
	};
	const Luna < RGUIFont >::PropertyType RGUIFont::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	void RGUIFont::setFont(irr::gui::IGUIFont* font)
	{
		base_self = font;
		fontExists=true;
	}
	
	irr::gui::IGUIFont* RGUIFont::getFont()
	{
		if (fontExists)
			return base_self;
		else
			return 0;
	}
	
	RGUIFont::RGUIFont(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RGUIFonts.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
	}
}
