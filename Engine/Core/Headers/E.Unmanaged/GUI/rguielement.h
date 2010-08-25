#ifndef HEADER_RGUIELEMENT
#define HEADER_RGUIELEMENT
namespace gui
{
	class RGUIElement : public RObject
	{
		irr::gui::IGUIElement * base_self;
		virtual irr::gui::IGUIElement *&self() { return base_self; };
		
		protected:
			bool isElementSet;
		
		public:
			RGUIElement(lua_State * L, bool instantedByUser);
			RGUIElement();
			virtual irr::gui::IGUIElement* getElement() { return base_self; };
			
			// TODO Implement all of the RGUIElement functions.
			//int addChild(lua_State* L);
			//int bringToFront(lua_State* L);
			//int draw(lua_State* L);
			//int getAbsoluteClippingRect(lua_State* L);
			//int getAbsolutePosition(lua_State* L);
			//int getChildren(lua_State* L);
			//int getElementFromId(lua_State* L);
			//int getElementFromPoint(lua_State* L);
			int getID(lua_State* L);
			int destroy(lua_State* L);
			//int getNextElement(lua_State* L);
			//int getParent(lua_State* L);
			//int getRelativePosition(lua_State* L);
			//int getTabGroup(lua_State* L);
			//int getTabOrder(lua_State* L);
			int getText(lua_State* L);
			//int getToolTipText(lua_State* L);
			//int getType(lua_State* L);
			//int getTypeName(lua_State* L);
			int isEnabled(lua_State* L);
			//int isMyChild(lua_State* L);
			int isPointInside(lua_State* L);
			//int isSubElement(lua_State* L);
			//int isTabGroup(lua_State* L);
			//int isTabStop(lua_State* L);
			int isVisible(lua_State* L);
			// TODO Turn this into a handling system like the event handler.
			//int OnEvent(lua_State* L);
			//int OnPostRender(lua_State* L);
			//int remove(lua_State* L);
			//int removeChild(lua_State* L);
			//int setAlignment(lua_State* L);
			int setEnabled(lua_State* L);
			int setID(lua_State* L);
			//int setMaxSize(lua_State* L);
			//int setMinSize(lua_State* L);
			//int setNotClipped(lua_State* L);
			//int setSubElement(lua_State* L);
			//int setTabGroup(lua_State* L);
			//int setTabOrder(lua_State* L);
			//int setTabStop(lua_State* L);
			int setText(lua_State* L);
			int setPos(lua_State* L);
			//int setToolTipText(lua_State* L);
			int setVisible(lua_State* L);
			//int updateAbsolutePosition(lua_State* L);
			int setName(lua_State* L);
			int getName(lua_State* L);
		
			static const char *className;
			static const Luna < RGUIElement >::FunctionType Functions[];
			static const Luna < RGUIElement >::PropertyType Properties[];
	};
	
}
#endif
