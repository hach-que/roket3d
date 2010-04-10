#include "roket3d.h"
#include "rscenenode.h"

namespace scene
{
	const char *RSceneNode::className = TYPE_RSCENENODE;
	const Luna < RSceneNode >::FunctionType RSceneNode::Functions[] = {
		{"setPosition", &RSceneNode::setPosition},
  {"getPosition", &RSceneNode::getPosition},
  {"setRotation", &RSceneNode::setRotation},
  {"getRotation", &RSceneNode::getRotation},
  {"applyTexture", &RSceneNode::applyTexture},
  {"applyFlag", &RSceneNode::applyFlag},
  {"applyMaterialType", &RSceneNode::applyMaterialType},
  {"applyPhysicsSettings", &RSceneNode::applyPhysicsSettings},
  {"applyForce", &RSceneNode::applyForce},
  {"applyTorque", &RSceneNode::applyTorque},
  {"setParent", &RSceneNode::setParent},
  {"destroy", &RSceneNode::destroy},
  {"destroyPhysics", &RSceneNode::destroyPhysics},
  {"setName", &RSceneNode::setName},
  {"getName", &RSceneNode::getName},
  {"setID", &RSceneNode::setID},
  {"getID", &RSceneNode::getID},
  {0}
	};
	const Luna < RSceneNode >::PropertyType RSceneNode::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RSceneNode::RSceneNode()
	{
		isNodeSet=false;
		physType=E_TYPE_STATIC;
		isPrecious = true;
	}

	RSceneNode::~RSceneNode()
	{
		isNodeSet=false;
		/*
		if (isNodeSet)
		{
			// clean up
			isNodeSet=false;
			if ((void*)base_self != (void*)0xCDCDCDCD && (void*)base_self != (void*)0x00000000)
			{
				base_self->remove();
				base_self = NULL;
			}
			if ((void*)physBody != (void*)0xCDCDCDCD && (void*)base_self != (void*)0x00000000)
			{
				if (physType == E_TYPE_SPHERE || 
				physType == E_TYPE_CUBE)
				{
					physBody->deletePhysics();
					delete physBody;
					physBody = NULL;
				}
			}
		}
		*/
	}
	
	RSceneNode::RSceneNode(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RSceneNodes.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		isNodeSet=false;
		isPrecious = true; // Lua should not destroy these.
		physType=E_TYPE_STATIC;
		physBody = 0;
		self() = 0;
	}
	
	void RSceneNode::setNode(irr::scene::ISceneNode* node, effectHandler* xe)
	{
		base_self = reinterpret_cast<irr::scene::IMeshSceneNode*>(node);
		ident.type("RSceneNode");
		ident.voidPointer(this);
		base_self->RoketSetUserData(&ident);
		xeffect = xe;
		isNodeSet=true;
	}

	void RSceneNode::setRoketDevice(core::RDevice* rd)
	{
		roketdevice = rd;
	}

	void RSceneNode::setNodeAdvanced(irr::scene::IMeshSceneNode* node, effectHandler* xe)
	{
		base_self = node;
		ident.type("RSceneNode");
		ident.voidPointer(this);
		base_self->RoketSetUserData(&ident);
		xeffect = xe;
		isNodeSet=true;
	}
	
	irr::scene::ISceneNode* RSceneNode::getNode()
	{
		if (isNodeSet)
			return base_self;
		else
			return NULL;
	}

	irr::scene::IMeshSceneNode* RSceneNode::getNodeAdvanced()
	{
		if (isNodeSet)
			return base_self;
		else
			return NULL;
	}
	
	int RSceneNode::setPosition(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		
		self()->setPosition(vector3d->getObject());

		// update physics if nessicary
		if (physType == E_TYPE_SPHERE || 
			physType == E_TYPE_CUBE ||
			physType == E_TYPE_MESH ||
			physType == E_TYPE_TERRAIN)
		{
			physBody->setPosition(vector3d->getObject());
			//debug("Use forces and torque to apply smooth physics.",LVL_WARN,L);
		}

		return 0;
		
		endifisvalid();
	}

	int RSceneNode::setParent(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		if (lua_isnil(L,2))
		{
			self()->setParent(roketdevice->getIrrlichtDevice()->getSceneManager()->getRootSceneNode());
			debug("Setting scene node parent to root scene node (no parent).",LVL_WARN,L);
			return 0;
		}
		else
		{
			scene::RSceneNode* node = 0;
			node = Luna<RSceneNode>::lightcheck(L, 1);
			node = Luna<RParticleSystemSceneNode>::lightcheck(L, 1);
			node = Luna<RBillboardSceneNode>::lightcheck(L, 1);
			node = Luna<RBillboardTextSceneNode>::lightcheck(L, 1);
			node = Luna<RSceneCamera>::lightcheck(L, 1);
			node = Luna<RLightSceneNode>::lightcheck(L, 1);
			node = Luna<RMeshSceneNode>::lightcheck(L, 1);
			node = Luna<RTerrainSceneNode>::lightcheck(L, 1);
			if (node != NULL)
			{
				self()->setParent(node->getNode());
				return 0;
			}
		}

		return 0;
		
		endifisvalid();
	}
	
	int RSceneNode::setID(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		self()->setID((int)getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}

	int RSceneNode::getID(lua_State* L)
	{
		ifistrue(isNodeSet);
			
		lua_pushnumber(L,self()->getID());
		return 1;
		
		endifisvalid();
	}

	int RSceneNode::setName(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		const char* newname = getmandatoryargument_constchar(L,1);

		irr::core::stringc mynewname;
		mynewname = newname;
		self()->RoketSetName(mynewname);
		return 0;
		
		endifisvalid();
	}

	int RSceneNode::getName(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		lua_pushstring(L,self()->RoketGetName().c_str());
		return 1;
		
		endifisvalid();
	}
	
	int RSceneNode::applyTexture(lua_State* L)
	{
		ifistrue(isNodeSet);
		
//		cout << "1" << endl;
		video::RTexture *tex = Luna<video::RTexture>::check(L, 1);
//		cout << "1a" << endl;
//		video::RTexture *tex = Luna<video::RTexture>::check(L, 1);
//		cout << "2" << endl;
		cNumber d = getmandatoryargument_cNumber(L,2);
//		VarCheck
//		ifdebug(lua_isnumber(L,3)==false,INVALID_ARGUMENT(TYPE_NUMBER,3),LVL_ERR,L);
//		cout << "3" << endl;
//		cNumber d = lua_tonumber(L,3);
//		cout << "4" << endl;
		self()->setMaterialTexture(d,tex->getTexture());
//		cout << "5" << endl;

		// Get Object
		//video::RTexture *tex = Luna<video::RTexture>::check(L, 1);
		//ifdebug(lua_isnumber(L,2)==false,INVALID_ARGUMENT(TYPE_NUMBER,2),LVL_ERR,L);
		return 0;
		
		endifisvalid();
	}
	
	int RSceneNode::applyFlag(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		const char* flagType = getmandatoryargument_constchar(L,1);
		bool flagSet = getmandatoryargument_bool(L,2);
		irr::video::E_MATERIAL_FLAG flagIrrType = irr::video::EMF_MATERIAL_FLAG_COUNT;
		
		if (strcmp(flagType,"WIREFRAME")==0)
			flagIrrType = irr::video::EMF_WIREFRAME;
		if (strcmp(flagType,"POINTCLOUD")==0)
			flagIrrType = irr::video::EMF_POINTCLOUD;
		if (strcmp(flagType,"GOURAUDSHADING")==0)
			flagIrrType = irr::video::EMF_GOURAUD_SHADING;
		if (strcmp(flagType,"ZBUFFER")==0)
			flagIrrType = irr::video::EMF_ZBUFFER;
		if (strcmp(flagType,"ZWRITEENABLE")==0)
			flagIrrType = irr::video::EMF_ZWRITE_ENABLE;
		if (strcmp(flagType,"BACKFACECULLING")==0)
			flagIrrType = irr::video::EMF_BACK_FACE_CULLING;
		if (strcmp(flagType,"FRONTFACECULLING")==0)
			flagIrrType = irr::video::EMF_FRONT_FACE_CULLING;
		if (strcmp(flagType,"BILINEARFILTER")==0)
			flagIrrType = irr::video::EMF_BILINEAR_FILTER;
		if (strcmp(flagType,"TRILINEARFILTER")==0)
			flagIrrType = irr::video::EMF_TRILINEAR_FILTER;
		if (strcmp(flagType,"ANISOTROPICFILTER")==0)
			flagIrrType = irr::video::EMF_ANISOTROPIC_FILTER;
		if (strcmp(flagType,"FOGENABLE")==0)
			flagIrrType = irr::video::EMF_FOG_ENABLE;
		if (strcmp(flagType,"NORMALIZENORMALS")==0)
			flagIrrType = irr::video::EMF_NORMALIZE_NORMALS;
		if (strcmp(flagType,"TEXTUREWRAP")==0)
			flagIrrType = irr::video::EMF_TEXTURE_WRAP;
		if (strcmp(flagType,"LIGHTING")==0)
			flagIrrType = irr::video::EMF_LIGHTING;

		if (flagIrrType == irr::video::EMF_MATERIAL_FLAG_COUNT)
		{
			debug("Invalid flag passed to applyFlag.",LVL_ERR,L);
			return 0;
		}
		
		self()->setMaterialFlag(flagIrrType, flagSet);
		return 0;
		
		endifisvalid();
	}

	int RSceneNode::applyMaterialType(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		const char* flagType = getmandatoryargument_constchar(L,1);
		irr::video::E_MATERIAL_TYPE flagIrrType = irr::video::EMT_FORCE_32BIT;

		if (strcmp(flagType,"SOLID")==0)
			flagIrrType = irr::video::EMT_SOLID;
		if (strcmp(flagType,"SOLID_2_LAYER")==0)
			flagIrrType = irr::video::EMT_SOLID_2_LAYER;
		if (strcmp(flagType,"LIGHTMAP")==0)
			flagIrrType = irr::video::EMT_LIGHTMAP;
		if (strcmp(flagType,"LIGHTMAP_ADD")==0)
			flagIrrType = irr::video::EMT_LIGHTMAP_ADD;
		if (strcmp(flagType,"LIGHTMAP_M2")==0)
			flagIrrType = irr::video::EMT_LIGHTMAP_M2;
		if (strcmp(flagType,"LIGHTMAP_M4")==0)
			flagIrrType = irr::video::EMT_LIGHTMAP_M4;
		if (strcmp(flagType,"LIGHTMAP_DYNAMIC")==0)
			flagIrrType = irr::video::EMT_LIGHTMAP_LIGHTING;
		if (strcmp(flagType,"LIGHTMAP_DYNAMIC_M2")==0)
			flagIrrType = irr::video::EMT_LIGHTMAP_LIGHTING_M2;
		if (strcmp(flagType,"LIGHTMAP_DYNAMIC_M4")==0)
			flagIrrType = irr::video::EMT_LIGHTMAP_LIGHTING_M4;
		if (strcmp(flagType,"DETAIL_MAP")==0)
			flagIrrType = irr::video::EMT_DETAIL_MAP;
		if (strcmp(flagType,"SPHERE_MAP")==0)
			flagIrrType = irr::video::EMT_SPHERE_MAP;
		if (strcmp(flagType,"REFLECTION_2_LAYER")==0)
			flagIrrType = irr::video::EMT_REFLECTION_2_LAYER;
		if (strcmp(flagType,"TRANSPARENT_ADD_COLOR")==0)
			flagIrrType = irr::video::EMT_TRANSPARENT_ADD_COLOR;
		if (strcmp(flagType,"TRANSPARENT_ALPHA_CHANNEL")==0)
			flagIrrType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
		if (strcmp(flagType,"TRANSPARENT_ALPHA_CHANNEL_REF")==0)
			flagIrrType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		if (strcmp(flagType,"TRANSPARENT_VERTEX_ALPHA")==0)
			flagIrrType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
		if (strcmp(flagType,"TRANSPARENT_REFLECTION_2_LAYER")==0)
			flagIrrType = irr::video::EMT_TRANSPARENT_REFLECTION_2_LAYER;
		if (strcmp(flagType,"NORMAL_MAP_SOLID")==0)
			flagIrrType = irr::video::EMT_NORMAL_MAP_SOLID;
		if (strcmp(flagType,"NORMAL_MAP_TRANSPARENT_ADD_COLOR")==0)
			flagIrrType = irr::video::EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR;
		if (strcmp(flagType,"NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA")==0)
			flagIrrType = irr::video::EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA;
		if (strcmp(flagType,"PARALLAX_MAP_SOLID")==0)
			flagIrrType = irr::video::EMT_PARALLAX_MAP_SOLID;
		if (strcmp(flagType,"PARALLAX_MAP_TRANSPARENT_ADD_COLOR")==0)
			flagIrrType = irr::video::EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR;
		if (strcmp(flagType,"PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA")==0)
			flagIrrType = irr::video::EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA;
		if (strcmp(flagType,"ONE_TEXTURE_BLEND")==0)
			flagIrrType = irr::video::EMT_ONETEXTURE_BLEND;

		if (flagIrrType == irr::video::EMT_FORCE_32BIT)
		{
			debug("Invalid flag passed to applyMaterialType.",LVL_ERR,L);
			return 0;
		}
		
		self()->setMaterialType(flagIrrType);
		return 0;
		
		endifisvalid();
	}
	
	int RSceneNode::setRotation(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		
		self()->setRotation(vector3d->getObject());

		// update physics if nessicary
		if (physType == E_TYPE_SPHERE || 
			physType == E_TYPE_CUBE ||
			physType == E_TYPE_MESH ||
			physType == E_TYPE_TERRAIN)
		{
			physBody->setRotation(vector3d->getObject());
			//debug("Use forces and torque to apply smooth physics.",LVL_WARN,L);
		}

		return 0;
		
		endifisvalid();
	}

	/* setParent
	int RSceneNode::setParent(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		lua_gettablevalue(L,2,0);
		ifdebug(luaL_testudata(L,-1,TYPE_RSCENENODE)==NULL,INVALID_ARGUMENT(TYPE_RSCENENODE,1),LVL_ERR,L);
		scene::RSceneNode *node = reinterpret_cast<scene::RSceneNode*>(lua_touserdata(L, -1));
		
		self()->setParent(node->getNode());
		return 0;
		
		endifisvalid();
		
		
		/*
		ifistrue(isNodeSet);
		
		lua_gettablevalue(L,2,0);
		bool testValid = false;
		if (luaL_testudata(L,-1,)!=NULL) testValid = true;
		if (luaL_testudata(L,-1,TYPE_RSCENECAMERA)!=NULL) testValid = true;
		if (luaL_testudata(L,-1,TYPE_RSCENEMESHNODE)!=NULL) testValid = true;
		if (luaL_testudata(L,-1,TYPE_RSCENETERRAINNODE)!=NULL) testValid = true;
		
		ifdebug(!testValid,INVALID_ARGUMENT(TYPE_RSCENENODE,1),LVL_ERR,L);
		if (testValid)
		{
			void* obj = lua_touserdata(L, -1);
			scene::RSceneNode * node = reinterpret_cast<scene::RSceneNode*>(obj);
		
			self()->setParent(node->getNode());
		}
		return 0;
		
		endifisvalid();
		
	}*/
	
	int RSceneNode::getPosition(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D* retvector = Luna < core::RVector3D >::createNew(L);
		retvector->setObject(self()->getPosition());
		
		return 1;
		
		endifisvalid();
	}

	int RSceneNode::getRotation(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D* retvector = Luna < core::RVector3D >::createNew(L);
		retvector->setObject(self()->getRotation());
		
		return 1;
		
		endifisvalid();
	}

	int RSceneNode::applyPhysicsSettings(lua_State* L)
	{
		ifistrue(isNodeSet);

		if (physType != E_TYPE_SPHERE && 
			physType != E_TYPE_CUBE &&
			physType != E_TYPE_MESH &&
			physType != E_TYPE_TERRAIN)
		{
			debug("This object cannot have physics applied to it.",LVL_WARN,L);
			return 0;
		}
		
		// developer can set the mass and density, etc.. with this function
		cNumber newmass = getmandatoryargument_cNumber(L,1);
		core::RVector3D* inertiar = Luna < core::RVector3D >::check(L,2);
		irr::core::vector3df newinertia = inertiar->getObject();
		physBody->setPhysicsSettings(newmass,newinertia);
		return 0;
		
		endifisvalid();
	}

	int RSceneNode::applyForce(lua_State* L)
	{
		ifistrue(isNodeSet);

		if (physType != E_TYPE_SPHERE && 
			physType != E_TYPE_CUBE &&
			physType != E_TYPE_MESH &&
			physType != E_TYPE_TERRAIN)
		{
			return 0;
		}

		// apply force
		core::RVector3D* f = Luna < core::RVector3D >::check(L,1);
		irr::core::vector3df irrf = f->getObject();
		physBody->applyForce(irrf);
		return 0;
		
		endifisvalid();
	}

	int RSceneNode::applyTorque(lua_State* L)
	{
		ifistrue(isNodeSet);

		if (physType != E_TYPE_SPHERE && 
			physType != E_TYPE_CUBE &&
			physType != E_TYPE_MESH &&
			physType != E_TYPE_TERRAIN)
		{
			return 0;
		}

		// apply torque
		core::RVector3D* f = Luna < core::RVector3D >::check(L,1);
		irr::core::vector3df irrf = f->getObject();
		physBody->applyTorque(irrf);
		return 0;
		
		endifisvalid();
	}

	int RSceneNode::destroy(lua_State* L)
	{
		ifistrue(isNodeSet);

		if (physType == E_TYPE_SPHERE ||
			physType == E_TYPE_CUBE ||
			physType == E_TYPE_MESH ||
			physType == E_TYPE_TERRAIN)
		{
			physBody->deletePhysics();
			delete physBody;
		}

#ifdef _IRR_MOD_PERPIXEL_BASIC
			if (!roketdevice->getStencilStatus())
				xeffect->removeShadowFromNode(self());
#else
			xeffect->removeShadowFromNode(self());
#endif
		self()->remove();
		self() = 0;
		isNodeSet = false;
		return 0;
		
		endifisvalid();
	}

	int RSceneNode::destroyPhysics(lua_State* L)
	{
		ifistrue(isNodeSet);

		if (physType == E_TYPE_SPHERE ||
			physType == E_TYPE_CUBE ||
			physType == E_TYPE_MESH ||
			physType == E_TYPE_TERRAIN)
		{
			physBody->deletePhysics();
			physType = E_TYPE_IGNORE;
			delete physBody;
		}
		return 0;
		
		endifisvalid();
	}

	void RSceneNode::setupPhysics(ePhysicsType type, NewtonWorld* physworld, irr::core::vector3df position, irr::scene::IMesh* mesh, irr::scene::ITerrainSceneNode* terrain)
	{
		physType = type;
//		tmpCount = 0;
		mb = NULL;
		mb_vertices_2t = NULL;
		mb_vertices_normal = NULL;
		mb_vertices_tangent = NULL;
		mb_indices = NULL;
		int a,b,x,z,scalex,scalez,width,height;
		char* attibutes;
		unsigned short* elevations;
		switch (physType)
		{
			case E_TYPE_SPHERE:
				// -- SPHERE --
				// Get an Irrlicht matrix
				posdesc.setTranslation(irr::core::vector3df(0,0,0)); //self()->getPosition());
				posdesc.setRotationDegrees(self()->getRotation());
				//posdesc.setScale(self()->getScale());
				// Setup Roket Physics Body for sphere
				physBody = new physics::Roket_PhysicsBody(
								NewtonCreateSphere(
									physworld,
									self()->getScale().X * NEWTON_SCALE,
									self()->getScale().Y * NEWTON_SCALE,
									self()->getScale().Z * NEWTON_SCALE,
									posdesc.pointer()
								),
								physworld,
								E_TYPE_SPHERE,
								0
								);
				physBody->attachNode(this);
				physBody->setPosition(position);
				self()->setPosition(position);
				break;
			case E_TYPE_CUBE:
				// -- CUBE --
				// Get an Irrlicht matrix
				posdesc.setTranslation(irr::core::vector3df(0,0,0));
				posdesc.setRotationDegrees(self()->getRotation());
				//posdesc.setScale(self()->getScale());
				// Setup Roket Physics Body for sphere
				physBody = new physics::Roket_PhysicsBody(
								NewtonCreateBox(
									physworld,
									self()->getScale().X * NEWTON_SCALE,
									self()->getScale().Y * NEWTON_SCALE,
									self()->getScale().Z * NEWTON_SCALE,
									posdesc.pointer()
								),
								physworld,
								E_TYPE_CUBE,
								0
								);
				physBody->attachNode(this);
				physBody->setPosition(position);
				self()->setPosition(position);
				break;
			case E_TYPE_MESH:
				total_indicies = 0;
				total_vertexes = 0;
				test_mbc = mesh->getMeshBufferCount();
				if (test_mbc <= 0)
				{
					nonlua_debug("Invalid terrain specified for physics object (no mesh buffers).",LVL_WARN);
					physType = E_TYPE_IGNORE;
					break;
				}
				has_indicies = false;
				for (int i = 0; i < mesh->getMeshBufferCount(); i++)
				{
					mb = mesh->getMeshBuffer(i);

					if (mb)
					{
						total_indicies += mb->getIndexCount();
						total_vertexes += mb->getVertexCount();
						if (mb->getIndexCount() > 0)
						{
							has_indicies = true;
							break;
						}
					}
				}
				if (!has_indicies)
				{
					nonlua_debug("Invalid terrain specified for physics object (no indicies, " << total_vertexes << " vertexes).",LVL_WARN);
					physType = E_TYPE_IGNORE;
					break;
				}

				// Setup Roket Physics Body for terrain
				posdesc.setTranslation(irr::core::vector3df(0,0,0));
				posdesc.setRotationDegrees(self()->getRotation());
				//posdesc.setScale(self()->getScale());
				// Setup Roket Physics Body for sphere
				collision = NewtonCreateTreeCollision(physworld);
				NewtonTreeCollisionBeginBuild(collision);

				// loop through, adding triangles.
				mesh->grab();
				for (int i = 0; i < mesh->getMeshBufferCount(); i++) 
				{       
					// Retrieve the meshbuffer. 
					mb = mesh->getMeshBuffer(i); 

					if (mb) 
					{ 
						if (mb->getVertexType() == irr::video::EVT_2TCOORDS)
						{
							// Retrieve vertices and indices.
							mb_vertices_2t = (irr::video::S3DVertex2TCoords*)mb->getVertices(); 
							mb_indices = mb->getIndices(); 
	
							// Add each triangle from the mesh to the tree. 
							for (int j = 0; j < mb->getIndexCount(); j += 3) 
							{ 
								int index_one = mb_indices[j]; 
								int index_two = mb_indices[j + 1]; 
								int index_three = mb_indices[j + 2]; 
	
								// First vertice. 
								vertarray[0] = mb_vertices_2t[index_one].Pos.X; 
								vertarray[1] = mb_vertices_2t[index_one].Pos.Y; 
								vertarray[2] = mb_vertices_2t[index_one].Pos.Z; 
	
								// Second vertice. 
								vertarray[3] = mb_vertices_2t[index_two].Pos.X; 
								vertarray[4] = mb_vertices_2t[index_two].Pos.Y; 
								vertarray[5] = mb_vertices_2t[index_two].Pos.Z; 
	
								// Third vertice. 
								vertarray[6] = mb_vertices_2t[index_three].Pos.X; 
								vertarray[7] = mb_vertices_2t[index_three].Pos.Y; 
								vertarray[8] = mb_vertices_2t[index_three].Pos.Z; 
	
								// Add the face to the collision tree. 
								NewtonTreeCollisionAddFace(collision, 3, vertarray, 12, 1); 
							}
						}
						else if (mb->getVertexType() == irr::video::EVT_STANDARD)
						{
							// Retrieve vertices and indices.
							mb_vertices_normal = (irr::video::S3DVertex*)mb->getVertices(); 
							mb_indices = mb->getIndices(); 
	
							// Add each triangle from the mesh to the tree. 
							for (int j = 0; j < mb->getIndexCount(); j += 3) 
							{ 
								int index_one = mb_indices[j]; 
								int index_two = mb_indices[j + 1]; 
								int index_three = mb_indices[j + 2]; 
	
								// First vertice. 
								vertarray[0] = mb_vertices_normal[index_one].Pos.X; 
								vertarray[1] = mb_vertices_normal[index_one].Pos.Y; 
								vertarray[2] = mb_vertices_normal[index_one].Pos.Z; 
	
								// Second vertice. 
								vertarray[3] = mb_vertices_normal[index_two].Pos.X; 
								vertarray[4] = mb_vertices_normal[index_two].Pos.Y; 
								vertarray[5] = mb_vertices_normal[index_two].Pos.Z; 
	
								// Third vertice. 
								vertarray[6] = mb_vertices_normal[index_three].Pos.X; 
								vertarray[7] = mb_vertices_normal[index_three].Pos.Y; 
								vertarray[8] = mb_vertices_normal[index_three].Pos.Z; 
	
								// Add the face to the collision tree. 
								NewtonTreeCollisionAddFace(collision, 3, vertarray, 12, 1); 
							}
						}
						else if (mb->getVertexType() == irr::video::EVT_TANGENTS)
						{
							// Retrieve vertices and indices.
							mb_vertices_tangent = (irr::video::S3DVertexTangents*)mb->getVertices(); 
							mb_indices = mb->getIndices(); 
	
							// Add each triangle from the mesh to the tree. 
							for (int j = 0; j < mb->getIndexCount(); j += 3) 
							{ 
								int index_one = mb_indices[j]; 
								int index_two = mb_indices[j + 1]; 
								int index_three = mb_indices[j + 2]; 
	
								// First vertice. 
								vertarray[0] = mb_vertices_tangent[index_one].Pos.X; 
								vertarray[1] = mb_vertices_tangent[index_one].Pos.Y; 
								vertarray[2] = mb_vertices_tangent[index_one].Pos.Z; 
	
								// Second vertice. 
								vertarray[3] = mb_vertices_tangent[index_two].Pos.X; 
								vertarray[4] = mb_vertices_tangent[index_two].Pos.Y; 
								vertarray[5] = mb_vertices_tangent[index_two].Pos.Z; 
	
								// Third vertice. 
								vertarray[6] = mb_vertices_tangent[index_three].Pos.X; 
								vertarray[7] = mb_vertices_tangent[index_three].Pos.Y; 
								vertarray[8] = mb_vertices_tangent[index_three].Pos.Z; 
	
								// Add the face to the collision tree. 
								NewtonTreeCollisionAddFace(collision, 3, vertarray, 12, 1); 
							}
						}
					}
				}
				mesh->drop();
				mesh = NULL;
				NewtonTreeCollisionEndBuild(collision, 0);

				physBody = new physics::Roket_PhysicsBody(
								collision,
								physworld,
								E_TYPE_MESH,
								0
								);
				physBody->attachNode(this);
				physBody->setPosition(position);
				self()->setPosition(position);

				break;
			case E_TYPE_TERRAIN:
				// Setup Roket Physics Body for terrain
				posdesc.setTranslation(irr::core::vector3df(0,0,0));
				posdesc.setRotationDegrees(self()->getRotation());

				width = irr::core::abs_(terrain->getBoundingBox().MinEdge.X - terrain->getBoundingBox().MaxEdge.X);
				height = irr::core::abs_(terrain->getBoundingBox().MinEdge.Z - terrain->getBoundingBox().MaxEdge.Z);

				scalex = terrain->getScale().X;
				scalez = terrain->getScale().Z;

				elevations = (unsigned short*) malloc (width * height * sizeof (unsigned short));
				attibutes = (char*) malloc (width * width * sizeof (char));
				memset (attibutes, 0, width * height * sizeof (char));

				cout << "Heightmap information: " << endl
					<< "  Width: " << width << endl
					<< "  Height: " << height << endl
					<< "  Cellsize X: " << scalex << endl
					<< "  Cellsize Z: " << scalez << endl
					<< "  Cells X: " << width / scalex << endl
					<< "  Cells Z: " << height / scalez << endl
					<< "  Center X: " << terrain->getTerrainCenter().X << endl
					<< "  Center Z: " << terrain->getTerrainCenter().Z << endl;

				a = 0;
				b = 0;
				/*
				for (int z = 0; z < height / scalez; z++ )
				{
					for (int x = 0; x < (width / scalex) - (scalex * 16); x++ )
					{
						//elevations[a] = terrain->getHeight(x * scalex,z * scalez) / terrain->getScale().Y;
						//elevations[a] = 0;
						if (terrain->getHeight((x * scalex) + (scalex * 16),z * scalez) == -999999.9f)
						{
							cout << "Invalid terrain height at " << ((x * scalex) + (scalex * 16)) << "," << z * scalez << endl;
                            elevations[a] = 0;
						}
						else if (terrain->getHeight((x * scalex) + (scalex * 16),z * scalez) < 0)
							elevations[a] = 0;
						else
							elevations[a] = terrain->getHeight((x * scalex) + (scalex * 16),z * scalez) * (65536.0f / 256.0f);
						//cout << "Elevations is: " << elevations[a] << " at " << (z * scalez) << ", " << (x * scalex) << endl;
						//if (b < terrain->getHeight(x * scalex,z * scalez) / terrain->getScale().Y)
						//	b = terrain->getHeight(x * scalex,z * scalez) / terrain->getScale().Y;
						a++;
					}
				}
				*/
				for (int z = 0; z < height / scalez; z++ )
				{
					for (int x = 0; x < (width / scalex); x++ )
					{
						//elevations[a] = terrain->getHeight(x * scalex,z * scalez) / terrain->getScale().Y;
						//elevations[a] = 0;
						if (terrain->getHeight((x * scalex),z * scalez) + 10 == -999999.9f)
						{
							cout << "Invalid terrain height at " << ((x * scalex)) << "," << z * scalez << endl;
                            elevations[a] = 0;
						}
						else if (terrain->getHeight((x * scalex),z * scalez) + 10 < 0)
						{
							cout << "Invalid terrain height at " << ((x * scalex)) << "," << z * scalez << endl;
							elevations[a] = 0;
						}
						else
							elevations[a] = (terrain->getHeight((x * scalex),z * scalez) + 10) * (65536.0f / (256.0f * terrain->getScale().Y));
						//cout << " " << (terrain->getHeight((x * scalex),z * scalez) + 10) * (65536.0f / 256.0f) << " VS " << terrain->getHeight((x * scalex),z * scalez) + 10 << endl;
						//cout << "Elevations is: " << elevations[a] << " at " << (z * scalez) << ", " << (x * scalex) << endl;
						//if (b < terrain->getHeight(x * scalex,z * scalez) / terrain->getScale().Y)
						//	b = terrain->getHeight(x * scalex,z * scalez) / terrain->getScale().Y;
						a++;
					}
				}

				cout << "  Maximum height: " << b << endl;
				
				/*cout << "Heightmap is: " << endl;
				for (int b = 0; b < a; b++)
				{
					cout << elevations[b] << ",";
				}
				cout << endl;*/

				collision = NewtonCreateHeightFieldCollision (physworld, (width / scalex)/* - (scalex * 16)*/, height / scalez, 0, elevations, attibutes, scalex, (1.0f / 256.0f) * terrain->getScale().Y);

				physBody = new physics::Roket_PhysicsBody(
								collision,
								physworld,
								E_TYPE_TERRAIN,
								0
								);
				physBody->attachNode(this);
				//position.Y -= 1;
				physBody->setPosition(position);
				//position.Y += 1;
				self()->setPosition(position);

				break;
			case E_TYPE_IGNORE:
			case E_TYPE_NONE:
			case E_TYPE_UNKNOWN:
				// Don't setup a Roket Physics Body
				break;
		}
	}
	
}
