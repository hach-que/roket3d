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


static void BuildJenga (SceneManager& system, dVector origin, int hight)
{
	int i;
	int j;
	dFloat gap;
	dFloat mass;

	// /////////////////////////////////////////////////////////////////////
	//
	// build a standard block stack of 20 * 3 boxes for a total of 60

	// set the Jenga rectangle using the golden ratio for base (1 * 0.5f, 1.62 * 0.5f, 2.0f)
	dVector blockBoxSize (0.8f, 0.5f, 0.8f * 3.0f);
	blockBoxSize = blockBoxSize.Scale (2.0f);

	// create the stack
	dMatrix baseMatrix (GetIdentityMatrix());

	// for the elevation of the floor at the stack position
	baseMatrix.m_posit.m_x = origin.m_x;
	baseMatrix.m_posit.m_z = origin.m_z;

	baseMatrix.m_posit.m_y = FindFloor (system.m_world, baseMatrix.m_posit.m_x, baseMatrix.m_posit.m_z); 
	baseMatrix.m_posit.m_y += blockBoxSize.m_y / 2.0f;


	// set realistic mass and inertia matrix for each block
	mass = 5.0f;

	// create a 90 degree rotation matrix
	dMatrix rotMatrix (dYawMatrix (3.141592f * 0.5f));

	// create a matrial carrier to to cou collision wit ethsi obejted
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);

	// separate a bit the block alone the horizontal direction
	gap = 0.01f;
	for (i = 0; i < hight; i ++) { 
		dMatrix matrix(baseMatrix);
		matrix.m_posit -= matrix.m_front.Scale (blockBoxSize.m_x - gap); 

		for (j = 0; j < 3; j ++) { 
			CreateGenericSolid (system.m_world, &system, mass, matrix, blockBoxSize, _BOX_PRIMITIVE, defaultMaterialID);
			matrix.m_posit += matrix.m_front.Scale (blockBoxSize.m_x + gap);  
		}

		baseMatrix = rotMatrix * baseMatrix;
		baseMatrix.m_posit += matrix.m_up.Scale (blockBoxSize.m_y * 0.99f);
	}
}


void Jenga (SceneManager& system)
{
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

//AddBoxes(&system, 10.0f, dVector ( 10.0f, 0.0f,  10.0f), dVector ( 2.0f, 1.0f, 2.0f), 1, 1, 10.0f, _BOX_PRIMITIVE, 0);

	// place for jenga games
	BuildJenga (system, dVector ( 10.0f, 0.0f,  10.0f), 20);
	BuildJenga (system, dVector (-10.0f, 0.0f,  10.0f), 20);
	BuildJenga (system, dVector ( 10.0f, 0.0f, -10.0f), 20);
	BuildJenga (system, dVector (-10.0f, 0.0f, -10.0f), 20);
}

void JengaTall (SceneManager& system)
{
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	// place for jenag games
	BuildJenga (system, dVector ( 0.0f, 0.0f,  0.0f), 50);
}

