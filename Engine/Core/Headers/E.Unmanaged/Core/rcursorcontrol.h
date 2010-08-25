#ifndef HEADER_RCURSORCONTROL
#define HEADER_RCURSORCONTROL
namespace core
{
	class RCursorControl : public RObject
	{
		irr::gui::ICursorControl * obj;
		bool objectExists;
		
		public:
			RCursorControl(lua_State* L, bool instantedByUser);
			int setPosition(lua_State* L);
			//int setRelativePosition(lua_State* L);
			int getPosition(lua_State* L);
			int getRelativePosition(lua_State* L);
			
			int setObject(irr::gui::ICursorControl* object);
			irr::gui::ICursorControl* getObject();
			
			static const char *className;
			static const Luna < RCursorControl >::FunctionType Functions[];
			static const Luna < RCursorControl >::PropertyType Properties[];
	};
}
#endif
