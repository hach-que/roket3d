#ifndef HEADER_RCOLOR
#define HEADER_RCOLOR
namespace core
{
	class RColor : public RObject
	{
		
		// This is the color class.  It provides methods for returning both
		// Irrlicht color, and is used to pass colors in Lua.
		
		irr::video::SColor irr_color;
		bool objectExists;

		public:
			RColor(lua_State* L, bool instantedByUser);

			int getInterpolated(lua_State* L);
			int getInterpolatedQuadratic(lua_State* L);

			int copyFrom(lua_State* L);
			int set(lua_State* L);
			
			// metaoperators
			int _equals(lua_State* L);
			
			// property functions
			int setAlpha(lua_State* L);
			int getAlpha(lua_State* L);
			int setRed(lua_State* L);
			int getRed(lua_State* L);
			int setGreen(lua_State* L);
			int getGreen(lua_State* L);
			int setBlue(lua_State* L);
			int getBlue(lua_State* L);
			
			// readonly functions
			int setAverage(lua_State* L); // just shows an error
			int getAverage(lua_State* L);
			int setLuminance(lua_State* L); // just shows an error
			int getLuminance(lua_State* L);


			int setObject(irr::video::SColor object);
			irr::video::SColor getObject();
			irr::video::SColorf getObjectFloat();

			static const char *className;
			static const Luna < RColor >::FunctionType Functions[];
			static const Luna < RColor >::PropertyType Properties[];
	};
}
#endif