#ifndef HEADER_RGUICHECKBOX
#define HEADER_RGUICHECKBOX
namespace gui
{
	class RGUICheckbox : public RGUIElement
	{
		irr::gui::IGUICheckBox * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUICheckbox(lua_State* L, bool instantedByUser);
			int setChecked(lua_State* L);
			int isChecked(lua_State* L);
			void setElement(irr::gui::IGUICheckBox* elem);
			irr::gui::IGUICheckBox* getElement();
			
			static const char *className;
			static const Luna < RGUICheckbox >::FunctionType Functions[];
			static const Luna < RGUICheckbox >::PropertyType Properties[];
	};
}
#endif
