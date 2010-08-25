#include "roket3d.h"
#include "E.Unmanaged/Scene/rscenemeshnode.h"

namespace scene
{
	const char *RMeshSceneNode::className = TYPE_RSCENEMESHNODE;
	const Luna < RMeshSceneNode >::FunctionType RMeshSceneNode::Functions[] = {
		{"setPosition", &RMeshSceneNode::setPosition},
  {"getPosition", &RMeshSceneNode::getPosition},
  {"setRotation", &RMeshSceneNode::setRotation},
  {"getRotation", &RMeshSceneNode::getRotation},
  {"applyTexture", &RMeshSceneNode::applyTexture},
  {"applyFlag", &RMeshSceneNode::applyFlag},
  {"applyMaterialType", &RMeshSceneNode::applyMaterialType},
  {"applyPhysicsSettings", &RMeshSceneNode::applyPhysicsSettings},
  {"applyForce", &RMeshSceneNode::applyForce},
  {"applyTorque", &RMeshSceneNode::applyTorque},
  {"setParent", &RMeshSceneNode::setParent},
  {"destroy", &RMeshSceneNode::destroy},
  {"destroyPhysics", &RMeshSceneNode::destroyPhysics},
  {"setName", &RMeshSceneNode::setName},
  {"getName", &RMeshSceneNode::getName},
  {"setID", &RMeshSceneNode::setID},
  {"getID", &RMeshSceneNode::getID},
		{0}
	};
	const Luna < RMeshSceneNode >::PropertyType RMeshSceneNode::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RMeshSceneNode::RMeshSceneNode(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RMeshSceneNodes.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		isPrecious = true; // Lua should not destroy these.
	}
	
	void RMeshSceneNode::setNode(irr::scene::IAnimatedMeshSceneNode* meshnode, effectHandler* xe)
	{
		extended_self = meshnode;
		ident.type("RMeshSceneNode");
		ident.voidPointer(this);
		xeffect = xe;
		extended_self->RoketSetUserData(&ident);
	}
	
	irr::scene::IAnimatedMeshSceneNode* RMeshSceneNode::getNode()
	{
		return extended_self;
	}
}
