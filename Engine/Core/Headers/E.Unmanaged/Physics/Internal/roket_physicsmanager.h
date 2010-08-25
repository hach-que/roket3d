#ifndef HEADER_ROKETPHYSICSMANAGER
#define HEADER_ROKETPHYSICSMANAGER

#include "roket_physicsbody.h"
#include "roket_physicsmaterialhandler.h"

namespace physics
{
	class Roket_PhysicsManager
	{
		physics::Roket_PhysicsMaterialHandler* materials;
		NewtonWorld* world;

		private:
			void * memory_allocate(int size);
			void memory_free(void * ptr);
		public:
			Roket_PhysicsManager();
			~Roket_PhysicsManager();
			void updateAll();
			void setSolver(int n);
			void setFrictionModel(int n);
			void setMinimumFPS(int n);
			void destroyAllBodies();
			void destroyBody(Roket_PhysicsBody* body);
			Roket_PhysicsMaterialHandler* getMaterialHandler();
			NewtonWorld* getNewtonWorld();
			//void setMeterUnit(int n);
			
	};
}
#endif
