#ifndef HEADER_RGUIBUTTON
#define HEADER_RGUIBUTTON
namespace gui
{
	class RGUIButton : public RGUIElement
	{
		irr::gui::IGUIButton * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUIButton (lua_State * L, bool instantedByUser);
			int isPressed(lua_State* L);
			int setPressed(lua_State* L);
			void setElement(irr::gui::IGUIButton* elem);
			irr::gui::IGUIButton* getElement();
			
			static const char *className;
			static const Luna < RGUIButton >::FunctionType Functions[];
			static const Luna < RGUIButton >::PropertyType Properties[];
	};
}
#endif
