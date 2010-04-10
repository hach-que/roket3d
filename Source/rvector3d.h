#ifndef HEADER_RVECTOR3D
#define HEADER_RVECTOR3D
namespace core
{
	class RVector3D : public RObject
	{
		
		// This is the 3D vector class.  It provides methods for returning both
		// Irrlicht and Newton vectors (in C++), and is used to pass vectors in
		// Lua.
		
		cIrrVector3D irr_vector;
		bool objectExists;

		// More accurate precision for X, Y, Z.
		double X;
		double Y;
		double Z;
		
		public:
			RVector3D(lua_State* L, bool instantedByUser);
			int crossProduct(lua_State* L);
			int dotProduct(lua_State* L);
			int equals(lua_State* L);
			int getAs4Values(lua_State* L);
			int getDistanceFrom(lua_State* L);
			int getDistanceFromSQ(lua_State* L);
			int getHorizontalAngle(lua_State* L);
			int getInterpolated(lua_State* L);
			int getInterpolatedQuadratic(lua_State* L);
			int getLength(lua_State* L);
			int getLengthSQ(lua_State* L);
			int invert(lua_State* L);
			int isBetweenPoints(lua_State* L);
			int normalize(lua_State* L);
			int rotateXYBy(lua_State* L);
			int rotateXZBy(lua_State* L);
			int rotateYZBy(lua_State* L);
			int copyFrom(lua_State* L);
			int set(lua_State* L);
			int setLength(lua_State* L);
			
			int setX(lua_State* L);
			int setY(lua_State* L);
			int setZ(lua_State* L);
			int getX(lua_State* L);
			int getY(lua_State* L);
			int getZ(lua_State* L);

			int setObject(cIrrVector3D object);
			int setObjectDouble(irr::core::vector3d<double> object);
			cIrrVector3D getObject();
			irr::core::vector3d<double> getObjectDouble();

			static const char *className;
			static const Luna < RVector3D >::FunctionType Functions[];
			static const Luna < RVector3D >::PropertyType Properties[];
	};
}
#endif
