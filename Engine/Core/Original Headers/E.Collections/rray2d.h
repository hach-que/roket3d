#ifndef HEADER_RRAY2D
#define HEADER_RRAY2D
namespace core
{
	class RRay2D : public RObject
	{
		
		// This is the 2D ray class.  It provides methods for returning both
		// Irrlicht and Newton vectors (in C++), and is used to pass rays in
		// Lua.
		
		irr::core::line2df irr_line;
		bool objectExists;
		
		// More accurate precision for points.
		irr::core::vector2d<double> start;
		irr::core::vector2d<double> end;

		public:
			RRay2D(lua_State* L, bool instantedByUser);
			int getClosestPoint(lua_State* L);
			int getLength(lua_State* L);
			int getLengthSQ(lua_State* L);
			int getMiddle(lua_State* L);
			int getVector(lua_State* L);
			int isPointBetweenStartAndEnd(lua_State* L);
			
			int copyFrom(lua_State* L);
			int set(lua_State* L);
			
			// metaoperators
			int _equals(lua_State* L);
			int _add(lua_State* L);
			int _sub(lua_State* L);
			
			// property functions
			int setStart(lua_State* L);
			int setEnd(lua_State* L);
			int getStart(lua_State* L);
			int getEnd(lua_State* L);

			int setObject(irr::core::line2df object);
			irr::core::line2df getObject();

			static const char *className;
			static const Luna < RRay2D >::FunctionType Functions[];
			static const Luna < RRay2D >::PropertyType Properties[];
	};
}
#endif