#ifndef HEADER_RGUIIMAGE
#define HEADER_RGUIIMAGE
namespace gui
{
	class RGUIImage : public RGUIElement
	{
		irr::gui::IGUIImage * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUIImage(lua_State* L, bool instantedByUser);
			int setImage(lua_State* L);
			void setElement(irr::gui::IGUIImage* elem);
			irr::gui::IGUIImage* getElement();
			
			static const char *className;
			static const Luna < RGUIImage >::FunctionType Functions[];
			static const Luna < RGUIImage >::PropertyType Properties[];
	};
}
#endif
