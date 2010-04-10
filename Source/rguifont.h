#ifndef HEADER_RGUIFONT
#define HEADER_RGUIFONT
namespace gui
{
	class RGUIFont : public RObject
	{
		irr::gui::IGUIFont* base_self;
		bool fontExists;
		
		public:
			RGUIFont(lua_State* L, bool instantedByUser);
			void setFont(irr::gui::IGUIFont* font);
			irr::gui::IGUIFont* getFont();
			//int drawAll(lua_State* L);
			
			static const char *className;
			static const Luna < RGUIFont >::FunctionType Functions[];
			static const Luna < RGUIFont >::PropertyType Properties[];
	};
}
#endif
