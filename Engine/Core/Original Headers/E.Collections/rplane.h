#ifndef HEADER_RPLANE3D
#define HEADER_RPLANE3D
namespace core
{
	class RPlane : public RObject
	{
		
		// This is the 3D plane class.  It provides methods for returning both
		// Irrlicht and Newton vectors (in C++), and is used to pass plane in
		// Lua.
		
		irr::core::plane3df irr_plane;
		bool objectExists;
		
		// More accurate precision for plane.
		irr::core::vector3d<double> normal;
		double D;

		public:
			RPlane(lua_State* L, bool instantedByUser);
			int classifyPointRelation(lua_State* L);
			int existsIntersection(lua_State* L);
			int getDistanceTo(lua_State* L);
			int getIntersectionWithLimitedLine(lua_State* L);
			int getIntersectionWithLine(lua_State* L);
			int getIntersectionWithPlane(lua_State* L);
			int getIntersectionWithPlanes(lua_State* L);
			int getKnownIntersectionWithLine(lua_State* L);
			int getMemberPoint(lua_State* L);
			int isFrontFacing(lua_State* L);
			int recalculateD(lua_State* L);
			
			int copyFrom(lua_State* L);
			int set(lua_State* L);
			
			// metaoperators
			int _equals(lua_State* L);
			
			// property functions
			int setD(lua_State* L);
			int setNormal(lua_State* L);
			int getD(lua_State* L);
			int getNormal(lua_State* L);

			int setObject(irr::core::plane3df object);
			irr::core::plane3df getObject();

			static const char *className;
			static const Luna < RPlane >::FunctionType Functions[];
			static const Luna < RPlane >::PropertyType Properties[];
	};
}
#endif