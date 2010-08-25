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

static void UserContactRestitution (const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dFloat mass;
	dFloat restitution; 
	const NewtonBody* body;
	const NewtonBody* body0;
	const NewtonBody* body1;

	// call  the basic call back
	GenericContactProcess (contactJoint, timestep, threadIndex);

	body0 = NewtonJointGetBody0(contactJoint);
	body1 = NewtonJointGetBody1(contactJoint);

	body = body0;
	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	if (mass == 0.0f) {
		body = body1;
	}

	for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact)) {
		RenderPrimitive* node;
		NewtonMaterial* material;

		material = NewtonContactGetMaterial (contact);
		node = (RenderPrimitive*) NewtonBodyGetUserData (body);
		
		restitution = node->m_density;

	//	NewtonMaterialSetContactFrictionCoef (material, friction, friction, 0);
	//	NewtonMaterialSetContactFrictionCoef (material, friction, friction, 1);
		NewtonMaterialSetContactElasticity (material, restitution);
	}
}


static void SetDemoCallbacks (SceneManager& system)
{
	system.m_control = Keyboard;
	system.m_autoSleep = AutoSleep;
	system.m_showIslands = SetShowIslands;
	system.m_showContacts = SetShowContacts; 
	system.m_setMeshCollision = SetShowMeshCollision;
//	system.m_scene = scene;
}


static void BuildFloorAndSceneRoot (SceneManager& system)
{
	NewtonWorld* world;
	RenderPrimitive* floor;
	NewtonBody* floorBody;
	NewtonCollision* floorCollision;
	OGLMesh* meshInstance;
//	dSceneNode* scene;

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


void Restitution (SceneManager& system)
{
	int zCount;
	int defaultMaterialID;
	dFloat spacing;
	NewtonWorld* world;
	

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);
	NewtonMaterialSetCollisionCallback (world, defaultMaterialID, defaultMaterialID, NULL, NULL, UserContactRestitution); 

	// create a friction Ramp
	dVector location (cameraEyepoint + cameraDir.Scale (40.0f));
	dMatrix matrix (GetIdentityMatrix());


	zCount = 10;
	spacing = 4.0f;
	dVector origin (matrix.m_posit);
	origin.m_x -= 10.0f;

	// create 
	for (int i = 0; i < zCount; i ++) {
		dFloat z;
		dFloat x;
		dFloat mass;
		NewtonBody* body;
		dVector size (1.0f, 0.5f, 2.0f, 0.0f);

		x = origin.m_x;
		z = origin.m_z + (i - zCount / 2) * spacing;

		mass = 1.0f;
		matrix.m_posit.m_x = x;
		matrix.m_posit.m_y = FindFloor (system.m_world, x, z) + size.m_y * 20.0f;
		matrix.m_posit.m_z = z;
		
		body = CreateGenericSolid (system.m_world, &system, mass, matrix, size, _SPHERE_PRIMITIVE, defaultMaterialID);
		NewtonBodySetLinearDamping (body, 0.0f);

		RenderPrimitive* node;
		node = (RenderPrimitive*) NewtonBodyGetUserData (body);
		node->m_density = i * 0.1f + 0.083f;

		matrix.m_posit.m_y += 4.0f;
		body = CreateGenericSolid (system.m_world, &system, mass, matrix, size, _SPHERE_PRIMITIVE, defaultMaterialID);
		NewtonBodySetLinearDamping (body, 0.0f);

		// save a coerfienet of frition in the the primitive
		node = (RenderPrimitive*) NewtonBodyGetUserData (body);
		node->m_density = i * 0.1f + 0.083f;


		matrix.m_posit.m_y += 4.0f;
		body = CreateGenericSolid (system.m_world, &system, mass, matrix, size, _SPHERE_PRIMITIVE, defaultMaterialID);
		NewtonBodySetLinearDamping (body, 0.0f);

		// save a coerfienet of frition in the the primitive
		node = (RenderPrimitive*) NewtonBodyGetUserData (body);
		node->m_density = i * 0.1f + 0.083f;

		matrix.m_posit.m_y += 4.0f;
		body = CreateGenericSolid (system.m_world, &system, mass, matrix, size, _BOX_PRIMITIVE, defaultMaterialID);
		NewtonBodySetLinearDamping (body, 0.0f);

		// save a coerfienet of frition in the the primitive
		node = (RenderPrimitive*) NewtonBodyGetUserData (body);
		node->m_density = i * 0.1f + 0.083f;
	}
	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-60.0f, 10.0f, 0.0f));
}

