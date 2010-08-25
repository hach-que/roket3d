//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// simple 4d vector class
//********************************************************************
#include <toolbox_stdafx.h>
#include "SkyBox.h"
#include "RenderPrimitive.h"
#include "../OGLMesh.h"
#include "../SceneManager.h"
#include "../PhysicsUtils.h"
#include "../toolBox/MousePick.h"
#include "../toolBox/OpenGlUtil.h"
#include "../toolBox/DebugDisplay.h"

static void SetDemoCallbacks (SceneManager& system)
{
	system.m_control = Keyboard;
	system.m_autoSleep = AutoSleep;
	system.m_showIslands = SetShowIslands;
	system.m_showContacts = SetShowContacts; 
	system.m_setMeshCollision = SetShowMeshCollision;
}

static void BuildFloorAndSceneRoot (SceneManager& system)
{
	NewtonWorld* world;
	RenderPrimitive* floor;
	NewtonBody* floorBody;
	NewtonCollision* floorCollision;
	OGLMesh* meshInstance;

	world = system.m_world;
	// /////////////////////////////////////////////////////////////////////
	//
	// create the sky box,
	system.AddModel (new SkyBox ());


	// create the the floor graphic objects
	dVector floorSize (100.0f, 2.0f, 100.0f);
	dMatrix location (GetIdentityMatrix());
	location.m_posit.m_y = -5.0f; 

	// create a box for floor 
	floorCollision = NewtonCreateBox (world, floorSize.m_x, floorSize.m_y, floorSize.m_z, NULL); 

	//	meshInstance = OGLMesh::MakeBox (world, size.m_x, size.m_y, size.m_z, "GrassAndDirt.tga");
	meshInstance = new OGLMesh (floorCollision, "GrassAndDirt.tga", "metal_30.tga", "metal_30.tga");
	floor = new RenderPrimitive (location, meshInstance);
	system.AddModel (floor);
	meshInstance->Release();

	// create the the floor collision, and body with default values
	floorBody = NewtonCreateBody (world, floorCollision);
	NewtonReleaseCollision (world, floorCollision);


	// set the transformation for this rigid body
	NewtonBodySetMatrix (floorBody, &location[0][0]);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData (floorBody, floor);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback (floorBody, PhysicsBodyDestructor);


	// get the default material ID
	int defaultID;
	defaultID = NewtonMaterialGetDefaultGroupID (world);

	// set default material properties
	NewtonMaterialSetDefaultSoftness (world, defaultID, defaultID, 0.05f);
	NewtonMaterialSetDefaultElasticity (world, defaultID, defaultID, 0.4f);
	NewtonMaterialSetDefaultCollidable (world, defaultID, defaultID, 1);
	NewtonMaterialSetDefaultFriction (world, defaultID, defaultID, 1.0f, 0.5f);
	NewtonMaterialSetCollisionCallback (world, defaultID, defaultID, NULL, NULL, GenericContactProcess); 

//	NewtonMaterialSetSurfaceThickness(world, materialID, materialID, 0.1f);
	NewtonMaterialSetSurfaceThickness(world, defaultID, defaultID, 0.0f);

	// set the island update callback
	NewtonSetIslandUpdateEvent (world, PhysicsIslandUpdate);

	// save the callback
	SetDemoCallbacks (system);

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-40.0f, 10.0f, 0.0f));
}


static void CreatePyramid (SceneManager& system, dVector origin, int high)
{
	// /////////////////////////////////////////////////////////////////////
	//
	// build a standard block stack of 20 * 3 boxes for a total of 60
	dVector blockBoxSize (2.0f, 1.5f, 3.0f);
	//	 blockBoxSize = blockBoxSize.Scale (2.0f);


	// create the stack
	dMatrix baseMatrix (GetIdentityMatrix());

	// for the elevation of the floor at the stack position
	baseMatrix.m_posit.m_x = origin.m_x;
	baseMatrix.m_posit.m_z = origin.m_z;

	baseMatrix.m_posit.m_y = FindFloor (system.m_world, baseMatrix.m_posit.m_x, baseMatrix.m_posit.m_z); 
	baseMatrix.m_posit.m_y += blockBoxSize.m_y / 2.0f;


	// set realistic mass and inertia matrix for each block
	float mass = 5.0f;

	// create a matrial carrier to to cou collision wit ethsi obejted
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);


	dMatrix spin (dYawMatrix( (3.141592f)));

	float step = blockBoxSize.m_x + 0.1f;
	baseMatrix.m_posit.m_x -= step * high / 2.0f - 5.0f;
	float x0 = baseMatrix.m_posit.m_x;
	for (int i = 0; i < high; i ++) {
		baseMatrix.m_posit.m_x = x0;
		for (int j = 0; j < high - i; j ++) {
			CreateGenericSolid (system.m_world, &system, mass, baseMatrix, blockBoxSize, _BOX_PRIMITIVE, defaultMaterialID);
			baseMatrix = spin * baseMatrix;
			baseMatrix.m_posit.m_x += step ;
		}
		x0 += step * 0.5f;
		baseMatrix.m_posit.m_y += blockBoxSize.m_y;
	}
}


void CreatePyramid (SceneManager& system)
{
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	// place four pyramids games
	CreatePyramid (system, dVector (0.0f, 0.0f,  30.0f), 12);
	CreatePyramid (system, dVector (0.0f, 0.0f,  15.0f), 12);
	CreatePyramid (system, dVector (0.0f, 0.0f,   0.0f), 12);
	CreatePyramid (system, dVector (0.0f, 0.0f, -15.0f), 12);
	CreatePyramid (system, dVector (0.0f, 0.0f, -30.0f), 12);
}


void CreatePyramidTall (SceneManager& system)
{
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	// place four pyramids games
	CreatePyramid (system, dVector ( 0.0f, 0.0f, -20.0f), 30);
	CreatePyramid (system, dVector ( 0.0f, 0.0f,  20.0f), 30);



//CreatePyramid (system, dVector ( 0.0f, 0.0f, -20.0f), 20);
//CreatePyramid (system, dVector ( 0.0f, 0.0f,  20.0f), 20);

}





