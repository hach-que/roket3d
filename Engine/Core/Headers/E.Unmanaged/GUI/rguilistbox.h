#ifndef HEADER_RGUILISTBOX
#define HEADER_RGUILISTBOX
namespace gui
{
	class RGUIListbox : public RGUIElement
	{
		irr::gui::IGUIListBox * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUIListbox(lua_State* L, bool instantedByUser);
			int addItem(lua_State* L);
			int clear(lua_State* L);
			int getItem(lua_State* L);
			int getItemCount(lua_State* L);
			int getSelected(lua_State* L);
			int removeItem(lua_State* L);
			int setSelected(lua_State* L);
			void setElement(irr::gui::IGUIListBox* elem);
			irr::gui::IGUIListBox* getElement();
			
			static const char *className;
			static const Luna < RGUIListbox >::FunctionType Functions[];
			static const Luna < RGUIListbox >::PropertyType Properties[];
	};
}
#endif
