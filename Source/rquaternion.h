#ifndef HEADER_RQUATERNION
#define HEADER_RQUATERNION
namespace core
{
	class RQuaternion : public RObject
	{
		
		// This is the 3D plane class.  It provides methods for returning both
		// Irrlicht and Newton vectors (in C++), and is used to pass plane in
		// Lua.
		
		irr::core::quaternion irr_quaternion;
		bool objectExists;

		// Added precision for values
		double W;
		double X;
		double Y;
		double Z;

		public:
			RQuaternion(lua_State* L, bool instantedByUser);
			int dotProduct(lua_State* L);
			int fromAngleAxis(lua_State* L);
			int makeIdentity(lua_State* L);
			int makeInverse(lua_State* L);
			int normalize(lua_State* L);
			int rotationFromTo(lua_State* L);
			int slerp(lua_State* L);
			int toAngleAxis(lua_State* L);
			int toEuler(lua_State* L);
			
			//int copyFrom(lua_State* L);
			//int set(lua_State* L);
			//int setEuler(lua_State* L);
			//int setVector(lua_State* L);
			
			// metaoperators
			int _equals(lua_State* L);
			int _multiply(lua_State* L);
			int _add(lua_State* L);
			
			// property functions
			int setW(lua_State* L);
			int setX(lua_State* L);
			int setY(lua_State* L);
			int setZ(lua_State* L);
			int getW(lua_State* L);
			int getX(lua_State* L);
			int getY(lua_State* L);
			int getZ(lua_State* L);

			int setObject(irr::core::quaternion object);
			irr::core::quaternion getObject();

			static const char *className;
			static const Luna < RQuaternion >::FunctionType Functions[];
			static const Luna < RQuaternion >::PropertyType Properties[];
	};
}
#endif