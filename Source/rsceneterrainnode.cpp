#include "roket3d.h"
#include "rsceneterrainnode.h"

namespace scene
{
	const char *RTerrainSceneNode::className = TYPE_RSCENETERRAINNODE;
	const Luna < RTerrainSceneNode >::FunctionType RTerrainSceneNode::Functions[] = {
		{"setPosition", &RTerrainSceneNode::setPosition},
  {"getPosition", &RTerrainSceneNode::getPosition},
  {"setRotation", &RTerrainSceneNode::setRotation},
  {"getRotation", &RTerrainSceneNode::getRotation},
  {"applyTexture", &RTerrainSceneNode::applyTexture},
  {"applyFlag", &RTerrainSceneNode::applyFlag},
  {"applyMaterialType", &RTerrainSceneNode::applyMaterialType},
  {"applyPhysicsSettings", &RTerrainSceneNode::applyPhysicsSettings},
  {"applyForce", &RTerrainSceneNode::applyForce},
  {"applyTorque", &RTerrainSceneNode::applyTorque},
  {"setParent", &RTerrainSceneNode::setParent},
  {"destroy", &RTerrainSceneNode::destroy},
  {"destroyPhysics", &RTerrainSceneNode::destroyPhysics},
  {"setName", &RTerrainSceneNode::setName},
  {"getName", &RTerrainSceneNode::getName},
  {"setID", &RTerrainSceneNode::setID},
  {"getID", &RTerrainSceneNode::getID},
  // begin custom functions
  {"getHeight", &RTerrainSceneNode::getHeight},
  {0}
	};
	const Luna < RTerrainSceneNode >::PropertyType RTerrainSceneNode::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RTerrainSceneNode::RTerrainSceneNode(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RTerrainSceneNodes.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		isPrecious = true; // Lua should not destroy these.
//		shadow_volume = NULL;
	}
	
	void RTerrainSceneNode::setNode(irr::scene::ITerrainSceneNode* meshnode, irr::scene::ISceneManager* mngr, effectHandler* xe)
	{
/*		if (shadow_volume)
		{
			// free the shadow volume
			delete shadow_volume;
		}
*/
		extended_self = meshnode;
		ident.type("RTerrainSceneNode");
		ident.voidPointer(this);
		xeffect = xe;
		extended_self->RoketSetUserData(&ident);
		isNodeSet = true;
/*
		// create the new shadow volume
		shadow_volume = new irr::scene::IShadowVolumeSceneNode(meshnode, mngr, -1);
		shadow_volume->setShadowMesh(meshnode->getMesh());
*/
	}
	
	irr::scene::ITerrainSceneNode* RTerrainSceneNode::getNode()
	{
		return extended_self;
	}
	
	int RTerrainSceneNode::getHeight(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector2D* checkvector = Luna < core::RVector2D >::check(L,1);

		lua_pushnumber(L,extended_self->getHeight(checkvector->getObject().X,checkvector->getObject().Y));
		return 1;
		
		endifisvalid();
	}
}
