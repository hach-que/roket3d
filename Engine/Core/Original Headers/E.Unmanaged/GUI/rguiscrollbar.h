#ifndef HEADER_RGUISCROLLBAR
#define HEADER_RGUISCROLLBAR
namespace gui
{
	class RGUIScrollBar : public RGUIElement
	{
		irr::gui::IGUIScrollBar * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUIScrollBar(lua_State* L, bool instantedByUser);
			int getLargeStep(lua_State* L);
			int getSmallStep(lua_State* L);
			int getMax(lua_State* L);
			int getPos(lua_State* L);
			int setLargeStep(lua_State* L);
			int setSmallStep(lua_State* L);
			int setMax(lua_State* L);
			int setPos(lua_State* L);
			
			void setElement(irr::gui::IGUIScrollBar* elem);
			irr::gui::IGUIScrollBar* getElement();
			
			static const char *className;
			static const Luna < RGUIScrollBar >::FunctionType Functions[];
			static const Luna < RGUIScrollBar >::PropertyType Properties[];
	};
}
#endif
