#include "roket3d.h"
#include "E.Unmanaged/Physics/Internal/roket_physicsmanager.h"

namespace physics
{
	Roket_PhysicsManager::~Roket_PhysicsManager()
	{
		NewtonDestroy(world);
//		if (memoryAllocated > 0)
//			nonlua_debug("Possible memory leak in physics management.",LVL_WARN);
	}
	
	void Roket_PhysicsManager::updateAll()
	{
		// initalize stuff here
		//cout << "Performing update..." << endl;
		NewtonUpdate(world,0.01f);
	}
	
	void Roket_PhysicsManager::setSolver(int n)
	{
		// initalize stuff here
		NewtonSetSolverModel(world,n);
	}
	
	void Roket_PhysicsManager::setFrictionModel(int n)
	{
		// initalize stuff here
		NewtonSetFrictionModel(world,n);
	}
	
	void Roket_PhysicsManager::setMinimumFPS(int n)
	{
		// initalize stuff here
		NewtonSetMinimumFrameRate(world,n);
	}

	/*
	Roket_PhysicsMaterialHandler* Roket_PhysicsManager::getMaterialHandler()
	{
		return materials;
	}
	*/

	NewtonWorld* Roket_PhysicsManager::getNewtonWorld()
	{
		return world;
	}

	//void Roket_PhysicsManager::setMeterUnit(int n)
	//{
		// initalize stuff here
		//NewtonSetGlobalScale(world,n);
	//}
	
	// TODO: Make this work.  Newton wants static functions.
	/*
	void * Roket_PhysicsManager::memory_allocate(int size)
	{
		// this function is used for monitoring the amount
		// of memory that newton is using and can be
		// accessed from the RPhysicsManager:getMemoryUsed()
		memoryAllocated += size;
		return malloc(size);
	}
	
	void Roket_PhysicsManager::memory_free(void * ptr)
	{
		// this function is used for monitoring the amount
		// of memory that newton is using and can be
		// accessed from the RPhysicsManager:getMemoryUsed()
		memoryAllocated -= sizeof(ptr);
		free(ptr);
	}
	*/
	
	Roket_PhysicsManager::Roket_PhysicsManager()
	{
		// this is the variable that stores the amount
		// of memory used by newton
//		memoryAllocated = 0;
		
		// create material handler
		materials = new Roket_PhysicsMaterialHandler();

		// create newton
		world = NewtonCreate(NULL,NULL);//boost::bind(,&(*this->*memory_free));
		float min[3];
		float max[3];

		min[0] = -10000.0;
		min[1] = -10000.0;
		min[2] = -10000.0;

		max[0] = 10000.0;
		max[1] = 10000.0;
		max[2] = 10000.0;

		NewtonSetWorldSize( world, min, max );
	}
}
