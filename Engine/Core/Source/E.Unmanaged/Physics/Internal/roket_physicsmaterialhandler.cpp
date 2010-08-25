#include "roket3d.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsmaterialhandler.h"

namespace physics
{
	struct RMaterialStruct
	{
		wchar_t* name;
		bool deleted;
		// we obviously need to know what else
		// to put in here
		bool static_mat;
		cNumber resistance;
		cNumber strength;
		// an array of nodes that are currently using the material
		irr::core::array<scene::RSceneNode*> usagenodes;
	};

	Roket_PhysicsMaterialHandler::Roket_PhysicsMaterialHandler()
	{
		this->newMaterial(L"default");
	}
	
	int Roket_PhysicsMaterialHandler::newMaterial(wchar_t* name)
	{
		RMaterialStruct new_RMaterialStruct;
		//new_RMaterialStruct = (RMaterialStruct*)calloc(1, sizeof(RMaterialStruct));
		new_RMaterialStruct.name = name;
		new_RMaterialStruct.resistance = MATERIAL_INFINITY;
		new_RMaterialStruct.strength = MATERIAL_INFINITY;
		new_RMaterialStruct.static_mat = true;
		new_RMaterialStruct.deleted = false;
		materials.insert(new_RMaterialStruct,materials.size());
		return 0;
	}
	
	cNumber Roket_PhysicsMaterialHandler::getMaterialPropertyResistance(int ID)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				return materials[ID].resistance;
			}
		}
		return -1;
	}
	
	cNumber Roket_PhysicsMaterialHandler::getMaterialPropertyStrength(int ID)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				return materials[ID].strength;
			}
		}
		return -1;
	}

	bool Roket_PhysicsMaterialHandler::getMaterialPropertyStatic(int ID)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				return materials[ID].static_mat;
			}
		}
		return -1;
	}

	void Roket_PhysicsMaterialHandler::setMaterialPropertyResistance(int ID, cNumber newval)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				materials[ID].resistance = newval;
				return;
			}
		}
	}
	
	void Roket_PhysicsMaterialHandler::setMaterialPropertyStrength(int ID, cNumber newval)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				materials[ID].strength = newval;
				return;
			}
		}
	}
	
	void Roket_PhysicsMaterialHandler::setMaterialPropertyStatic(int ID, bool newval)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				materials[ID].static_mat = newval;
				return;
			}
		}
	}
	
	int Roket_PhysicsMaterialHandler::getMaterialID(const wchar_t* name)
	{
		for (int i=0;i<materials.size();i+=1)
		{
			if (strcmp(ConvertToChar(materials[i].name),ConvertToChar(name))==0)
			{
				return i;
			}
		}
		return -1;
	}

	void Roket_PhysicsMaterialHandler::addSceneNode(int ID, scene::RSceneNode* node)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				materials[ID].usagenodes.insert(node,materials[ID].usagenodes.size());
				return;
			}
		}
	}

	void Roket_PhysicsMaterialHandler::removeSceneNode(int ID, scene::RSceneNode* node)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				int result = materials[ID].usagenodes.binary_search(node);
				if (result != -1)
				{
					materials[ID].usagenodes.erase(result);
				}
				return;
			}
		}
	}

	/*
	TODO: Make this function free the memory.
	void Roket_PhysicsMaterialHandler::removeMaterial(int ID)
	{
		if (ID<materials.size())
		{
			if (materials[ID].deleted == false)
			{
				materials[ID].deleted = true;
				return;
			}
		}
	}
	*/
}
