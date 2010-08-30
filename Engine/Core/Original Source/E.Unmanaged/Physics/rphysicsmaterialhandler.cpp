#include "roket3d.h"
#include "E.Unmanaged/Physics/rphysicsmaterialhandler.h"

namespace physics
{
	const char *RPhysicsMaterialHandler::className = "RPhysicsMaterialHandler";
	const Luna < RPhysicsMaterialHandler >::FunctionType RPhysicsMaterialHandler::Functions[] = {
		{"newMaterial", &RPhysicsMaterialHandler::newMaterial},
  {"getMaterialPropertyResistance", &RPhysicsMaterialHandler::getMaterialPropertyResistance},
  {"getMaterialPropertyStrength", &RPhysicsMaterialHandler::getMaterialPropertyStrength},
  {"getMaterialID", &RPhysicsMaterialHandler::getMaterialID},
  //{"removeMaterial", &RPhysicsMaterialHandler::removeMaterial},
		{0}
	};
	const Luna < RPhysicsMaterialHandler >::PropertyType RPhysicsMaterialHandler::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RPhysicsMaterialHandler::RPhysicsMaterialHandler(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RPhysicsMaterialHandlers.  You must collect one from an RPhysicsManager.",LVL_ERR,L);
	}
	
	int RPhysicsMaterialHandler::newMaterial(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber num = obj->newMaterial(ConvertToWideChar(getmandatoryargument_constchar(L,1)));
		if (num != -1)
			lua_pushnumber(L,num);
		else
			lua_pushnil(L);
		return 1;
		
		endifisvalid();
	}
	
	int RPhysicsMaterialHandler::getMaterialPropertyResistance(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber num = obj->getMaterialPropertyResistance(getmandatoryargument_cNumber(L,1));
		if (num != -1)
			lua_pushnumber(L,num);
		else
			lua_pushnil(L);
		return 1;
		
		endifisvalid();
	}
	
	int RPhysicsMaterialHandler::getMaterialPropertyStrength(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber num = obj->getMaterialPropertyStrength(getmandatoryargument_cNumber(L,1));
		if (num != -1)
			lua_pushnumber(L,num);
		else
			lua_pushnil(L);
		return 1;
		
		endifisvalid();
	}
	
	int RPhysicsMaterialHandler::getMaterialID(lua_State* L)
	{
		ifistrue(objectExists);
		
		cNumber num = obj->getMaterialID(ConvertToWideChar(getmandatoryargument_constchar(L,1)));
		if (num != -1)
			lua_pushnumber(L,num);
		else
			lua_pushnil(L);
		return 1;
		
		endifisvalid();
	}
	
	/*
	int RPhysicsMaterialHandler::removeMaterial(lua_State* L)
	{
		ifistrue(objectExists);
		
		obj->removeMaterial(getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	*/
	
	int RPhysicsMaterialHandler::setObject(Roket_PhysicsMaterialHandler* object)
	{
		obj=object;
		objectExists=true;
		return 0;
	}
	
	Roket_PhysicsMaterialHandler* RPhysicsMaterialHandler::getObject()
	{
		if (objectExists)
			return obj;
		else
			return NULL;
	}
}
