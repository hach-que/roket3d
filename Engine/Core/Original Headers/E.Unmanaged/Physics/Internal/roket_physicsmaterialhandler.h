#ifndef HEADER_ROKETPHYSICSMATERIALHANDLER
#define HEADER_ROKETPHYSICSMATERIALHANDLER

namespace physics
{
	struct RMaterialStruct; // structure definition

	class Roket_PhysicsMaterialHandler
	{
		irr::core::array<RMaterialStruct> materials;
		
		public:
			Roket_PhysicsMaterialHandler();
			int newMaterial(wchar_t* name);
			cNumber getMaterialPropertyResistance(int ID);
			cNumber getMaterialPropertyStrength(int ID);
			bool getMaterialPropertyStatic(int ID);
			void setMaterialPropertyResistance(int ID, cNumber newval);
			void setMaterialPropertyStrength(int ID, cNumber newval);
			void setMaterialPropertyStatic(int ID, bool newval);
			void addSceneNode(int ID,scene::RSceneNode* node);
			void removeSceneNode(int ID,scene::RSceneNode* node);
			int getMaterialID(const wchar_t* name);
			//void removeMaterial(int ID);
	};
}

#endif
