#ifndef HEADER_RGUISTATICTEXT
#define HEADER_RGUISTATICTEXT
namespace gui
{
	class RGUIStaticText : public RGUIElement
	{
		irr::gui::IGUIStaticText * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUIStaticText(lua_State* L, bool instantedByUser);
// 			int enableOverrideColor(lua_State* L);
// 			int getOverrideColor(lua_State* L);
// 			int getOverrideFont(lua_State* L);
// 			int getTextHeight(lua_State* L);
// 			int getTextWidth(lua_State* L);
// 			int isOverrideColorEnabled(lua_State* L);
 			int isWordWrapEnabled(lua_State* L);
// 			int setBackgroundColor(lua_State* L);
// 			int setDrawBackground(lua_State* L);
// 			int setDrawBorder(lua_State* L);
// 			int setOverrideColor(lua_State* L);
// 			int setOverrideFont(lua_State* L);
// 			int setTextAlignment(lua_State* L);
 			int setWordWrap(lua_State* L);
			void setElement(irr::gui::IGUIStaticText* elem);
			irr::gui::IGUIStaticText* getElement();
			
			static const char *className;
			static const Luna < RGUIStaticText >::FunctionType Functions[];
			static const Luna < RGUIStaticText >::PropertyType Properties[];
	};
}
#endif
