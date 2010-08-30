#ifndef HEADER_RGUIWINDOW
#define HEADER_RGUIWINDOW
namespace gui
{
	class RGUIWindow : public RGUIElement
	{
		irr::gui::IGUIWindow * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		irr::gui::IGUIButton * closeButton;
		bool isCloseButtonSet;
		
		public:
			RGUIWindow (lua_State * L, bool instantedByUser);
			//int isPressed(lua_State* L);
			void windowSetClosed();
			void setElement(irr::gui::IGUIWindow* elem);
			irr::gui::IGUIWindow* getElement();
			
			static const char *className;
			static const Luna < RGUIWindow >::FunctionType Functions[];
			static const Luna < RGUIWindow >::PropertyType Properties[];
	};
}
#endif
