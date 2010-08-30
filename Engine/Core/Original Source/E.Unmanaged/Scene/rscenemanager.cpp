#include "roket3d.h"
#include "E.Unmanaged/Scene/rscenemanager.h"

namespace scene
{
	const char *RSceneManager::className = TYPE_RSCENEMANAGER;
	const Luna < RSceneManager >::FunctionType RSceneManager::Functions[] = {
		{"drawAll", &RSceneManager::drawAll},
  {"drawAllNoShader", &RSceneManager::drawAllNoShader},
  {"getMesh", &RSceneManager::getMesh},
  {"getHillPlaneMesh", &RSceneManager::getHillPlaneMesh},
  {"getCollisionManager", &RSceneManager::getCollisionManager},
  {"addEmptySceneNode", &RSceneManager::addEmptySceneNode},
  {"addMeshSceneNode", &RSceneManager::addMeshSceneNode},
  {"addTerrainSceneNode", &RSceneManager::addTerrainSceneNode},
  {"addCubeSceneNode", &RSceneManager::addCubeSceneNode},
  {"addSphereSceneNode", &RSceneManager::addSphereSceneNode},
  {"addCameraSceneNode", &RSceneManager::addCameraSceneNode},
  {"addLightSceneNode", &RSceneManager::addLightSceneNode},
  {"addBillboardSceneNode", &RSceneManager::addBillboardSceneNode},
  {"addBillboardTextSceneNode", &RSceneManager::addBillboardTextSceneNode},
  {"addWaterSceneNode", &RSceneManager::addWaterSceneNode},
  {"addParticleSystemSceneNode", &RSceneManager::addParticleSystemSceneNode},
//  {"addCameraSceneNodeLazy", &RSceneManager::addCameraSceneNodeLazy},
  {"addCameraSceneNodeFPS", &RSceneManager::addCameraSceneNodeFPS},
//   {"addCameraSceneNodeMaya", &RSceneManager::addCameraSceneNodeMaya},
  {"getAmbientColor", &RSceneManager::getAmbientColor},
  {"setAmbientColor", &RSceneManager::setAmbientColor},
  {"getSpecialEffectsManager", &RSceneManager::getSpecialEffectsManager},
  {"setActiveCamera", &RSceneManager::setActiveCamera},
  {0}
	};
	const Luna < RSceneManager >::PropertyType RSceneManager::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RSceneManager::RSceneManager(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RSceneManagers.  You must collect one from an RDevice.",LVL_ERR,L);
	}
	
	int RSceneManager::setScene(irr::scene::ISceneManager* scene)
	{
		scn=scene;
		isSceneSet=true;
		return 0;
	}
	
	irr::scene::ISceneManager* RSceneManager::getScene()
	{
		if (isSceneSet)
			return scn;
		else
			return NULL;
	}
	
	int RSceneManager::drawAll(lua_State* L)
	{
		if (isSceneSet)
		{
#ifdef _IRR_MOD_PERPIXEL_BASIC
			//if (roketdevice->getStencilStatus())
			//	scn->drawAll();
			//else
				xeffect->update();
#else
			xeffect->update();
#endif
		}
		return 0;
	}

	int RSceneManager::drawAllNoShader(lua_State* L)
	{
		if (isSceneSet)
		{
			scn->drawAll();
		}
		return 0;
	}
	
	int RSceneManager::getMesh(lua_State* L)
	{
		if (isSceneSet)
		{
			// TODO: Check argument check is correct.
			const char* filename = getmandatoryargument_constchar(L,1);
			if (!fileExists(filename))
			{
				debug(FILE_NOT_FOUND("load mesh",filename).c_str(),LVL_ERR,L);
				return 0;
			}
			
			scene::RMesh* msh = Luna < scene::RMesh >::createNew(L);
			msh->setIrrlichtMesh(scn->getMesh(filename));
			return 1;
		}
		return 0;
	}

	int RSceneManager::getHillPlaneMesh(lua_State* L)
	{
		if (isSceneSet)
		{
			const char* meshname = getmandatoryargument_constchar(L,1);
			core::RVector2D* tileSize = Luna<core::RVector2D>::check(L,2);
			core::RVector2D* tileCount = Luna<core::RVector2D>::check(L,3);
			cNumber hillHeight = getmandatoryargument_cNumber(L,4);
			core::RVector2D* hillCount = Luna<core::RVector2D>::check(L,5);
			core::RVector2D* textureRepeat = Luna<core::RVector2D>::check(L,6);
			
			scene::RMesh* msh = Luna < scene::RMesh >::createNew(L);
			irr::core::dimension2d<irr::u32> newdim;
			newdim.Width = tileCount->getObjectAsDimension().Width;
			newdim.Height = tileCount->getObjectAsDimension().Height;
			msh->setIrrlichtMesh(
				scn->addHillPlaneMesh(
					meshname,
					tileSize->getObjectAsDimension(),
					newdim,
					0,
					hillHeight,
					hillCount->getObjectAsDimension(),
					textureRepeat->getObjectAsDimension()
					)
				);
			return 1;
		}
		return 0;
	}
	
	int RSceneManager::addTerrainSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			const char* filename = getmandatoryargument_constchar(L,1);
			if (!fileExists(filename))
			{
				debug(FILE_NOT_FOUND("load terrain",filename).c_str(),LVL_ERR,L);
				return 0;
			}
			
			core::RVector3D *vector_scale = Luna<core::RVector3D>::check(L, 2);
			cIrrVector3D scale = vector_scale->getObject();
			
			scene::RTerrainSceneNode* node = Luna < scene::RTerrainSceneNode >::createNew(L);
			irr::scene::ITerrainSceneNode* irrnode = scn->addTerrainSceneNode(filename,
										0,
										-1,
	  									cIrrVector3D(0,0,0),
										cIrrVector3D(0,0,0),
										scale
										);
			if (irrnode==NULL)
			{
				debug(INTERNAL_ERROR("create the terrain node").c_str(),LVL_ERR,L);
			}
			node->setNode(irrnode,scn, xeffect);
			node->setRoketDevice(roketdevice);

#ifdef _IRR_MOD_PERPIXEL_BASIC
			if (roketdevice->getStencilStatus())
				irrnode->addShadowVolumeSceneNode();
			else
				xeffect->addShadowToNode(irrnode,EFT_16PCF);
#else
			xeffect->addShadowToNode(irrnode,EFT_16PCF);
#endif

			core::RVector3D *vector = Luna<core::RVector3D>::check(L, 3);
			irrnode->setPosition(vector->getObject());
			vector = Luna<core::RVector3D>::check(L, 4);
			irrnode->setRotation(vector->getObject());

			node->getNode()->setID(getoptionalargument(L,5,-1));
			node->getNode()->RoketSetName(getoptionalargument(L,6,""));
			
			// setup physics for this mesh :/
			node->setupPhysics(E_TYPE_TERRAIN,physdevice->getNewtonWorld(),irrnode->getPosition(), irrnode->getMesh(), irrnode);

			return 1;
		}
		return 0;
	}
	
	int RSceneManager::addMeshSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			// NOTE: This cannot be done via Luna::check because it checks multiple types.
			lua_gettablevalue(L,2,0);
			ifdebug(luaL_testudata(L,-1,TYPE_RSCENEMESH)==NULL,INVALID_ARGUMENT(TYPE_RSCENEMESH,1).c_str(),LVL_ERR,L);
			scene::RMesh *msh = *((scene::RMesh **)lua_touserdata(L, -1));
			
			scene::RMeshSceneNode* node = Luna < scene::RMeshSceneNode >::createNew(L);
			irr::scene::IAnimatedMesh* imsh = msh->getIrrlichtMesh();
			if (imsh==NULL)
			{
				debug(INTERNAL_ERROR("retrieve mesh from RMesh").c_str(),LVL_ERR,L);
			}
			irr::scene::IAnimatedMeshSceneNode* irrnode = scn->addAnimatedMeshSceneNode(imsh);
			if (irrnode==NULL)
			{
				debug(INTERNAL_ERROR("create the mesh node").c_str(),LVL_ERR,L);
			}
			node->setNode(irrnode, xeffect);
			node->setRoketDevice(roketdevice);

#ifdef _IRR_MOD_PERPIXEL_BASIC
			if (roketdevice->getStencilStatus())
				irrnode->addShadowVolumeSceneNode();
			else
				xeffect->addShadowToNode(irrnode,EFT_16PCF);
#else
			xeffect->addShadowToNode(irrnode,EFT_16PCF);
#endif
			
			core::RVector3D *vector = Luna<core::RVector3D>::check(L, 2);
			irr::core::vector3df position = vector->getObject();
			irrnode->setPosition(position);
			vector = Luna<core::RVector3D>::check(L, 3);
			irrnode->setRotation(vector->getObject());
			
			node->getNode()->setID(getoptionalargument(L,4,-1));
			node->getNode()->RoketSetName(getoptionalargument(L,5,""));

			// setup physics for this mesh :/
			node->setupPhysics(E_TYPE_MESH,physdevice->getNewtonWorld(),position, imsh);
			
			return 1;
		}
		return 0;
	}

	int RSceneManager::addOctTreeSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			// get arguments
			scene::RMesh* mesh = Luna<scene::RMesh>::check(L,1);
			cNumber maximumPolygons = getmandatoryargument_cNumber(L,2);
			core::RVector3D* position = Luna<core::RVector3D>::check(L, 3);
			core::RVector3D* rotation = Luna<core::RVector3D>::check(L, 4);
			core::RVector3D* scale = Luna<core::RVector3D>::check(L, 5);
			int id = getoptionalargument(L,6,-1);
			const char* name = getoptionalargument(L,7,"");

			scene::RSceneNode* node = Luna < scene::RSceneNode >::createNew(L);
			irr::scene::IAnimatedMesh* imsh = mesh->getIrrlichtMesh();
			if (imsh==NULL)
			{
				debug(INTERNAL_ERROR("retrieve mesh from RMesh").c_str(),LVL_ERR,L);
			}
			irr::scene::ISceneNode* irrnode = scn->addOctTreeSceneNode(imsh,0,id,maximumPolygons);
			if (irrnode==NULL)
			{
				debug(INTERNAL_ERROR("create the mesh node").c_str(),LVL_ERR,L);
			}

			node->setNode(irrnode, xeffect);
			node->setRoketDevice(roketdevice);
			node->getNode()->RoketSetName(name);

#ifdef _IRR_MOD_PERPIXEL_BASIC
			if (!roketdevice->getStencilStatus())
				xeffect->addShadowToNode(irrnode,EFT_16PCF);
#else
			xeffect->addShadowToNode(irrnode,EFT_16PCF);
#endif

			// setup physics for this mesh :/
			node->setupPhysics(E_TYPE_MESH,physdevice->getNewtonWorld(),position->getObject(), imsh);
			
			return 1;
		}
		return 0;
	}
	
	int RSceneManager::addCameraSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			core::RVector3D *position = Luna<core::RVector3D>::check(L, 1);
			core::RVector3D *lookat = Luna<core::RVector3D>::check(L, 2);
			
			scene::RSceneCamera* cam = Luna < scene::RSceneCamera >::createNew(L);
			int id = getoptionalargument(L,3,-1);
			const char* name = getoptionalargument(L,4,"");
			
			cIrrVector3D src = position->getObject();
			cIrrVector3D dest = lookat->getObject();
			
			irr::scene::ICameraSceneNode* icam = scn->addCameraSceneNode(0,position->getObject(),lookat->getObject(),id);
			icam->RoketSetName(name);
			cam->setNode(icam, xeffect);
			cam->setRoketDevice(roketdevice);
			
			return 1;
		}
		return 0;
	}
	
	/*
	int RSceneManager::addCameraSceneNodeLazy(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RSceneLazyCamera* cam = Luna < scene::RSceneLazyCamera >::createNew(L);	

			// get the node to follow
			lua_gettablevalue(L,2,0);
			bool invalid;
			invalid=(luaL_testudata(L,-1,TYPE_RSCENEMESHNODE)==NULL && luaL_testudata(L,-1,TYPE_RSCENENODE)==NULL);
			ifdebug(invalid,INVALID_ARGUMENT(TYPE_RSCENEMESHNODE,1),LVL_ERR,L);
			scene::RSceneNode *inode = *((scene::RSceneNode **)lua_touserdata(L, -1));
			
			lua_pop(L,1);

			int id = getoptionalargument(L,3,-1);
			const char* name = getoptionalargument(L,4,"");
			
			cam->initalize(scn,inode->getNode(),id,name);
			
			return 1;
		}
		return 0;
	}
	*/
	
	int RSceneManager::addCubeSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RSceneNode* node = Luna < scene::RSceneNode >::createNew(L);
			cIrrVector3D position = cIrrVector3D(0,0,0);
			cIrrVector3D rotation = cIrrVector3D(0,0,0);
			cIrrVector3D size = cIrrVector3D(10,10,10);
			int id = getoptionalargument(L,4,-1);
			const char* name = getoptionalargument(L,5,"");
			
			core::RVector3D *vector = Luna<core::RVector3D>::check(L, 1);
			position = vector->getObject();
			vector = Luna<core::RVector3D>::check(L, 2);
			rotation = vector->getObject();
			vector = Luna<core::RVector3D>::check(L, 3);
			size = vector->getObject();
			
			irr::scene::IMeshSceneNode* inode = scn->addCubeSceneNode(1,0,id,position,rotation,size);

#ifdef _IRR_MOD_PERPIXEL_BASIC
			if (roketdevice->getStencilStatus())
				inode->addShadowVolumeSceneNode();
			else
				xeffect->addShadowToNode(inode,EFT_16PCF);
#else
			xeffect->addShadowToNode(inode,EFT_16PCF);
#endif

			//setupNodePerPixelShading(inode); // setup material type

			inode->RoketSetName(name);
			node->setNodeAdvanced(inode,xeffect);
			node->setRoketDevice(roketdevice);
			
			// physics setup
			node->setupPhysics(E_TYPE_CUBE,physdevice->getNewtonWorld(),position, NULL);

			return 1;
		}
		return 0;
	}
	
	int RSceneManager::addLightSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RLightSceneNode* node = Luna < scene::RLightSceneNode >::createNew(L);
			core::RVector3D* position = Luna<core::RVector3D>::check(L, 1);
			core::RColor* color = Luna<core::RColor>::check(L, 2);
			float nearValue = getmandatoryargument_cNumber(L, 3);
			float farValue = getmandatoryargument_cNumber(L, 4);
			int id = getoptionalargument(L,5,-1);
			const char* name = getoptionalargument(L,6,"");
			
			irr::scene::ILightSceneNode* inode = scn->addLightSceneNode(
				0,
				position->getObject(),
				color->getObjectFloat(),
				farValue,
				id);
			
			//setupNodePerPixelShading(inode); // setup material type

			inode->RoketSetName(name);
			node->setNode(inode, xeffect);
			node->setRoketDevice(roketdevice);

			if (!roketdevice->getStencilStatus())
				node->setupShadows(position->getObject(),roketdevice,xeffect,color->getObject(),nearValue,farValue);
			else
				node->setRoketDevice(roketdevice);

			// physics setup
			node->setupPhysics(E_TYPE_IGNORE,physdevice->getNewtonWorld(),position->getObject(), NULL);
			
			return 1;
		}
		return 0;
	}

	int RSceneManager::addSphereSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RSceneNode* node = Luna < scene::RSceneNode >::createNew(L);
			cIrrVector3D position = cIrrVector3D(0,0,0);
			cIrrVector3D rotation = cIrrVector3D(0,0,0);
			cIrrVector3D size = cIrrVector3D(10,10,10);
			int id = getoptionalargument(L,5,-1);
			const char* name = getoptionalargument(L,6,"");
			int polycount = getoptionalargument(L,4,16);
			
			core::RVector3D *vector = Luna<core::RVector3D>::check(L, 1);
			position = vector->getObject();
			vector = Luna<core::RVector3D>::check(L, 2);
			rotation = vector->getObject();
			vector = Luna<core::RVector3D>::check(L, 3);
			size = vector->getObject();
			
			irr::scene::IMeshSceneNode* inode = scn->addSphereSceneNode(1,polycount,0,id,position,rotation,size);

#ifdef _IRR_MOD_PERPIXEL_BASIC
			if (roketdevice->getStencilStatus())
				inode->addShadowVolumeSceneNode();
			else
				xeffect->addShadowToNode(inode,EFT_16PCF);
#else
			xeffect->addShadowToNode(inode,EFT_16PCF);
#endif

			//setupNodePerPixelShading(inode); // setup material type

			inode->RoketSetName(name);
			node->setNodeAdvanced(inode,xeffect);
			node->setRoketDevice(roketdevice);
			inode->updateAbsolutePosition();

			// physics setup
			node->setupPhysics(E_TYPE_SPHERE,physdevice->getNewtonWorld(),position, NULL);
			
			return 1;
		}
		return 0;
	}

	int RSceneManager::addWaterSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RSceneNode* node = Luna < scene::RSceneNode >::createNew(L);
			scene::RMesh* mesh = Luna < scene::RMesh >::check(L,1);
			cNumber waveHeight = getmandatoryargument_cNumber(L,2);
			cNumber waveSpeed = getmandatoryargument_cNumber(L,3);
			cNumber waveLength = getmandatoryargument_cNumber(L,4);
			core::RVector3D* position = Luna < core::RVector3D >::check(L,5);
			core::RVector3D* rotation = Luna < core::RVector3D >::check(L,6);
			core::RVector3D* scale = Luna < core::RVector3D >::check(L,7);
			int id = getoptionalargument(L,8,-1);
			const char* name = getoptionalargument(L,9,"");
			
			irr::scene::ISceneNode* inode = scn->addWaterSurfaceSceneNode(
				mesh->getIrrlichtMesh()->getMesh(0),
				waveHeight,
				waveSpeed,
				waveLength,
				0,
				id,
				position->getObject(),
				rotation->getObject(),
				scale->getObject());
/*
#ifdef _IRR_MOD_PERPIXEL_BASIC
			if (roketdevice->getStencilStatus())
				inode->addShadowVolumeSceneNode();
			else
				xeffect->addShadowToNode(inode,EFT_16PCF);
#else
			xeffect->addShadowToNode(inode,EFT_16PCF);
#endif
*/
			inode->RoketSetName(name);
			node->setNode(inode,xeffect);
			node->setRoketDevice(roketdevice);
			inode->updateAbsolutePosition();

			// physics setup
			node->setupPhysics(E_TYPE_IGNORE,physdevice->getNewtonWorld(),position->getObject(), NULL);
			
			return 1;
		}
		return 0;
	}
	
	int RSceneManager::getCollisionManager(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RSceneCollisionManager* cmgr = Luna < scene::RSceneCollisionManager >::createNew(L);
			cmgr->setSceneManager(scn);
			cmgr->setCollisionManager(scn->getSceneCollisionManager());
			return 1;
		}
		return 0;
	}

	int RSceneManager::getSpecialEffectsManager(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RSceneSpecialEffects* sfxmgr = Luna < scene::RSceneSpecialEffects >::createNew(L);
			sfxmgr->setObject(xeffect,roketdevice);
			return 1;
		}
		return 0;
	}
	
	
	int RSceneManager::addCameraSceneNodeFPS(lua_State* L)
	{
		if (isSceneSet)
		{
			// f32 rotateSpeed=100, f32 moveSpeed=500, s32 id=-1, SKeyMap *keyMapArray=0, s32 keyMapSize=0, bool noVerticalMovement=false, f32 jumpSpeed=0.f
			scene::RSceneCamera* cam = Luna < scene::RSceneCamera >::createNew(L);
			int rotateSpeed = getoptionalargument(L,1,100);
			int moveSpeed = getoptionalargument(L,2,500);
			int vertMovementAllowed = getoptionalargument(L,3,false);
			int jumpSpeed = getoptionalargument(L,4,0);
			int id = getoptionalargument(L,5,-1);
			const char* name = getoptionalargument(L,6,"");
			
			irr::scene::ICameraSceneNode* icam = scn->addCameraSceneNodeFPS(0,rotateSpeed,moveSpeed,id,0,0,vertMovementAllowed,jumpSpeed);
			icam->RoketSetName(name);
			cam->setNode(icam, xeffect);
			cam->setRoketDevice(roketdevice);

			cam->setupPhysics(E_TYPE_IGNORE,physdevice->getNewtonWorld(),irr::core::vector3df(0,0,0), NULL);
			
			return 1;
		}
		return 0;
	}
	
	int RSceneManager::addEmptySceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RSceneNode* node = Luna < scene::RSceneNode >::createNew(L);
			node->setNode(scn->addEmptySceneNode(),xeffect);
			node->getNode()->setID(getoptionalargument(L,1,-1));
			node->getNode()->RoketSetName(getoptionalargument(L,2,""));
			node->setupPhysics(E_TYPE_IGNORE,physdevice->getNewtonWorld(),irr::core::vector3df(0,0,0), NULL);
			return 1;
		}
		return 0;
	}

	int RSceneManager::addParticleSystemSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RParticleSystemSceneNode* node = Luna < scene::RParticleSystemSceneNode >::createNew(L);
			cIrrVector3D position = cIrrVector3D(0,0,0);
			cIrrVector3D rotation = cIrrVector3D(0,0,0);
			cIrrVector3D size = cIrrVector3D(10,10,10);
			int id = getoptionalargument(L,4,-1);
			const char* name = getoptionalargument(L,5,"");
			
			core::RVector3D *vector = Luna<core::RVector3D>::check(L, 1);
			position = vector->getObject();
			vector = Luna<core::RVector3D>::check(L, 2);
			rotation = vector->getObject();
			vector = Luna<core::RVector3D>::check(L, 3);
			size = vector->getObject();
			
			irr::scene::IParticleSystemSceneNode* inode = scn->addParticleSystemSceneNode(false,0,id,position,rotation,size);

#ifdef _IRR_MOD_PERPIXEL_BASIC
			if (!roketdevice->getStencilStatus())
				xeffect->addShadowToNode(inode,EFT_16PCF);
#else
			xeffect->addShadowToNode(inode,EFT_16PCF);
#endif

			//setupNodePerPixelShading(inode); // setup material type

			inode->RoketSetName(name);
			node->setNode(inode,xeffect);
			node->setRoketDevice(roketdevice);
			inode->updateAbsolutePosition();

			// physics setup
			node->setupPhysics(E_TYPE_IGNORE,physdevice->getNewtonWorld(),position, NULL);
			
			return 1;
		}
		return 0;
	}

	int RSceneManager::addBillboardSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RBillboardSceneNode* node = Luna < scene::RBillboardSceneNode >::createNew(L);
			core::RVector3D* position = Luna < core::RVector3D >::check(L,1);
			core::RVector2D* size = Luna < core::RVector2D >::check(L,2);
			int id = getoptionalargument(L,3,-1);
			const char* name = getoptionalargument(L,4,"");
			
			irr::scene::IBillboardSceneNode* inode = scn->addBillboardSceneNode(
									0,
									size->getObjectAsDimension(),
									position->getObject(),
									-1);

			inode->RoketSetName(name);
			node->setNode(inode,xeffect);
			node->setRoketDevice(roketdevice);
			
			// physics setup
			node->setupPhysics(E_TYPE_IGNORE,physdevice->getNewtonWorld(),position->getObject(), NULL);

			return 1;
		}
		return 0;
	}

	int RSceneManager::addBillboardTextSceneNode(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RBillboardTextSceneNode* node = Luna < scene::RBillboardTextSceneNode >::createNew(L);
			gui::RGUIFont* font = Luna < gui::RGUIFont >::check(L,1);
			const wchar_t* text = ConvertToWideChar(getmandatoryargument_constchar(L,2));
			core::RVector3D* position = Luna < core::RVector3D >::check(L,3);
			core::RVector2D* size = Luna < core::RVector2D >::check(L,4);
			int id = getoptionalargument(L,5,-1);
			const char* name = getoptionalargument(L,6,"");
			
			irr::scene::IBillboardTextSceneNode* inode = scn->addBillboardTextSceneNode(
									font->getFont(),
									text,
									0,
									size->getObjectAsDimension(),
									position->getObject(),
									-1);

			inode->RoketSetName(name);
			node->setNode(inode,xeffect);
			node->setRoketDevice(roketdevice);
			
			// physics setup
			node->setupPhysics(E_TYPE_IGNORE,physdevice->getNewtonWorld(),position->getObject(), NULL);

			return 1;
		}
		return 0;
	}

	int RSceneManager::getAmbientColor(lua_State* L)
	{
		if (isSceneSet)
		{
			core::RColor* col = Luna < core::RColor >::createNew(L);
			col->setObject(xeffect->getAmbientColor());
			return 1;
		}
		return 0;
	}

	int RSceneManager::setAmbientColor(lua_State* L)
	{
		if (isSceneSet)
		{
			core::RColor* col = Luna < core::RColor >::check(L,1);
			xeffect->setAmbientColor(col->getObject());
			return 0;
		}
		return 0;
	}

	int RSceneManager::setActiveCamera(lua_State* L)
	{
		if (isSceneSet)
		{
			scene::RSceneCamera* cam = Luna < scene::RSceneCamera >::check(L,1);
			scn->setActiveCamera(cam->getNode());
			return 0;
		}
		return 0;
	}

	void RSceneManager::setCPhysicsDevice(physics::Roket_PhysicsManager* pd)
	{
		physdevice = pd;
	}

	void RSceneManager::setCIrrlichtDevice(irr::IrrlichtDevice* idevice)
	{
		irrdevice = idevice;
	}

	void RSceneManager::setCXEffect(effectHandler* xe)
	{
		xeffect = xe;
	}

	void RSceneManager::setCRoketDevice(core::RDevice* rd)
	{
		roketdevice = rd;
	}
	
	void RSceneManager::setupNodePerPixelShading(irr::scene::ISceneNode * node)
	{
//		scn->addShadow
//		node->addShadowVolumeSceneNode
		/*
		irr::video::IMaterialRenderer* renderer =
			irrdevice->getVideoDriver()->getMaterialRenderer(irr::video::EMT_PARALLAX_MAP_SOLID);
		if (renderer && renderer->getRenderCapability() == 0)
			node->setMaterialType(irr::video::EMT_PARALLAX_MAP_SOLID); 
		else
			node->setMaterialType(irr::video::EMT_NORMAL_MAP_SOLID); 
		*/
	}
}
