#ifndef HEADER_PHYSICSMATERIALHANDLER
#define HEADER_PHYSICSMATERIALHANDLER

#include "roket_physicsmaterialhandler.h"

namespace physics
{
	class RPhysicsMaterialHandler : public RObject
	{
		Roket_PhysicsMaterialHandler* obj;
		bool objectExists;
		
		public:
			RPhysicsMaterialHandler(lua_State* L, bool instantedByUser);
			int newMaterial(lua_State* L);
			int getMaterialPropertyResistance(lua_State* L);
			int getMaterialPropertyStrength(lua_State* L);
			int getMaterialID(lua_State* L);
			//int removeMaterial(lua_State* L);
			
			int setObject(Roket_PhysicsMaterialHandler* object);
			Roket_PhysicsMaterialHandler* getObject();

			static const char *className;
			static const Luna < RPhysicsMaterialHandler >::FunctionType Functions[];
			static const Luna < RPhysicsMaterialHandler >::PropertyType Properties[];
	};
}
#endif
