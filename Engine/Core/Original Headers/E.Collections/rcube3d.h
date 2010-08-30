#ifndef HEADER_RCUBE3D
#define HEADER_RCUBE3D
namespace core
{
	class RCube3D : public RObject
	{
		
		// This is the 3D ray class.  It provides methods for returning both
		// Irrlicht and Newton vectors (in C++), and is used to pass rays in
		// Lua.
		
		irr::core::aabbox3df irr_cube;
		bool objectExists;
		
		// More accurate precision for points.
		irr::core::vector3d<double> minPoint;
		irr::core::vector3d<double> maxPoint;

		public:
			RCube3D(lua_State* L, bool instantedByUser);
			int addInternalBox(lua_State* L);
			int addInternalPoint(lua_State* L);
			//int classifyPlaneRelation(lua_State* L);
			int getCenter(lua_State* L);
			int getEdges(lua_State* L);
			int getExtent(lua_State* L);
			int getInterpolated(lua_State* L);
			int intersectsWithBox(lua_State* L);
			int intersectsWithLine(lua_State* L);
			int isEmpty(lua_State* L);
			int isFullInside(lua_State* L);
			int isPointInside(lua_State* L);
			int isPointTotalInside(lua_State* L);
			int repair(lua_State* L);
			
			int copyFrom(lua_State* L);
			int set(lua_State* L);
			
			// metaoperators
			int _equals(lua_State* L);
			
			// property functions
			int setMin(lua_State* L);
			int setMax(lua_State* L);
			int getMin(lua_State* L);
			int getMax(lua_State* L);

			int setObject(irr::core::aabbox3df object);
			irr::core::aabbox3df getObject();

			static const char *className;
			static const Luna < RCube3D >::FunctionType Functions[];
			static const Luna < RCube3D >::PropertyType Properties[];
	};
}
#endif