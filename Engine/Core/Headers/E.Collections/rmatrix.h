#ifndef HEADER_RMATRIX
#define HEADER_RMATRIX
namespace core
{
	class RMatrix : public RObject
	{
		
		// This is the matrix class.  It provides methods for returning both
		// Irrlicht and Newton matrixes (in C++), and is used to pass matrixes in
		// Lua.
		
		irr::core::matrix4 irr_matrix;
		bool objectExists;

		public:
			RMatrix(lua_State* L, bool instantedByUser);
			/*int intersectsWithBox(lua_State* L);
			int intersectsWithLine(lua_State* L);
			int isEmpty(lua_State* L);
			int isFullInside(lua_State* L);
			int isPointInside(lua_State* L);
			int isPointTotalInside(lua_State* L);
			int repair(lua_State* L);*/
			
			int copyFrom(lua_State* L);
			//int set(lua_State* L);
			
			// metaoperators
			int _equals(lua_State* L);
			int _multiply(lua_State* L);
			int _add(lua_State* L);
			int _subtract(lua_State* L);
			
			// property functions
			int setTranslation(lua_State* L);
			int setRotation(lua_State* L);
			int setScale(lua_State* L);
			int getTranslation(lua_State* L);
			int getRotation(lua_State* L);
			int getScale(lua_State* L);

			int setObject(irr::core::matrix4 object);
			irr::core::matrix4 getObject();

			static const char *className;
			static const Luna < RMatrix >::FunctionType Functions[];
			static const Luna < RMatrix >::PropertyType Properties[];
	};
}
#endif