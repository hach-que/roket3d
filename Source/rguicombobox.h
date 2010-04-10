#ifndef HEADER_RGUICOMBOBOX
#define HEADER_RGUICOMBOBOX
namespace gui
{
	class RGUICombobox : public RGUIElement
	{
		irr::gui::IGUIComboBox * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUICombobox(lua_State* L, bool instantedByUser);
			int addItem(lua_State* L);
			int clear(lua_State* L);
			int getItem(lua_State* L);
			int getItemCount(lua_State* L);
			int getSelected(lua_State* L);
			int removeItem(lua_State* L);
			int setSelected(lua_State* L);
			void setElement(irr::gui::IGUIComboBox* elem);
			irr::gui::IGUIComboBox* getElement();
			
			static const char *className;
			static const Luna < RGUICombobox >::FunctionType Functions[];
			static const Luna < RGUICombobox >::PropertyType Properties[];
	};
}
#endif
