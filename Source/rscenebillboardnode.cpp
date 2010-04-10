#include "roket3d.h"
#include "rscenebillboardnode.h"

namespace scene
{
	const char *RBillboardSceneNode::className = TYPE_RSCENEBILLBOARDNODE;
	const Luna < RBillboardSceneNode >::FunctionType RBillboardSceneNode::Functions[] = {
		{"setPosition", &RBillboardSceneNode::setPosition},
  {"getPosition", &RBillboardSceneNode::getPosition},
  {"setRotation", &RBillboardSceneNode::setRotation},
  {"getRotation", &RBillboardSceneNode::getRotation},
  {"applyTexture", &RBillboardSceneNode::applyTexture},
  {"applyFlag", &RBillboardSceneNode::applyFlag},
  {"applyMaterialType", &RBillboardSceneNode::applyMaterialType},
  {"applyPhysicsSettings", &RBillboardSceneNode::applyPhysicsSettings},
  {"applyForce", &RBillboardSceneNode::applyForce},
  {"applyTorque", &RBillboardSceneNode::applyTorque},
  {"setParent", &RBillboardSceneNode::setParent},
  {"destroy", &RBillboardSceneNode::destroy},
  {"destroyPhysics", &RBillboardSceneNode::destroyPhysics},
  {"setName", &RBillboardSceneNode::setName},
  {"getName", &RBillboardSceneNode::getName},
  {"setID", &RBillboardSceneNode::setID},
  {"getID", &RBillboardSceneNode::getID},
  // object functions
  {"setSize", &RBillboardSceneNode::setSize},
  {"getSize", &RBillboardSceneNode::getSize},
		{0}
	};
	const Luna < RBillboardSceneNode >::PropertyType RBillboardSceneNode::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RBillboardSceneNode::RBillboardSceneNode(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RBillboardSceneNode.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		isPrecious = true; // Lua should not destroy these.
	}
	
	void RBillboardSceneNode::setNode(irr::scene::IBillboardSceneNode* node, effectHandler* xe)
	{
		extended_self = node;
		ident.type("RBillboardSceneNode");
		ident.voidPointer(this);
		xeffect = xe;
		extended_self->RoketSetUserData(&ident);
	}
	
	irr::scene::IBillboardSceneNode* RBillboardSceneNode::getNode()
	{
		return extended_self;
	}

	int RBillboardSceneNode::setSize(lua_State* L)
	{
		ifisvalid(self());
		
		// TODO: Check argument check is correct.
		core::RVector2D* size = Luna<core::RVector2D>::check(L,1);

		extended_self->setSize(size->getObjectAsDimension());
		return 0;
		
		endifisvalid();
	}

	int RBillboardSceneNode::getSize(lua_State* L)
	{
		ifisvalid(self());
		
		// TODO: Check argument check is correct.
		core::RVector2D* size = Luna<core::RVector2D>::createNew(L);
		irr::core::vector2df vec;
		irr::core::dimension2df dim = extended_self->getSize();
		vec.X = dim.Width;
		vec.Y = dim.Height;
		size->setObject(vec);
		return 1;
		
		endifisvalid();
	}
}
