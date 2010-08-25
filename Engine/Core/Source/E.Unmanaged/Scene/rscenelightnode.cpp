#include "roket3d.h"
#include "E.Unmanaged/Scene/rscenelightnode.h"

#include "effectWrapper.h"
#include "effectWrapperCB.h"

namespace scene
{
	const char *RLightSceneNode::className = TYPE_RSCENELIGHTNODE;
	const Luna < RLightSceneNode >::FunctionType RLightSceneNode::Functions[] = {
		{"setPosition", &RLightSceneNode::setPosition},
  {"getPosition", &RLightSceneNode::getPosition},
  {"setRotation", &RLightSceneNode::setRotation},
  {"getRotation", &RLightSceneNode::getRotation},
  {"applyTexture", &RLightSceneNode::applyTexture},
  {"applyFlag", &RLightSceneNode::applyFlag},
  {"applyMaterialType", &RLightSceneNode::applyMaterialType},
  {"applyPhysicsSettings", &RLightSceneNode::applyPhysicsSettings},
  {"applyForce", &RLightSceneNode::applyForce},
  {"applyTorque", &RLightSceneNode::applyTorque},
  {"setParent", &RLightSceneNode::setParent},
  {"destroy", &RLightSceneNode::destroy},
  {"destroyPhysics", &RLightSceneNode::destroyPhysics},
  {"setName", &RLightSceneNode::setName},
  {"getName", &RLightSceneNode::getName},
  {"setID", &RLightSceneNode::setID},
  {"getID", &RLightSceneNode::getID},
  // object specific functions
  {"setTargetPosition", &RLightSceneNode::setTargetPosition},
  {"getTargetPosition", &RLightSceneNode::getTargetPosition},
  {"isRelativeTarget", &RLightSceneNode::isRelativeTarget},
  {"setRelativeTarget", &RLightSceneNode::setRelativeTarget},
		{0}
	};
	const Luna < RLightSceneNode >::PropertyType RLightSceneNode::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RLightSceneNode::RLightSceneNode(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RLightSceneNode.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		isPrecious = true; // Lua should not destroy these.
		// default settings
		isRelativeTarget_setting = true;
		targetPosition = irr::core::vector3df(0,0,0);
	}
	
	void RLightSceneNode::setNode(irr::scene::ILightSceneNode* lightnode, effectHandler* xe)
	{
		extended_self = lightnode;
		ident.type("RLightSceneNode");
		ident.voidPointer(this);
		extended_self->RoketSetUserData(&ident);
	}
	
	irr::scene::ILightSceneNode* RLightSceneNode::getNode()
	{
		return extended_self;
	}

	void RLightSceneNode::setRoketDevice(core::RDevice* rd)
	{
		roketdevice = rd;
	}

	void RLightSceneNode::setupShadows(irr::core::vector3df start, core::RDevice* rd, effectHandler* xe, irr::video::SColor lc, float nearValue, float farValue)
	{
		xeffect = xe;
		roketdevice = rd;
		lightColor = lc;
#ifdef _IRR_MOD_PERPIXEL_BASIC
		if (!roketdevice->getStencilStatus())
#endif
			xeffect->addShadowLight( SShadowLight(start, targetPosition, 
				lightColor , nearValue, farValue, 90.0f * irr::core::DEGTORAD64));
		shadowLightID = xeffect->getShadowLightCount()-1;
	}

	// override set position
	int RLightSceneNode::setPosition(lua_State* L)
	{
		ifisvalid(self());

		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		
		self()->setPosition(vector3d->getObject());
#ifdef _IRR_MOD_PERPIXEL_BASIC
		if (!roketdevice->getStencilStatus())
		{
#endif
		irr::core::vector3df mypos = extended_self->getPosition();
		xeffect->getShadowLight(shadowLightID).setPosition(mypos);
		if (isRelativeTarget_setting)
		{
			mypos += targetPosition;
			xeffect->getShadowLight(shadowLightID).setTarget(mypos);
		}
#ifdef _IRR_MOD_PERPIXEL_BASIC
		}
#endif
		return 0;
		
		endifisvalid();
	}

	// target positions
	int RLightSceneNode::setTargetPosition(lua_State* L)
	{
		ifisvalid(self());
		
#ifdef _IRR_MOD_PERPIXEL_BASIC
		if (!roketdevice->getStencilStatus())
		{
#endif
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		
		targetPosition = vector3d->getObject();
		if (isRelativeTarget_setting)
		{
			irr::core::vector3df mypos = extended_self->getPosition();
			mypos += targetPosition;
			xeffect->getShadowLight(shadowLightID).setTarget(mypos);
		}
		else
		{
			xeffect->getShadowLight(shadowLightID).setTarget(targetPosition);
		}

#ifdef _IRR_MOD_PERPIXEL_BASIC
		}
#endif

		return 0;
		
		endifisvalid();
	}

	int RLightSceneNode::getTargetPosition(lua_State* L)
	{
		ifisvalid(self());

#ifdef _IRR_MOD_PERPIXEL_BASIC
		if (!roketdevice->getStencilStatus())
		{
#endif
		core::RVector3D *vector3d = Luna<core::RVector3D>::createNew(L);
		vector3d->setObject(targetPosition);
		return 1;
#ifdef _IRR_MOD_PERPIXEL_BASIC
		}
		else
		{
			return 0;
		}
#endif
		
		endifisvalid();
	}

	int RLightSceneNode::setRelativeTarget(lua_State* L)
	{
		ifisvalid(self());
		
		isRelativeTarget_setting = getmandatoryargument_bool(L,1);
		return 0;
		
		endifisvalid();
	}

	int RLightSceneNode::isRelativeTarget(lua_State* L)
	{
		ifisvalid(self());
		
		lua_pushboolean(L,isRelativeTarget_setting);
		return 1;
		
		endifisvalid();
	}
}
