#ifndef HEADER_RPHYSICSMANAGER
#define HEADER_RPHYSICSMANAGER
namespace physics
{
	class RPhysicsManager : public RObject
	{
		Roket_PhysicsManager * obj;
		core::RDevice* roketdevice;
		bool objectExists;

		public:
			RPhysicsManager(lua_State* L, bool instantedByUser);
			int updateAll(lua_State* L);
			int setMinimumFPS(lua_State* L);
			int setSolver(lua_State* L);
			int setFrictionModel(lua_State* L);
			int createJoint(lua_State* L);
			
			int getMaterialHandler(lua_State* L);

			int setObject(Roket_PhysicsManager* object, core::RDevice* rd);
			Roket_PhysicsManager* getObject();
			Roket_PhysicsBody* checkBody(lua_State* L, int pos);

			static const char *className;
			static const Luna < RPhysicsManager >::FunctionType Functions[];
			static const Luna < RPhysicsManager >::PropertyType Properties[];
	};
}
#endif
