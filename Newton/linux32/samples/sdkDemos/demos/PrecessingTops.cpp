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



void PrecessingTops (SceneManager& system)
{
	int xCount;
	int zCount;
	int defaultMaterialID;
	dFloat x;
	dFloat z;
	dFloat spacing;
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);
	dVector location (cameraEyepoint + cameraDir.Scale (20.0f));
	
	xCount = 10;
	zCount = 10;
	spacing = 6.0f;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);

	dVector size (3.0f, 1.0f, 0.5f);
	dVector origin (cameraEyepoint + cameraDir.Scale (20.0f));

	// craete teh conpound pieces;
	dMatrix matrix (dRollMatrix (- 70.0f * 3.141592f / 180.0f));
	dVector omega (-40.0f, 0.0f, 0.0f);
	omega = matrix.RotateVector (omega);
	//	dVector origin (cameraEyepoint + cameraDir.Scale (80.0f));
	for (int i = 0; i < xCount; i ++) {
		x = origin.m_x + (i - xCount / 2) * spacing;
		for (int j = 0; j < zCount; j ++) {
			dFloat mass;
			NewtonBody* body;
			z = origin.m_z + (j - zCount / 2) * spacing;

			mass = 1.0f;
			// create a simple primitive
			matrix.m_posit.m_x = x;
			matrix.m_posit.m_y = FindFloor (system.m_world, x, z) + 5.0f;
			matrix.m_posit.m_z = z;

			body = CreateGenericSolid (system.m_world, &system, mass, matrix, size, _CONE_PRIMITIVE, defaultMaterialID);
			NewtonBodySetOmega (body, &omega[0]);

			dVector damp (0.0f, 0.0f, 0.0f);
			NewtonBodySetAngularDamping (body, &damp[0]);
		}
	}

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-60.0f, 10.0f, 0.0f));
}



