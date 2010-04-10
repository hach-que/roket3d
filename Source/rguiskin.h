#ifndef HEADER_RGUISKIN
#define HEADER_RGUISKIN
namespace gui
{
	class RGUISkin : public RObject
	{
		irr::gui::IGUISkin* base_self;
		bool skinExists;
		
		public:
			RGUISkin(lua_State* L, bool instantedByUser);
			void setSkin(irr::gui::IGUISkin* font);
			irr::gui::IGUISkin* getSkin();
			int setFont(lua_State* L);
			
			static const char *className;
			static const Luna < RGUISkin >::FunctionType Functions[];
			static const Luna < RGUISkin >::PropertyType Properties[];
	};
}
#endif
