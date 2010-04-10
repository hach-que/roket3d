#ifndef HEADER_RGUIEDITBOX
#define HEADER_RGUIEDITBOX
namespace gui
{
	class RGUIEditBox : public RGUIElement
	{
		irr::gui::IGUIEditBox * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUIEditBox (lua_State * L, bool instantedByUser);
			int getMax(lua_State* L);
			int isAutoScrollEnabled(lua_State* L);
			int isMultiLineEnabled(lua_State* L);
			int isPasswordBox(lua_State* L);
			int isWordWrapEnabled(lua_State* L);
			int setAutoScroll(lua_State* L);
			int setMultiLine(lua_State* L);
			int setMax(lua_State* L);
			int setPasswordBox(lua_State* L);
			int setWordWrap(lua_State* L);
			void setElement(irr::gui::IGUIEditBox* elem);
			irr::gui::IGUIEditBox* getElement();
			
			static const char *className;
			static const Luna < RGUIEditBox >::FunctionType Functions[];
			static const Luna < RGUIEditBox >::PropertyType Properties[];
	};
}
#endif
