#include "roket3d.h"
#include "E.Unmanaged/GUI/gui.h"

void GUIInitalize (lua_State * L)
{
	lua_newtable(L);
	lua_setglobal(L,"GUI");
	
	/* Register all classes here */
	Luna < gui::RGUIElement >::Register		(L,"GUI");
	Luna < gui::RGUIEnvironment >::Register		(L,"GUI");
	Luna < gui::RGUIButton >::Register		(L,"GUI");
	Luna < gui::RGUIEditBox >::Register		(L,"GUI");
	Luna < gui::RGUIStaticText >::Register		(L,"GUI");
	Luna < gui::RGUICheckbox >::Register		(L,"GUI");
	Luna < gui::RGUICombobox >::Register		(L,"GUI");
	Luna < gui::RGUIFileOpenDialog >::Register	(L,"GUI");
	Luna < gui::RGUIWindow >::Register		(L,"GUI");
	Luna < gui::RGUIMeshViewer >::Register		(L,"GUI");
	Luna < gui::RGUIScrollBar >::Register		(L,"GUI");
	Luna < gui::RGUIListbox >::Register		(L,"GUI");
	Luna < gui::RGUIImage >::Register		(L,"GUI");
	Luna < gui::RGUISkin >::Register		(L,"GUI");
	Luna < gui::RGUIFont >::Register		(L,"GUI");
}
