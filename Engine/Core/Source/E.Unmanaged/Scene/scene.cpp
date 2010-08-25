#include "roket3d.h"
#include "E.Unmanaged/Scene/scene.h"

void SceneInitalize (lua_State * L)
{
	lua_newtable(L);
	lua_setglobal(L,"Scene");
	
	/* Register all classes here */
	Luna < scene::RSceneManager >::Register				(L,"Scene");
	Luna < scene::RMesh >::Register						(L,"Scene");
	Luna < scene::RSceneNode >::Register				(L,"Scene");
	Luna < scene::RMeshSceneNode >::Register			(L,"Scene");
	Luna < scene::RLightSceneNode >::Register			(L,"Scene");
	Luna < scene::RSceneCamera >::Register				(L,"Scene");
	Luna < scene::RSceneCollisionManager >::Register	(L,"Scene");
	Luna < scene::RTerrainSceneNode >::Register			(L,"Scene");
	Luna < scene::RBillboardSceneNode >::Register		(L,"Scene");
	Luna < scene::RBillboardTextSceneNode >::Register	(L,"Scene");
	Luna < scene::RParticleSystemSceneNode >::Register	(L,"Scene");
	
	//Luna < scene::RSceneLazyCamera >::Register		(L,"Scene");
}
