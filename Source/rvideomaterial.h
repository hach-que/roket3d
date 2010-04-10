#ifndef HEADER_RMATERIAL
#define HEADER_RMATERIAL
namespace video
{
	class RMaterial : public RObject
	{
		
		// This is the material class.  It provides methods for returning
		// Irrlicht materials (in C++), and is used to pass materials in
		// Lua.
		
		irr::video::SMaterial irr_material;
		bool objectExists;

		public:
			RMaterial(lua_State* L, bool instantedByUser);
			int applyTexture(lua_State* L);
			int applyTextureMatrix(lua_State* L);
			int applyFlag(lua_State* L);
			/*int isFullInside(lua_State* L);
			int isPointInside(lua_State* L);
			int isPointTotalInside(lua_State* L);
			int repair(lua_State* L);*/

			int setObject(irr::video::SMaterial object);
			irr::video::SMaterial getObject();

			static const char *className;
			static const Luna < RMaterial >::FunctionType Functions[];
			static const Luna < RMaterial >::PropertyType Properties[];
	};
}
#endif