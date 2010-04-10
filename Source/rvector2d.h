#ifndef HEADER_RVECTOR2D
#define HEADER_RVECTOR2D
namespace core
{
	class RVector2D : public RObject
	{
		
		// This is the 2D vector class.  It provides methods for returning both
		// Irrlicht and Newton vectors (in C++), and is used to pass vectors in
		// Lua.
		
		cIrrVector2D irr_vector;
		bool objectExists;
		
		// More accurate precision for X, Y, Z.
		double X;
		double Y;
		
		public:
			RVector2D(lua_State* L, bool instantedByUser);
			int dotProduct(lua_State* L);
			int equals(lua_State* L);
			int getAngle(lua_State* L);
			int getAngleTrig(lua_State* L);
			int getAngleWith(lua_State* L);
			int getDistanceFrom(lua_State* L);
			int getDistanceFromSQ(lua_State* L);
			int getInterpolated(lua_State* L);
			int getInterpolatedQuadratic(lua_State* L);
			int getLength(lua_State* L);
			int getLengthSQ(lua_State* L);
			int isBetweenPoints(lua_State* L);
			int normalize(lua_State* L);
			int rotateBy(lua_State* L);
			int copyFrom(lua_State* L);
			int set(lua_State* L);
			
			int setX(lua_State* L);
			int setY(lua_State* L);
			int getX(lua_State* L);
			int getY(lua_State* L);

			int setObject(cIrrVector2D object);
			int setObjectDouble(irr::core::vector2d<double> object);
			cIrrVector2D getObject();
			irr::core::dimension2d<cIrrNumber> getObjectAsDimension();
			irr::core::position2d<cIrrNumber> getObjectAsPosition();
			irr::core::vector2d<double> getObjectDouble();

			static const char *className;
			static const Luna < RVector2D >::FunctionType Functions[];
			static const Luna < RVector2D >::PropertyType Properties[];
	};
}
#endif
