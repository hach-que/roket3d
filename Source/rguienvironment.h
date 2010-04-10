#ifndef HEADER_RENVIRONMENT
#define HEADER_RENVIRONMENT
namespace gui
{
	class RGUIEnvironment : public RObject
	{
		irr::gui::IGUIEnvironment* env;
		irr::gui::IGUIElement* getParent(lua_State* L, int pos);
		
		public:
			RGUIEnvironment(lua_State* L, bool instantedByUser);
			int setEnv(irr::gui::IGUIEnvironment* enviro);
			int getFont(lua_State* L);
			int getSkin(lua_State* L);
			int drawAll(lua_State* L);
			int addButton(lua_State* L);
			int addEditBox(lua_State* L);
			int addStaticText(lua_State* L);
			int addComboBox(lua_State* L);
			int addListBox(lua_State* L);
			int addImage(lua_State* L);
			int addCheckBox(lua_State* L);
			int addFileOpenDialog(lua_State* L);
			int addWindow(lua_State* L);
			irr::gui::IGUIEnvironment* getEnvC();
			
			static const char *className;
			static const Luna < RGUIEnvironment >::FunctionType Functions[];
			static const Luna < RGUIEnvironment >::PropertyType Properties[];
			bool isEnvSet;
	};

}
#endif
