#ifndef HEADER_RGUIMESHVIEWER
#define HEADER_RGUIMESHVIEWER
namespace gui
{
	class RGUIMeshViewer : public RGUIElement
	{
		irr::gui::IGUIMeshViewer * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUIMeshViewer (lua_State * L, bool instantedByUser);
			//int isPressed(lua_State* L);
			void setElement(irr::gui::IGUIMeshViewer* elem);
			irr::gui::IGUIMeshViewer* getElement();
			
			static const char *className;
			static const Luna < RGUIMeshViewer >::FunctionType Functions[];
			static const Luna < RGUIMeshViewer >::PropertyType Properties[];
	};
}
#endif
