//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// simple 4d vector class
//********************************************************************

#include <toolbox_stdafx.h>
#include "RenderPrimitive.h"
#include "../NewtonDemos.h"
#include "../PhysicsUtils.h"
#include "../ToolBox/MousePick.h"
#include "../ToolBox/OpenGlUtil.h"
#include "../ToolBox/DebugDisplay.h"



// structure use to hold game play especial effects
class BreakPrimitiveEffect
{
	public:

	// create the resources for this materials effect here
	void Init (char* effectName)
	{
	}

	// destroy all resources for this material interaction here
	void Detroy ()
	{
	}

	// play the impact sound
	void PlayImpactSound (dFloat volume)
	{
	}

	// play scratch sound
	void PlayScratchSound (dFloat volume)
	{
	}

	NewtonBody* CreateSplitedBody (const NewtonBody* source, NewtonCollision* shape, NewtonMesh* visualMesh)
	{
		dFloat Ixx;
		dFloat Iyy;
		dFloat Izz;
		dFloat mass;
		dFloat volume;
		dFloat shapeVolume;
		NewtonWorld* world;
		NewtonBody* rigidBody;
		NewtonCollision* collision;
		dGeometry* meshInstance;
		RenderPrimitive* parent;
		RenderPrimitive* base;
		RenderPrimitive* primitive;

		base = (RenderPrimitive*) NewtonBodyGetUserData (source);
		parent = (RenderPrimitive*) base->GetParent();


		dMatrix matrix;
		NewtonBodyGetMatrix (source, &matrix[0][0]);
		NewtonBodyGetMassMatrix (source, &mass, &Ixx, &Iyy, &Izz);
		collision = NewtonBodyGetCollision (source);
		volume = NewtonConvexCollisionCalculateVolume (collision);

		// make a visual object
		world = NewtonBodyGetWorld (source);
		meshInstance = new dGeometry();

		int vertexCount;
		vertexCount = NewtonMeshGetVertexCount (visualMesh); 
		meshInstance->AllocVertexData(vertexCount);
		NewtonMeshGetVertexStreams (visualMesh, 
									3 * sizeof (GLfloat), (dFloat*) meshInstance->m_vertex,
									3 * sizeof (GLfloat), (dFloat*) meshInstance->m_normal,
									2 * sizeof (GLfloat), (dFloat*) meshInstance->m_uv);

		for (int handle = NewtonMeshFirstMaterial (visualMesh); handle != -1; handle = NewtonMeshNextMaterial (visualMesh, handle)) {
			int material; 
			int indexCount; 
			dGeometrySegment* segment;
			material = NewtonMeshMaterialGetMaterial (visualMesh, handle); 
			indexCount = NewtonMeshMaterialGetIndexCount (visualMesh, handle); 

			segment = meshInstance->AddSegment();
			segment->m_texture = (GLuint)material;
			segment->AllocIndexData (indexCount);
			NewtonMeshMaterialGetIndexStreamShort (visualMesh, handle, (short int*)segment->m_indexes); 
		}


		// create a visual geometry
		primitive = new RenderPrimitive (parent, matrix, meshInstance);
		meshInstance->Release();

		//primitive->m_destrctionLevel = base->m_destrctionLevel - 1;
		primitive->m_controlVolume = base->m_controlVolume;
		primitive->m_destrutableStrength = base->m_destrutableStrength * 2.0f;


		// calculate the moment of inertia and the relative center of mass of the solid
		dVector inertia;
		dVector origin;
		shapeVolume = NewtonConvexCollisionCalculateVolume (shape);
		NewtonConvexCollisionCalculateInertialMatrix (shape, &inertia[0], &origin[0]);	

		mass = mass * shapeVolume / volume;
		Ixx = mass * inertia[0];
		Iyy = mass * inertia[1];
		Izz = mass * inertia[2];


		//create the rigid body
		rigidBody = NewtonCreateBody (world, shape);

		// set the correct center of gravity for this body
		NewtonBodySetCentreOfMass (rigidBody, &origin[0]);

		// set the mass matrix
		NewtonBodySetMassMatrix (rigidBody, mass, Ixx, Iyy, Izz);

		// save the pointer to the graphic object with the body.
		NewtonBodySetUserData (rigidBody, primitive);

		// assign the wood id
		NewtonBodySetMaterialGroupID (rigidBody, NewtonBodyGetMaterialGroupID(source));

		// set continue collision mode
		NewtonBodySetContinuousCollisionMode (rigidBody, 1);

		// set a destructor for this rigid body
		NewtonBodySetDestructorCallback (rigidBody, PhysicsBodyDestructor);

		// set the transform call back function
		NewtonBodySetTransformCallback (rigidBody, PhysicsSetTransform);

		// set the force and torque call back function
		NewtonBodySetForceAndTorqueCallback (rigidBody, PhysicsApplyGravityForce);


		// set the matrix for both the rigid body and the graphic body
		NewtonBodySetMatrix (rigidBody, &matrix[0][0]);
		PhysicsSetTransform (rigidBody, &matrix[0][0], 0);

		dVector veloc;
		dVector omega;

		NewtonBodyGetVelocity(source, &veloc[0]);
		NewtonBodyGetOmega(source, &omega[0]);
		veloc += omega * matrix.RotateVector(origin);

		NewtonBodySetVelocity(rigidBody, &veloc[0]);
		NewtonBodySetOmega(rigidBody, &omega[0]);
		return rigidBody;
	}

	dMatrix CreateTextureMatrix (const dVector& plane)
	{
		dMatrix matrix (dgGrammSchmidt(plane));

		matrix.m_up = matrix.m_up.Scale (0.1f);
		matrix.m_right = matrix.m_right.Scale (0.1f);
		matrix = matrix.Transpose();
		matrix.m_posit.m_y = 0.1f;
		matrix.m_posit.m_z = 0.1f;
		return matrix;
	}


	void ApplyDestruction (const NewtonBody* body, int threadIndex)
	{
		int fractureCount;
		dMatrix matrix;
		NewtonWorld* world;
		NewtonMesh* effectMesh;
		RenderPrimitive* srcPrimitive;
		NewtonMesh* fractureMesh[16];

		world = NewtonBodyGetWorld (body);

		// get the visual primitive
		srcPrimitive = (RenderPrimitive*) NewtonBodyGetUserData (body);

		// make sure this primitive become harder to break in case it can not be split
		srcPrimitive->m_destrutableStrength *= 2.0f;

		if (threadIndex != 0) {
			threadIndex *=1;
		}

		// get the effect mesh
		effectMesh = srcPrimitive->m_specialEffect;

		// calculate the cut plane plane
		NewtonBodyGetMatrix (body, &matrix[0][0]);
		dVector plane (m_tangentDir0);
		plane.m_w = - (m_tangentDir0 % m_position);
		plane = matrix.UntransformPlane (plane);
		dMatrix textureMatrix (CreateTextureMatrix(plane));

		// lock other threads while doing this operation
		NewtonWorldCriticalSectionLock (world);

		// break the mesh into two pieces
		fractureCount = NewtonMeshPlaneClip (effectMesh, &plane[0], &textureMatrix[0][0], fractureMesh, 16, srcPrimitive->m_effectTexture);
		if (fractureCount) {
			int applySplite;
			dFloat srcVolume;
			NewtonCollision* collision[16];

			NewtonCollision* srcCollision;
			srcCollision = NewtonBodyGetCollision(body);
			srcVolume = NewtonConvexCollisionCalculateVolume (srcCollision);

			applySplite = 1;
			memset (collision, 0, sizeof (NewtonCollision*) * fractureCount);
			for (int i = 0; i < fractureCount; i ++) {
				dFloat volume;
				collision[i] = NewtonCreateConvexHullFromMesh (world, fractureMesh[i], 0.2f);
				volume = 0.0f;
				if (collision[i]) {
					volume = NewtonConvexCollisionCalculateVolume (collision[i]);
				}
				if (volume < 0.125f * srcVolume) {
					applySplite = 0;
					break;
				}
			}

			if (applySplite) {
				for (int i = 0; i < fractureCount; i ++) {
					NewtonBody* debridBody;
					RenderPrimitive* primitive;
					debridBody = CreateSplitedBody (body, collision[i], fractureMesh[i]);
					primitive = (RenderPrimitive*) NewtonBodyGetUserData (debridBody);
					primitive->m_specialEffect = fractureMesh[i];
					primitive->m_effectTexture = srcPrimitive->m_effectTexture;
				}
				srcPrimitive->m_controlVolume = 1.0e12f;
				NewtonDestroyBody(world, body);
			} else {
				// the volume of at least one of the fracture pieces was too small, ignore destruction and delete meshes
				for (int i = 0; i < fractureCount; i ++) {
					NewtonMeshDestroy(fractureMesh[i]);
				}
			}

			for (int i = 0; i < fractureCount; i ++) {
				if (collision[i]) {
					NewtonReleaseCollision(world, collision[i]);
				}
			}
		}

		// relase all oteh threads
		NewtonWorldCriticalSectionUnlock (world);
	}

	dVector m_normal;
	dVector m_position;
	dVector m_tangentDir0;
	dVector m_tangentDir1;
	dFloat m_contactMaxNormalSpeed;
	dFloat m_contactMaxTangentSpeed;

	void *soundHandle;
	void *particleHandle;
};


static BreakPrimitiveEffect destrutibleMaterial[8];


static void SetDemoCallbacks (DemosSystem& system, dSceneNode* scene)
{
	system.m_control = Keyboard;
	system.m_autoSleep = AutoSleep;
	system.m_showIslands = SetShowIslands;
	system.m_showContacts = SetShowContacts; 
	system.m_setMeshCollision = SetShowMeshCollision;
	system.m_scene = scene;
}


static dSceneNode* BuildFloorAndSceneRoot (DemosSystem& system)
{
	NewtonWorld* world;
	RenderPrimitive* floor;
	NewtonBody* floorBody;
	NewtonCollision* floorCollision;
	dGeometry* meshInstance;
	dSceneNode* scene;

	world = system.m_world;

	// create the sky box,
	scene = new SkyBoxPrimitive (NULL);

	// create the the floor graphic objects
	dVector floorSize (100.0f, 2.0f, 100.0f);
	dMatrix location (GetIdentityMatrix());
	location.m_posit.m_y = -5.0f; 


	// create a box for floor 
	floorCollision = NewtonCreateBox (world, floorSize.m_x, floorSize.m_y, floorSize.m_z, NULL); 

	//	meshInstance = dGeometry::MakeBox (world, size.m_x, size.m_y, size.m_z, "GrassAndDirt.tga");
	meshInstance = new dGeometry (floorCollision, "GrassAndDirt.tga", "metal_30.tga", "metal_30.tga");
	floor = new RenderPrimitive (scene, location, meshInstance);
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



	// save th3 callback
	SetDemoCallbacks (system, scene);

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-40.0f, 10.0f, 0.0f));

	return scene;
}



/*
static void BreakableMaterialProcessContacts (const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	_ASSERTE (0);

	dFloat speed;
	BreakPrimitiveEffect* effectArray;
	BreakPrimitiveEffect* currectEffect;

	// get the pointer to the special effect structure
	effectArray = (BreakPrimitiveEffect *)NewtonMaterialGetMaterialPairUserData (material);
	_ASSERTE (effectArray);
	currectEffect = &effectArray[threadIndex];

	// Get the maximum normal speed of this impact. this can be used for positioning collision sound
	speed = NewtonMaterialGetContactNormalSpeed (material);
	if (speed > currectEffect->m_contactMaxNormalSpeed) {
		// save the position of the contact (for 3d sound of particles effects)
		currectEffect->m_contactMaxNormalSpeed = speed;

		NewtonMaterialGetContactPositionAndNormal (material, &currectEffect->m_position.m_x, &currectEffect->m_normal.m_x);
		NewtonMaterialGetContactTangentDirections (material, &currectEffect->m_tangentDir0.m_x, &currectEffect->m_tangentDir1.m_x);
	}
	return GenericContactProcess (material, body0, body1, timestep, threadIndex);

}
*/

/*
static void DetructibleMaterialEnd (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex)
{
	#define MIN_CONTACT_SPEED 15
	#define MIN_SCRATCH_SPEED 5

	BreakPrimitiveEffect* effectArray;
	BreakPrimitiveEffect* currectEffect;

	// get the pointer to the special effect structure
	effectArray = (BreakPrimitiveEffect *)NewtonMaterialGetMaterialPairUserData (material);
	_ASSERTE (effectArray);
	currectEffect = &effectArray[threadIndex];

	// if the max contact speed is larger than some minimum value. play a sound
//	if (currectEffect->m_contactMaxNormalSpeed > MIN_CONTACT_SPEED) {
//		currectEffect->PlayImpactSound (currectEffect->m_contactMaxNormalSpeed - MIN_CONTACT_SPEED);
//	}

	// if the max contact speed is larger than some minimum value. play a sound
//	if (currectEffect->m_contactMaxNormalSpeed > MIN_SCRATCH_SPEED) {
//		currectEffect->PlayScratchSound (currectEffect->m_contactMaxNormalSpeed - MIN_SCRATCH_SPEED);
//	}

	RenderPrimitive* primitive;
	primitive = (RenderPrimitive*) NewtonBodyGetUserData (body0);
	//	if ((primitive->m_destrctionLevel > 0) && (currectEffect->m_contactMaxNormalSpeed > primitive->m_destrutableStrength)) {
	if (primitive->m_specialEffect && (currectEffect->m_contactMaxNormalSpeed > primitive->m_destrutableStrength)) {
		NewtonCollision* collision;
		collision = NewtonBodyGetCollision (body0);
		if (NewtonConvexCollisionCalculateVolume (collision) > primitive->m_controlVolume) {
			currectEffect->ApplyDestruction (body0, threadIndex);
		}
	}

	primitive = (RenderPrimitive*) NewtonBodyGetUserData (body1);
	//	if ((primitive->m_destrctionLevel > 0) && (currectEffect->m_contactMaxNormalSpeed > primitive->m_destrutableStrength)) {
	if (primitive->m_specialEffect && (currectEffect->m_contactMaxNormalSpeed > primitive->m_destrutableStrength)) {
		NewtonCollision* collision;
		collision = NewtonBodyGetCollision (body1);
		if (NewtonConvexCollisionCalculateVolume (collision) > primitive->m_controlVolume) {
			currectEffect->ApplyDestruction (body1, threadIndex);
		}
	}
	// implement here any other effects

	// clear the contact normal speed 
	currectEffect->m_contactMaxNormalSpeed = 0.0f;

	// clear the contact sliding speed 
	currectEffect->m_contactMaxTangentSpeed = 0.0f;
}
*/


static void BreakableStruture (DemosSystem& system, dVector location, int high)
{
	int i;
	dFloat plankMass;
	dFloat columnMass;

	// /////////////////////////////////////////////////////////////////////
	//
	// Build a parking lot type structure

	dVector columnBoxSize (3.0f, 1.0f, 1.0f);
	//	dVector columnBoxSize (3.0f, 3.0f, 1.0f);
	dVector plankBoxSize (6.0f, 1.0f, 6.0f);

	// create the stack
	dMatrix baseMatrix (GetIdentityMatrix());

	// get the floor position in from o the camera
	baseMatrix.m_posit = location;
	baseMatrix.m_posit.m_y += columnBoxSize.m_x;

	// set realistic (extremes in this case for 24 bits precision) masses for the different components
	// note how the newton engine handle different masses ratios without compromising stability, 
	// we recommend the application keep this ration under 100 for contacts and 50 for joints 
	columnMass = 1.0f;
	plankMass = 20.0f;
//	 plankMass = 1.0f;

	// create a matrial carrier to to cou collision wit ethsi obejted
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);

	dMatrix columAlignment (dRollMatrix(3.1416f * 0.5f));
	for (i = 0; i < high; i ++) { 
		NewtonBody* body;
		RenderPrimitive* primitive;

		dMatrix matrix(columAlignment * baseMatrix);


		// add the 4 column
		matrix.m_posit.m_x -=  (columnBoxSize.m_z - plankBoxSize.m_x) * 0.5f;
		matrix.m_posit.m_z -=  (columnBoxSize.m_z - plankBoxSize.m_z) * 0.5f;
		body = CreateGenericSolid (system.m_world, system.m_scene, columnMass, matrix, columnBoxSize, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
		primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);
		primitive->m_destrutableStrength = 30;
		ConvexCastPlacement (body);

		matrix.m_posit.m_x += columnBoxSize.m_z - plankBoxSize.m_x;
		body = CreateGenericSolid (system.m_world, system.m_scene, columnMass, matrix, columnBoxSize, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
		primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);
		primitive->m_destrutableStrength = 30;
		ConvexCastPlacement (body);

		matrix.m_posit.m_z += columnBoxSize.m_z - plankBoxSize.m_z;		
		body = CreateGenericSolid (system.m_world, system.m_scene, columnMass, matrix, columnBoxSize, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
		primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);
		primitive->m_destrutableStrength = 30;
		ConvexCastPlacement (body);

		matrix.m_posit.m_x -= columnBoxSize.m_z - plankBoxSize.m_x;
		body = CreateGenericSolid (system.m_world, system.m_scene, columnMass, matrix, columnBoxSize, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
		primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);
		primitive->m_destrutableStrength = 30;
		ConvexCastPlacement (body);

		// add a plank
		dVector size (plankBoxSize);
		size.m_x *= 0.85f;
		size.m_z *= 0.85f;
		body = CreateGenericSolid (system.m_world, system.m_scene, plankMass, baseMatrix, size, _BOX_PRIMITIVE, defaultMaterialID);
		primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);
		primitive->m_destrutableStrength = 20;
		ConvexCastPlacement (body);

		// set up for another level
		baseMatrix.m_posit.m_y += (columnBoxSize.m_x + plankBoxSize.m_y);
	}

	dFloat mass;
	NewtonBody* body;
	RenderPrimitive* primitive;
	PrimitiveType type = _BOX_PRIMITIVE;
	dVector size (1.0f, 2.0f, 1.0f);
	dMatrix matrix (GetIdentityMatrix());

	mass = 10.0f;
	matrix.m_posit = location;
	matrix.m_posit.m_y = FindFloor (system.m_world, matrix.m_posit.m_x, matrix.m_posit.m_z) + baseMatrix.m_posit.m_y + 5.0f; 

	body = CreateGenericSolid (system.m_world, system.m_scene, mass, matrix, size, type, defaultMaterialID);
	primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);
	primitive->m_destrutableStrength = 200000000;
}



void BreakableStruture (DemosSystem& system)
{
	NewtonWorld* world;
	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	// set contact callback for material destruction
	int defaultID;

	for (int i = 0; i < int (sizeof (destrutibleMaterial) / sizeof (destrutibleMaterial[0])); i ++) {
		destrutibleMaterial[i].Init((char*)"destruction");
	}
	defaultID = NewtonMaterialGetDefaultGroupID (system.m_world);
	_ASSERTE (0);
//	NewtonMaterialSetCollisionCallback (system.m_world, defaultID, defaultID, destrutibleMaterial, NULL, BreakableMaterialProcessContacts, DetructibleMaterialEnd); 

	BreakableStruture (system, dVector (-10.0f, 0.0f, -10.0f, 0.0f), 5);
	BreakableStruture (system, dVector ( 10.0f, 0.0f, -10.0f, 0.0f), 5);
	BreakableStruture (system, dVector (-10.0f, 0.0f,  10.0f, 0.0f), 5);
	BreakableStruture (system, dVector ( 10.0f, 0.0f,  10.0f, 0.0f), 5);
}


