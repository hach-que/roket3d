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


void CompoundCollision (SceneManager& system)
{
	int xCount;
	int zCount;
	int defaultMaterialID;
	dFloat x;
	dFloat z;
	dFloat spacing;
	NewtonWorld* world;
	NewtonCollision* coumpound;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);
	dVector location (cameraEyepoint + cameraDir.Scale (20.0f));
	
	xCount = 10;
	zCount = 10;
	spacing = 6.0f;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);

	dVector size (1.0f, 0.25f, 0.5f);
	dVector origin (cameraEyepoint + cameraDir.Scale (20.0f));

	// craete teh conpound pieces;

	NewtonCollision* snowMan[5];
	snowMan[0] = NewtonCreateSphere (system.m_world, 1.0f, 1.0f, 1.0f, NULL);

	dMatrix offset (GetIdentityMatrix());
	offset.m_posit.m_y += 1.0f + 0.75f;
	snowMan[1] = NewtonCreateSphere (system.m_world, 0.75f, 0.75f, 0.75f, &offset[0][0]);

	offset.m_posit.m_y += 0.75f + 0.5;
	snowMan[2] = NewtonCreateSphere (system.m_world, 0.5f, 0.6f, 0.5f, &offset[0][0]);

	offset.m_posit.m_y -= 0.75f + 0.25;
	offset.m_posit.m_z += 1.0f;
	offset = dYawMatrix (90.0f * 3.141592f / 180.0f) * offset;
	snowMan[3] = NewtonCreateCapsule (system.m_world, 0.35f, 2.0f, &offset[0][0]);

	offset.m_posit.m_z -= 2.0f;
	snowMan[4] = NewtonCreateCapsule (system.m_world, 0.35f, 2.0f, &offset[0][0]);

	// make a cmpound collision
	coumpound = NewtonCreateCompoundCollision (system.m_world, sizeof (snowMan) / sizeof (snowMan[1]), snowMan);
	for (int i = 0; i < int (sizeof (snowMan) / sizeof (snowMan[0])); i ++) {
		NewtonReleaseCollision (system.m_world, snowMan[i]);
	}

/*
test supportvertex
dVector dir (1, 0, 0, 0);
dVector vertex;
NewtonCollisionSupportVertex (coumpound, &dir[0], &vertex[0]);
*/


	// create the visual shape
	OGLMesh* geo;
	geo = new OGLMesh(coumpound, "wood_0.tga", "wood_0.tga", "wood_1.tga");


	dMatrix matrix (GetIdentityMatrix());
	for (int i = 0; i < xCount; i ++) {
		x = origin.m_x + (i - xCount / 2) * spacing;
		for (int j = 0; j < zCount; j ++) {
			dFloat mass;
			NewtonBody* body;
			RenderPrimitive* node;
			
			z = origin.m_z + (j - zCount / 2) * spacing;

			mass = 1.0f;
			// create a simple primitive
			matrix.m_posit.m_x = x;
			matrix.m_posit.m_y = FindFloor (system.m_world, x, z) + 5.0f;
			matrix.m_posit.m_z = z;
			body = CreateGenericSolid (system.m_world, &system, mass, matrix, size, _SPHERE_PRIMITIVE, defaultMaterialID);

			// make a compound collision
			NewtonBodySetCollision (body, coumpound);


			// now make a visual represenation to matcj the collison shape
			node = (RenderPrimitive*) NewtonBodyGetUserData (body);
//			node->RemoveMesh(node->m_meshList.GetFirst()->GetInfo().m_data.GetFirst()->GetInfo());
			node->RemoveMesh (node->GetMesh());
			node->AddMesh(geo);

			dVector origin;
			dVector inertia;
			NewtonConvexCollisionCalculateInertialMatrix (coumpound, &inertia[0], &origin[0]);	
			inertia = inertia.Scale (mass);
			NewtonBodySetMassMatrix (body, mass, inertia.m_x, inertia.m_y, inertia.m_z); 

			dVector com (0.0f, -1.0f, 0.0f, 0.0f);
			NewtonBodySetCentreOfMass (body, &com[0]);
		}
	}

	geo->Release();
	NewtonReleaseCollision (system.m_world, coumpound);

}



