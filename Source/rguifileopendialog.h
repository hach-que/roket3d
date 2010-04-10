#ifndef HEADER_RGUIFILEOPENDIALOG
#define HEADER_RGUIFILEOPENDIALOG
namespace gui
{
	class RGUIFileOpenDialog : public RGUIElement
	{
		irr::gui::IGUIFileOpenDialog * extended_self;
		irr::gui::IGUIElement *&self() { return (irr::gui::IGUIElement*&)extended_self; };
		
		public:
			RGUIFileOpenDialog (lua_State * L, bool instantedByUser);
			int getFilename(lua_State* L);
			void setElement(irr::gui::IGUIFileOpenDialog* elem);
			irr::gui::IGUIFileOpenDialog* getElement();
			
			static const char *className;
			static const Luna < RGUIFileOpenDialog >::FunctionType Functions[];
			static const Luna < RGUIFileOpenDialog >::PropertyType Properties[];
	};
}
#endif
