#include "roket3d.h"
#include "E.Unmanaged/Scene/rscenecamera.h"

namespace scene
{
	const char *RSceneCamera::className = TYPE_RSCENECAMERA;
	const Luna < RSceneCamera >::FunctionType RSceneCamera::Functions[] = {
		{"setSourcePosition", &RSceneCamera::setSourcePosition},
  {"getSourcePosition", &RSceneCamera::getSourcePosition},
  {"setRotation", &RSceneCamera::setRotation},
  {"getRotation", &RSceneCamera::getRotation},
  {"setDestinationPosition", &RSceneCamera::setDestinationPosition},
  {"getDestinationPosition", &RSceneCamera::getDestinationPosition},
  {"setParent", &RSceneCamera::setParent},
  {"setUpVectorPosition", &RSceneCamera::setUpVectorPosition},
  {"setName", &RSceneCamera::setName},
  {"getName", &RSceneCamera::getName},
  {"setID", &RSceneCamera::setID},
  {"getID", &RSceneCamera::getID},
  {0}
	};
	const Luna < RSceneCamera >::PropertyType RSceneCamera::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
	  { 0 }
	};
	
	RSceneCamera::RSceneCamera(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RSceneCameras.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		isPrecious = true; // Lua should not destroy these.
	}
	
	int RSceneCamera::setDestinationPosition(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D *vector = Luna<core::RVector3D>::check(L, 1);
		
		extended_self->setTarget(vector->getObject());
		return 0;
		
		endifisvalid();
	}
	
	int RSceneCamera::setUpVectorPosition(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D *vector = Luna<core::RVector3D>::check(L, 1);
		
		extended_self->setUpVector(vector->getObject());
		return 0;
		
		endifisvalid();
	}
	
	int RSceneCamera::getDestinationPosition(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D* vector = Luna < core::RVector3D >::createNew(L);
		cIrrVector3D pos = extended_self->getTarget();
		vector->setObject(pos);
		return 1;
		
		endifisvalid();
	}
	
	int RSceneCamera::setSourcePosition(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D *vector = Luna<core::RVector3D>::check(L, 1);
		
		extended_self->setPosition(vector->getObject());
		return 0;
		
		endifisvalid();
	}
	
	int RSceneCamera::getSourcePosition(lua_State* L)
	{
		ifistrue(isNodeSet);
		
		core::RVector3D* vector = Luna < core::RVector3D >::createNew(L);
		cIrrVector3D pos = extended_self->getPosition();
		vector->setObject(pos);
		return 1;
		
		endifisvalid();
	}
	
	void RSceneCamera::setNode(irr::scene::ICameraSceneNode* meshnode, effectHandler* xe)
	{
		extended_self = meshnode;
		xeffect = xe;
		isNodeSet = true;
	}
	
	irr::scene::ICameraSceneNode* RSceneCamera::getNode()
	{
		return extended_self;
	}
}
