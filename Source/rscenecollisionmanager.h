#ifndef HEADER_RSCENECOLLISIONMANAGER
#define HEADER_RSCENECOLLISIONMANAGER
namespace scene
{
	class RSceneCollisionManager : public RObject
	{
		irr::scene::ISceneCollisionManager * cmanager;
		irr::scene::ISceneManager * scn;
		bool managerExists;
		
		public:
			RSceneCollisionManager(lua_State* L, bool instantedByUser);
			void setCollisionManager(irr::scene::ISceneCollisionManager* newmanager);
			void setSceneManager(irr::scene::ISceneManager* newscn);
			irr::scene::ISceneCollisionManager* getCollisionManager();
			int getCollisionPoint(lua_State* L);
			int getRayFromScreenCoordinates(lua_State* L);
			int getSceneNodeFromCameraBB(lua_State* L);
			int getSceneNodeFromRayBB(lua_State* L);
			int getSceneNodeFromScreenCoordinatesBB(lua_State* L);
			int getScreenCoordinatesFrom3DPosition(lua_State* L);
			
			static const char *className;
			static const Luna < RSceneCollisionManager >::FunctionType Functions[];
			static const Luna < RSceneCollisionManager >::PropertyType Properties[];
	};
}
#endif
