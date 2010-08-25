#ifndef HEADER_RSCENEMANAGER
#define HEADER_RSCENEMANAGER
namespace scene
{
	class RSceneManager : public RObject
	{
		irr::scene::ISceneManager* scn;
		irr::IrrlichtDevice* irrdevice;
		effectHandler* xeffect;
		physics::Roket_PhysicsManager* physdevice; // link to physics device
		core::RDevice* roketdevice;
		
		public:
			RSceneManager(lua_State* L, bool instantedByUser);
			int setScene(irr::scene::ISceneManager* scene);
			int getCollisionManager(lua_State* L);
			int getSpecialEffectsManager(lua_State* L);
			int drawAll(lua_State* L);
			int drawAllNoShader(lua_State* L);
			int getMesh(lua_State* L);
			int getHillPlaneMesh(lua_State* L);
			int addMeshSceneNode(lua_State* L);
			int addOctTreeSceneNode(lua_State* L);
			int addTerrainSceneNode(lua_State* L);
			int addEmptySceneNode(lua_State* L);
			int addCubeSceneNode(lua_State* L);
			int addSphereSceneNode(lua_State* L);
			int addCameraSceneNode(lua_State* L);
			int addCameraSceneNodeFPS(lua_State* L);
			int addLightSceneNode(lua_State* L);
			int addParticleSystemSceneNode(lua_State* L);
			int addBillboardSceneNode(lua_State* L);
			int addBillboardTextSceneNode(lua_State* L);
			int addWaterSceneNode(lua_State* L);
			int setActiveCamera(lua_State* L);

			int setAmbientColor(lua_State* L);
			int getAmbientColor(lua_State* L);

			//int addCameraSceneNodeLazy(lua_State* L);
// 			int addCameraSceneNodeMaya(lua_State* L);
			irr::scene::ISceneManager* getScene();
			//irr::gui::IGUIEnvironment* getEnvC();
			
			void setCPhysicsDevice(physics::Roket_PhysicsManager* pd);
			void setCIrrlichtDevice(irr::IrrlichtDevice* idevice);
			void setCXEffect(effectHandler* xe);
			void setCRoketDevice(core::RDevice* rd);
			void setupNodePerPixelShading(irr::scene::ISceneNode * node);

			static const char *className;
			static const Luna < RSceneManager >::FunctionType Functions[];
			static const Luna < RSceneManager >::PropertyType Properties[];
			bool isSceneSet;
	};

}
#endif
