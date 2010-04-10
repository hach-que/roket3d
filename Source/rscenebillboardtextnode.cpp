#include "roket3d.h"
#include "rscenebillboardtextnode.h"

namespace scene
{
	const char *RBillboardTextSceneNode::className = TYPE_RSCENEBILLBOARDTEXTNODE;
	const Luna < RBillboardTextSceneNode >::FunctionType RBillboardTextSceneNode::Functions[] = {
		{"setPosition", &RBillboardTextSceneNode::setPosition},
  {"getPosition", &RBillboardTextSceneNode::getPosition},
  {"setRotation", &RBillboardTextSceneNode::setRotation},
  {"getRotation", &RBillboardTextSceneNode::getRotation},
  {"applyTexture", &RBillboardTextSceneNode::applyTexture},
  {"applyFlag", &RBillboardTextSceneNode::applyFlag},
  {"applyMaterialType", &RBillboardTextSceneNode::applyMaterialType},
  {"applyPhysicsSettings", &RBillboardTextSceneNode::applyPhysicsSettings},
  {"applyForce", &RBillboardTextSceneNode::applyForce},
  {"applyTorque", &RBillboardTextSceneNode::applyTorque},
  {"setParent", &RBillboardTextSceneNode::setParent},
  {"destroy", &RBillboardTextSceneNode::destroy},
  {"destroyPhysics", &RBillboardTextSceneNode::destroyPhysics},
  {"setName", &RBillboardTextSceneNode::setName},
  {"getName", &RBillboardTextSceneNode::getName},
  {"setID", &RBillboardTextSceneNode::setID},
  {"getID", &RBillboardTextSceneNode::getID},
  // object functions
  {"setSize", &RBillboardTextSceneNode::setSize},
  {"getSize", &RBillboardTextSceneNode::getSize},
  {"setText", &RBillboardTextSceneNode::setText},
  {"setTextColor", &RBillboardTextSceneNode::setTextColor},
		{0}
	};
	const Luna < RBillboardTextSceneNode >::PropertyType RBillboardTextSceneNode::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RBillboardTextSceneNode::RBillboardTextSceneNode(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RBillboardTextSceneNodes.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		isPrecious = true; // Lua should not destroy these.
	}
	
	void RBillboardTextSceneNode::setNode(irr::scene::IBillboardTextSceneNode* node, effectHandler* xe)
	{
		extended_self = node;
		ident.type("RBillboardTextSceneNode");
		ident.voidPointer(this);
		xeffect = xe;
		extended_self->RoketSetUserData(&ident);
	}
	
	irr::scene::IBillboardTextSceneNode* RBillboardTextSceneNode::getNode()
	{
		return extended_self;
	}

	int RBillboardTextSceneNode::setSize(lua_State* L)
	{
		ifisvalid(self());
		
		// TODO: Check argument check is correct.
		core::RVector2D* size = Luna<core::RVector2D>::check(L,1);

		extended_self->setSize(size->getObjectAsDimension());
		return 0;
		
		endifisvalid();
	}

	int RBillboardTextSceneNode::getSize(lua_State* L)
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

	int RBillboardTextSceneNode::setText(lua_State* L)
	{
		ifisvalid(self());
		
		const char* text = getmandatoryargument_constchar(L,1);

		extended_self->setText(ConvertToWideChar(text));

		return 0;
		
		endifisvalid();
	}

	int RBillboardTextSceneNode::setTextColor(lua_State* L)
	{
		ifisvalid(self());
		
		core::RColor* col = Luna<core::RColor>::check(L,1);

		extended_self->setTextColor(col->getObject());
		return 0;
		
		endifisvalid();
	}
}
