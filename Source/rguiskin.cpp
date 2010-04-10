#include "roket3d.h"
#include "rguiskin.h"

namespace gui
{
	const char *RGUISkin::className = TYPE_RGUISKIN;
	const Luna < RGUISkin >::FunctionType RGUISkin::Functions[] = {
		{"setFont", &RGUISkin::setFont},
		{0}
	};
	const Luna < RGUISkin >::PropertyType RGUISkin::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	void RGUISkin::setSkin(irr::gui::IGUISkin* skin)
	{
		base_self = skin;
	}
	
	irr::gui::IGUISkin* RGUISkin::getSkin()
	{
		if (skinExists)
			return base_self;
		else
			return 0;
	}
	
	int RGUISkin::setFont(lua_State* L)
	{
		gui::RGUIFont* fnt = Luna<gui::RGUIFont>::check(L,1);

		base_self->setFont(fnt->getFont());
		return 0;
	}
	
	RGUISkin::RGUISkin(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RGUISkins.  You must collect one from an RGUIEnvironment.",LVL_ERR,L);
	}
}
