#ifndef HEADER_RTRIANGLE3D
#define HEADER_RTRIANGLE3D
namespace core
{
	class RTriangle3D : public RObject
	{
		
		// This is the 3D triangle class.  It provides methods for returning both
		// Irrlicht and Newton vectors (in C++), and is used to pass triangles in
		// Lua.
		
		/*
		vector3d< T > 	 (const core::vector3d< T > &p) const
		T 	getArea () const
		Returns the area of the triangle.
		bool 	 (const vector3d< T > &linePoint, const vector3d< T > &lineVect, vector3d< T > &outIntersection) const
		bool 	 (const line3d< T > &line, vector3d< T > &outIntersection) const
		bool 	 (const vector3d< T > &linePoint, const vector3d< T > &lineVect, vector3d< T > &outIntersection) const
		vector3d< T > 	 () const
		plane3d< T > 	 () const
		Returns the plane of this triangle.
		bool 	 (const vector3d< T > &lookDirection) const
		bool 	 (const vector3d< T > &p) const
		bool 	 (const vector3d< T > &p) const
		bool 	 (const aabbox3d< T > &box) const
		bool 	operator!= (const triangle3d< T > &other) const
		Inequality operator.
		bool 	operator== (const triangle3d< T > &other) const
		Equality operator.
		void 	set (const core::vector3d< T > &a, const core::vector3d< T > &b, const core::vector3d< T > &c)
		sets the triangle's points 
		*/
		
		irr::core::triangle3df irr_triangle;
		bool objectExists;
		
		// More accurate precision for points.
		irr::core::vector3d<double> pointA;
		irr::core::vector3d<double> pointB;
		irr::core::vector3d<double> pointC;

		public:
			RTriangle3D(lua_State* L, bool instantedByUser);
			int closestPointOnTriangle(lua_State* L);
			int getIntersectionOfPlaneWithLine(lua_State* L);
			int getIntersectionWithLimitedLine(lua_State* L);
			int getIntersectionWithLine(lua_State* L);
			//int getNormal(lua_State* L);
			int getPlane(lua_State* L);
			int isFrontFacing(lua_State* L);
			int isPointInside(lua_State* L);
			int isPointInsideFast(lua_State* L);
			int isTotalInsideBox(lua_State* L);
			
			int copyFrom(lua_State* L);
			int set(lua_State* L);
			
			// metaoperators
			int _equals(lua_State* L);
			
			// property functions
			int setPointA(lua_State* L);
			int setPointB(lua_State* L);
			int setPointC(lua_State* L);
			int getPointA(lua_State* L);
			int getPointB(lua_State* L);
			int getPointC(lua_State* L);

			int setObject(irr::core::triangle3df object);
			irr::core::triangle3df getObject();

			static const char *className;
			static const Luna < RTriangle3D >::FunctionType Functions[];
			static const Luna < RTriangle3D >::PropertyType Properties[];
	};
}
#endif