#include "roket3d.h"
#include "E.Unmanaged/Scene/rscenespecialeffects.h"

namespace scene
{
	const char *RSceneSpecialEffects::className = TYPE_RSCENESPECIALEFFECTS;
	const Luna < RSceneSpecialEffects >::FunctionType RSceneSpecialEffects::Functions[] = {
		{"addPostProcessor", &RSceneSpecialEffects::addPostProcessor},
  {"removePostProcessor", &RSceneSpecialEffects::removePostProcessor},
  {"setShaderPath", &RSceneSpecialEffects::setShaderPath},
  {"getShaderPath", &RSceneSpecialEffects::getShaderPath},
  {0}
	};
	const Luna < RSceneSpecialEffects >::PropertyType RSceneSpecialEffects::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RSceneSpecialEffects::RSceneSpecialEffects()
	{
		objectExists=false;
	}

	RSceneSpecialEffects::~RSceneSpecialEffects()
	{
		objectExists=false;
	}
	
	RSceneSpecialEffects::RSceneSpecialEffects(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RSceneSpecialEffects.  You must collect one from an RSceneManager using the appropriate functions.",LVL_ERR,L);
		objectExists=false;
	}
	
	void RSceneSpecialEffects::setObject(effectHandler* xe, core::RDevice* dev)
	{
		xeffect = xe;
		device = dev;
		objectExists=true;
	}
	
	effectHandler* RSceneSpecialEffects::getObject()
	{
		if (objectExists)
			return xeffect;
		else
			return NULL;
	}

	int RSceneSpecialEffects::addPostProcessor(lua_State* L)
	{
		ifistrue(objectExists);
		
		const char* shaderName = getmandatoryargument_constchar(L,1);
		irr::core::stringc shaderExt = (device->getIrrlichtDevice()->getVideoDriver()->getDriverType()
			== irr::video::EDT_DIRECT3D9) ? ".hlsl" : ".glsl";
		irr::core::stringc shaderFullPath = device->getShaderLocation() + shaderName + shaderExt;
		if (fileExists(shaderFullPath))
		{
			debug("Loading post-processing effect from " << shaderFullPath.c_str() << "...",LVL_INFO,L);
			lua_pushnumber(L,xeffect->addPostProcessingEffectFromFile(shaderFullPath));
		}
		else
		{
			debug("Unable to load post-processing effect from " << shaderFullPath.c_str() << "!",LVL_WARN,L);
			lua_pushnil(L);
		}

		return 1;
		
		endifisvalid();
	}
	
	int RSceneSpecialEffects::removePostProcessor(lua_State* L)
	{
		ifistrue(objectExists);

		int removeId = getmandatoryargument_cNumber(L,1);
		xeffect->removePostProcessingEffect(removeId);
		return 0;
		
		endifisvalid();
	}

	int RSceneSpecialEffects::setShaderPath(lua_State* L)
	{
		ifistrue(objectExists);
		
		const char* shaderName = getmandatoryargument_constchar(L,1);
		debug("Shader path set to " << shaderName << ".",LVL_INFO,L);
		device->setShaderLocation(shaderName);
		return 0;
		
		endifisvalid();
	}
	
	int RSceneSpecialEffects::getShaderPath(lua_State* L)
	{
		ifistrue(objectExists);

		lua_pushstring(L,device->getShaderLocation().c_str());
		return 1;
		
		endifisvalid();
	}

}
