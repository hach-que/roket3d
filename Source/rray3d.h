#ifndef HEADER_RRAY3D
#define HEADER_RRAY3D
namespace core
{
	class RRay3D : public RObject
	{
		
		// This is the 3D ray class.  It provides methods for returning both
		// Irrlicht and Newton vectors (in C++), and is used to pass rays in
		// Lua.
		
		irr::core::line3df irr_line;
		bool objectExists;
		
		// More accurate precision for points.
		irr::core::vector3d<double> start;
		irr::core::vector3d<double> end;

		public:
			RRay3D(lua_State* L, bool instantedByUser);
			int getClosestPoint(lua_State* L);
			int getIntersectionWithSphere(lua_State* L);
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

			int setObject(irr::core::line3df object);
			irr::core::line3df getObject();

			static const char *className;
			static const Luna < RRay3D >::FunctionType Functions[];
			static const Luna < RRay3D >::PropertyType Properties[];
	};
}
#endif