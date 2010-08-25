#include <toolbox_stdafx.h>
#include "SceneManager.h"
#include "PhysicsUtils.h"
#include "toolBox/MousePick.h"
#include "toolBox/OpenGlUtil.h"
#include "toolBox/DebugDisplay.h"
#include "toolBox/RenderPrimitive.h"
#include "NewtonCustomJoint.h"
#include "OGLMesh.h"



#define CAMERA_SPEED 4.0f

dFloat rollAngle = 0.0f;
dFloat yawAngle = 0.0f;

dVector cameraDir (1.0f, 0.0f, 0.0f, 0.0f);
dVector cameraEyepoint (-40.0f, 10.0f, 0.0f, 0.0f);

static int showIslans;
static int showContacts;




#ifdef USE_TEST_SERIALIZATION
//static char* MAGIC_NUMBER = "serialize data";
static void SerializeFile (void* serializeHandle, const void* buffer, int size)
{
	fwrite (buffer, size, 1, (FILE*) serializeHandle);
}

static void DeSerializeFile (void* serializeHandle, void* buffer, int size)
{
	fread (buffer, size, 1, (FILE*) serializeHandle);
}
#endif


dFloat RandomVariable(dFloat amp)
{
	int val;
	val = dRand() + dRand();
	return amp * (dFloat (val) / dFloat(dRAND_MAX) - 1.0f) * 0.5f;
}




static unsigned ConvexCastCallback (const NewtonBody* body, const NewtonCollision* collision, void* userData)
{
	NewtonBody* me = (NewtonBody*) userData;
	return (me == body) ? 0 : 1;
}


static dFloat RayCastPlacement (const NewtonBody* body, const dFloat* normal, int collisionID, void* userData, dFloat intersetParam)
{
	dFloat* paramPtr;
	paramPtr = (dFloat*)userData;
	if (intersetParam < paramPtr[0]) {
		paramPtr[0] = intersetParam;
	}
	return paramPtr[0];
}


dFloat FindFloor (const NewtonWorld* world, dFloat x, dFloat z)
{
	dFloat parameter;

//parameter = 1.2f;
//float q0[] = {8.07129, -1.66341 - 1.5, 0};
//float q1[] = {8.04629, -3.1684  - 1.5, 0};
//NewtonWorldRayCast(world, q0, q1, RayCastPlacement, &parameter, NULL);



	// shot a vertical ray from a high altitude and collect the intersection parameter.
	dVector p0 (x, 1000.0f, z); 
	dVector p1 (x, -1000.0f, z); 

	parameter = 1.2f;
	NewtonWorldRayCast (world, &p0[0], &p1[0], RayCastPlacement, &parameter, NULL);
	//_ASSERTE (parameter < 1.0f);

	// the intersection is the interpolated value
	return 1000.0f - 2000.0f * parameter;
//return 0.0f;
}


void ConvexCastPlacement (NewtonBody* body)
{
	dFloat param;
	dMatrix matrix;
	NewtonWorld* world;
	NewtonCollision* collision;
	NewtonWorldConvexCastReturnInfo info[16];

	NewtonBodyGetMatrix (body, &matrix[0][0]);

	matrix.m_posit.m_y += 20.0f;
	dVector p (matrix.m_posit);
	p.m_y -= 40.0f;

	world = NewtonBodyGetWorld(body);
	collision = NewtonBodyGetCollision(body);
	NewtonWorldConvexCast (world, &matrix[0][0], &p[0], collision, &param, body, ConvexCastCallback, info, 16, 0);
	_ASSERTE (param < 1.0f);

	matrix.m_posit.m_y += (p.m_y - matrix.m_posit.m_y) * param;

	NewtonBodySetMatrix(body, &matrix[0][0]);
}


// set the transformation of a rigid body
void PhysicsSetTransform (const NewtonBody* body, const dFloat* matrix, int threadIndex)
{
	RenderPrimitive* primitive;

	// get the graphic object form the rigid body
	primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);

	// set the transformation matrix for this rigid body
	dMatrix& mat = *((dMatrix*)matrix);
	primitive->SetMatrix (mat);

}


// add force and torque to rigid body
void  PhysicsApplyGravityForce (const NewtonBody* body, dFloat timestep, int threadIndex)
{
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dFloat mass;

	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	dVector force (0.0f, -mass * 10.0f, 0.0f);
	NewtonBodySetForce (body, &force.m_x);
}


// rigid body destructor
void  PhysicsBodyDestructor (const NewtonBody* body)
{
	RenderPrimitive* primitive;

	// get the graphic object form the rigid body
	primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);

	// destroy the graphic object
//	delete primitive;
}

void SetShowContacts (SceneManager& me, int mode)
{
	showContacts = mode;
}



void AutoSleep (SceneManager& me, int mode)
{
	mode = mode ? 0 : 1;
	for (const NewtonBody* body = NewtonWorldGetFirstBody (me.m_world); body; body = NewtonWorldGetNextBody (me.m_world, body)) {
		NewtonBodySetAutoSleep (body, mode);
	}
}


void ShowContacts (const NewtonJoint* contactJoint)
{
	// the application can implement some kind of contact debug here
	if (showContacts) {
		NewtonWorld* world;

		world = NewtonBodyGetWorld(NewtonJointGetBody0(contactJoint));
		NewtonWorldCriticalSectionLock(world);
		for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact)) {
			dVector point;
			dVector normal;	

			NewtonMaterial* material;

			material = NewtonContactGetMaterial (contact);
			NewtonMaterialGetContactPositionAndNormal (material, &point.m_x, &normal.m_x);

			// if we are display debug info we need to block other threads from writing the data at the same time
			DebugDrawContact (point, normal);
		}
		NewtonWorldCriticalSectionUnlock(world);
	}
}

void ShowJointInfo(const NewtonCustomJoint* joint)
{
	NewtonJointRecord info;

	if (showContacts) {
		joint->GetInfo (&info);

		dMatrix bodyMatrix0;
		NewtonBodyGetMatrix (info.m_attachBody_0, &bodyMatrix0[0][0]);
		dMatrix matrix0 (*((dMatrix*) &info.m_attachmenMatrix_0[0]));
		matrix0 = matrix0 * bodyMatrix0;
		DebugDrawLine (matrix0.m_posit, matrix0.m_posit + matrix0.m_front, dVector (1, 0, 0));
		DebugDrawLine (matrix0.m_posit, matrix0.m_posit + matrix0.m_up, dVector (0, 1, 0));
		DebugDrawLine (matrix0.m_posit, matrix0.m_posit + matrix0.m_right, dVector (0, 0, 1));


		dMatrix bodyMatrix1;
		NewtonBodyGetMatrix (info.m_attachBody_1, &bodyMatrix1[0][0]);
		dMatrix matrix1 (*((dMatrix*) &info.m_attachmenMatrix_1[0]));
		matrix1 = matrix1 * bodyMatrix1;
		DebugDrawLine (matrix1.m_posit, matrix1.m_posit + matrix1.m_front, dVector (1, 0, 0));
		DebugDrawLine (matrix1.m_posit, matrix1.m_posit + matrix1.m_up,	   dVector (0, 1, 0));
		DebugDrawLine (matrix1.m_posit, matrix1.m_posit + matrix1.m_right, dVector (0, 0, 1));

	}

}


static void ShowMeshCollidingFaces (
	const NewtonBody* bodyWithTreeCollision, 
	const NewtonBody* body, 
	int faceID, 
	int vertexCount, 
	const dFloat* vertex, 
	int vertexstrideInBytes)
{

	// we are copieng data to and array of memory, anothe call back may be doing teh same thong
	// her fore we nee to avopid race coditions
	NewtonWorldCriticalSectionLock (NewtonBodyGetWorld (bodyWithTreeCollision));

	dVector face[64];
	int stride = vertexstrideInBytes / sizeof (dFloat);
	for (int j = 0; j < vertexCount; j ++) {
		face [j] = dVector (vertex[j * stride + 0], vertex[j * stride + 1] , vertex[j * stride + 2]);
	}
	DebugDrawPolygon (vertexCount, face);

	// unlock the critical section
	NewtonWorldCriticalSectionUnlock (NewtonBodyGetWorld (bodyWithTreeCollision));
}


void SetShowMeshCollision (SceneManager& me, int mode)
{
	NewtonTreeCollisionCallback showFaceCallback;

	showFaceCallback = NULL;
	if (mode) {
		showFaceCallback = ShowMeshCollidingFaces;
	}

	// iterate the world
	for (const NewtonBody* body = NewtonWorldGetFirstBody (me.m_world); body; body = NewtonWorldGetNextBody (me.m_world, body)) {
		NewtonCollision* collision;
		NewtonCollisionInfoRecord info;

		collision = NewtonBodyGetCollision (body);
		NewtonCollisionGetInfo (collision, &info);

		switch (info.m_collisionType) 
		{
			case SERIALIZE_ID_TREE:
			case SERIALIZE_ID_SCENE:
			case SERIALIZE_ID_USERMESH:
			case SERIALIZE_ID_HEIGHTFIELD:
			{
				NewtonStaticCollisionSetDebugCallback (collision, showFaceCallback);
				break;
			}

			default: 
				break;
		}
	}
}


void SetShowIslands (SceneManager& me, int mode)
{
	showIslans = mode;
}


static void CalcylateAABB (const NewtonBody* body, dVector& minP, dVector& maxP)
{
	NewtonCollision *collision;
	

	// show an exact AABB
	collision = NewtonBodyGetCollision (body) ;

	dMatrix matrix;
	NewtonBodyGetMatrix (body, &matrix[0][0]);

	for (int i = 0; i < 3; i ++) {
		dVector support;
		dVector dir (0.0f, 0.0f, 0.0f, 0.0f);
		dir[i] = 1.0f;

		dVector localDir (matrix.UnrotateVector (dir));
		NewtonCollisionSupportVertex (collision, &localDir[0], &support[0]);
		support = matrix.TransformVector (support);
		maxP[i] = support[i];  

		localDir = localDir.Scale (-1.0f);
		NewtonCollisionSupportVertex (collision, &localDir[0], &support[0]);
		support = matrix.TransformVector (support);
		minP[i] = support[i];  
	}
}

int PhysicsIslandUpdate (const void* islandHandle, int bodyCount)
{
	if (showIslans) {
		dVector minAABB ( 1.0e10f,  1.0e10f,  1.0e10f, 0.0f);
		dVector maxAABB (-1.0e10f, -1.0e10f, -1.0e10f, 0.0f);
		for (int i = 0; i < bodyCount; i ++) {
			dVector p0;
			dVector p1;

#if 0
			// show the engine loose aabb
			NewtonIslandGetBodyAABB (islandHandle, i, &p0[0], &p1[0]);
#else
			// show an exact AABB
			CalcylateAABB (NewtonIslandGetBody (islandHandle, i), p0, p1);
#endif
			for (int j = 0; j < 3; j ++ ) {
				minAABB[j] = p0[j] < minAABB[j] ? p0[j] : minAABB[j];
				maxAABB[j] = p1[j] > maxAABB[j] ? p1[j] : maxAABB[j];
			}
		}
		DebugDrawAABB (minAABB, maxAABB);
	}

	//	g_activeBodies += bodyCount;
	return 1;
}



NewtonBody* CreateGenericSolid (
	NewtonWorld* world,
	SceneManager* scene, 
	dFloat mass, 
	const dMatrix& srcMatrix, 
	const dVector& originalSize, 
	PrimitiveType type,
	int matrixlId)
{
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dFloat volume;
	NewtonBody* rigidBody;
	OGLMesh* meshInstance;
	RenderPrimitive* primitive;
	NewtonCollision* collision;
	dVector origin;
	dVector inertia;


	dMatrix matrix (srcMatrix);
	dVector size (originalSize);
	//size.m_y = 1;
	//size.m_x = 4;

	collision = NULL;
	meshInstance = NULL;


	#define STEPS_HULL 16
	#define SAMPLE_COUNT 5000
	dVector cloud [SAMPLE_COUNT];

	volume = 1.0f;
	switch (type) 
	{
		case _SPHERE_PRIMITIVE:
		{
			dFloat r = size.m_x * 0.5f;

			// create the collision 
			collision = NewtonCreateSphere (world, r, r, r, NULL); 

			// the simple representation of implicit shape is smaller than the real volume
			volume = 0.5f * NewtonConvexCollisionCalculateVolume (collision);
			break;
		}

		case _BOX_PRIMITIVE:
		{
			// create the collision 
			collision = NewtonCreateBox (world, size.m_x, size.m_y, size.m_z, NULL); 
			
			volume = 0.5f * NewtonConvexCollisionCalculateVolume (collision);
			break;
		}


		case _CONE_PRIMITIVE:
		{
			dFloat h = size.m_x;
			dFloat r = size.m_y;

			// create the collision 
			collision = NewtonCreateCone (world, r, h, NULL); 

			// the simple representation of implicit shape is smaller than the real volume
			volume = 0.5f * NewtonConvexCollisionCalculateVolume (collision);
			break;
		}

		case _CYLINDER_PRIMITIVE:
		{
			// create the collision 
			collision = NewtonCreateCylinder (world, size.m_y, size.m_x, NULL); 

			// the simple representation of implicit shape is smaller than the real volume
			volume = 0.5f * NewtonConvexCollisionCalculateVolume (collision);
			break;
		}


		case _CAPSULE_PRIMITIVE:
		{
			// create the collision 
			collision = NewtonCreateCapsule (world, size.m_y, size.m_x, NULL); 
/*
NewtonCollision* collision1;
collision1 = NewtonCreateCapsule (world, size.m_y, size.m_x, NULL); 
collision = NewtonCreateConvexHullModifier( world, collision1 );
float my_matrix[16] = { 1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1 };
NewtonConvexHullModifierSetMatrix( collision, my_matrix );
NewtonReleaseCollision (world, collision1);
*/

			// the simple representation of implicit shape is smaller than the real volume
			volume = 0.5f * NewtonConvexCollisionCalculateVolume (collision);
			break;
		}

		case _CHAMFER_CYLINDER_PRIMITIVE:
		{
			// create the collision 
			collision = NewtonCreateChamferCylinder (world, size.m_x, size.m_y, NULL); 

			// the simple representation of implicit shape is smaller than the real volume
			volume = 0.5f * NewtonConvexCollisionCalculateVolume (collision);
			break;
		}

		case _RANDOM_CONVEX_HULL_PRIMITIVE:
		{
			int i;			
			int count;
			// Create a clouds of random point around the origin

			// make sure that at least the top and bottom are present
			cloud [0] = dVector ( size.m_x * 0.5f, -0.3f,  0.0f);
			cloud [1] = dVector ( size.m_x * 0.5f,  0.3f,  0.3f);
			cloud [2] = dVector ( size.m_x * 0.5f,  0.3f, -0.3f);
			cloud [3] = dVector (-size.m_x * 0.5f, -0.3f,  0.0f);
			cloud [4] = dVector (-size.m_x * 0.5f,  0.3f,  0.3f);
			cloud [5] = dVector (-size.m_x * 0.5f,  0.3f, -0.3f);
			count = 5;

			// populate the cloud with pseudo Gaussian random points
			for (i = 6; i < SAMPLE_COUNT; i ++) {
				cloud [i].m_x = RandomVariable(size.m_x);
				cloud [i].m_y = RandomVariable(size.m_y * 2.0f);
				cloud [i].m_z = RandomVariable(size.m_z * 2.0f);
				count ++;
			}
			/*
			float scale = 0.04f; 
			for(i = 0; i < SAMPLE_COUNT; i++) 
			{ 
			cloud[i].m_x *= scale; 
			cloud[i].m_y *= scale; 
			cloud[i].m_z *= scale; 
			} 
			*/

			collision = NewtonCreateConvexHull (world, count, &cloud[0].m_x, sizeof (dVector), 0.01f, NULL); 

			// the simple representation of implicit shape is smaller than the real volume
			volume = 0.5f * NewtonConvexCollisionCalculateVolume (collision);
			break;
		}

		case _REGULAR_CONVEX_HULL_PRIMITIVE:
		{
			int i;			
			int count;
			// Create a clouds of random point around the origin

			dInt32 j;
			dFloat x;
			dFloat height;
			dFloat radius;

			count = 0;
			radius = size.m_y;
			height = size.m_x * 0.999f;
			x = - height * 0.5f;
			dMatrix rotation (dPitchMatrix(2.0f * 3.141592f / STEPS_HULL));
			for (i = 0; i < 4; i ++) {
				dFloat pad;
				pad = ((i == 1) || (i == 2)) * 0.25f * radius;
				dVector p (x, 0.0f, radius + pad);
				x += 0.3333f * height;
				dMatrix acc (GetIdentityMatrix());
				for (j = 0; j < STEPS_HULL; j ++) {
					cloud[count] = acc.RotateVector(p);
					acc = acc * rotation;
					count ++;
				}
			}

			collision = NewtonCreateConvexHull (world, count, &cloud[0].m_x, sizeof (dVector), 0.02f, NULL); 

			// the simple representation of implicit shape is smaller than the real volume
			volume = 0.5f * NewtonConvexCollisionCalculateVolume (collision);
			break;
		}

		default: _ASSERTE (0);
	}

#ifdef USE_TEST_SERIALIZATION
		FILE* file;
		char fullPathName[2048];

		// save the collision file
		GetWorkingFileName ("collisiontest.bin", fullPathName);

		file = fopen (fullPathName, "wb");
//		SerializeFile (file, MAGIC_NUMBER, strlen (MAGIC_NUMBER) + 1);
		NewtonCollisionSerialize (world, collision, SerializeFile, file);
		fclose (file);

		// load the collision file
		NewtonReleaseCollision (world, collision);

		file = fopen (fullPathName, "rb");

//		DeSerializeFile (file, magicNumber, strlen (MAGIC_NUMBER) + 1);

		collision = NewtonCreateCollisionFromSerialization (world, DeSerializeFile, file);

		fclose (file);
		NewtonCollisionInfoRecord collisionInfo;
		NewtonCollisionGetInfo (collision, &collisionInfo);

#endif



	meshInstance = new OGLMesh (collision, "wood_0.tga", "wood_0.tga", "wood_1.tga");

	_ASSERTE (collision);
	_ASSERTE (meshInstance);

	// create a visual geometry
	primitive = new RenderPrimitive (matrix, meshInstance);
	scene->AddModel (primitive);

	// create a visual effect for fracture
	primitive->CreateVisualEffect ();

	// release the instance
	meshInstance->Release();

	// save the collision volume;
	#define CONTROL_VOLUME 0.1f

	dFloat randomizeDnsdity = 1.0f + ((RandomVariable(0.4f) + RandomVariable(0.4f)) - 0.1f);
	primitive->m_density =  randomizeDnsdity * mass / volume;

	primitive->m_controlVolume = NewtonConvexCollisionCalculateVolume (collision) * (CONTROL_VOLUME * CONTROL_VOLUME * CONTROL_VOLUME);

	// calculate the moment of inertia and the relative center of mass of the solid
	NewtonConvexCollisionCalculateInertialMatrix (collision, &inertia[0], &origin[0]);	
	Ixx = mass * inertia[0];
	Iyy = mass * inertia[1];
	Izz = mass * inertia[2];

	//create the rigid body
	rigidBody = NewtonCreateBody (world, collision);

	// release the collision geometry when not need it
	NewtonReleaseCollision (world, collision);


	// set the correct center of gravity for this body
	NewtonBodySetCentreOfMass (rigidBody, &origin[0]);

	// set the mass matrix
	NewtonBodySetMassMatrix (rigidBody, mass, Ixx, Iyy, Izz);

	// activate 
	//	NewtonBodyCoriolisForcesMode (blockBoxBody, 1);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData (rigidBody, primitive);


	// assign the wood id
	NewtonBodySetMaterialGroupID (rigidBody, matrixlId);

//  set continue collision mode
//	NewtonBodySetContinuousCollisionMode (rigidBody, continueCollisionMode);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback (rigidBody, PhysicsBodyDestructor);

	// set the transform call back function
	NewtonBodySetTransformCallback (rigidBody, PhysicsSetTransform);

	// set the force and torque call back function
	NewtonBodySetForceAndTorqueCallback (rigidBody, PhysicsApplyGravityForce);


	// set the matrix for both the rigid body and the graphic body
	NewtonBodySetMatrix (rigidBody, &matrix[0][0]);
	PhysicsSetTransform (rigidBody, &matrix[0][0], 0);

//dVector xxx (0, -9.8f * mass, 0.0f, 0.0f);
//NewtonBodySetForce (rigidBody, &xxx[0]);

	// force the body to be active of inactive
	//	NewtonBodySetAutoSleep (rigidBody, sleepMode);

	return rigidBody;
}


void InitEyePoint (const dVector& dir, const dVector& origin)
{
	cameraDir = dir;
	cameraEyepoint = origin;
	
	rollAngle = dAsin (dir.m_y);
	yawAngle = dAtan2 (-dir.m_z, dir.m_x);
	dMatrix cameraDirMat (dRollMatrix(rollAngle) * dYawMatrix(yawAngle));
}

//	Keyboard handler. 
void Keyboard(SceneManager& me)
{
	NewtonWorld* world;

	world = me.m_world;

	dVector target (cameraEyepoint + cameraDir);
	SetCamera (cameraEyepoint, target);


	// read the mouse position and set the camera direction

	dMOUSE_POINT mouse1;
	dInt32 mouseLeftKey;
	static dMOUSE_POINT mouse0;

	GetCursorPos(mouse1);
	
	// this section control the camera object picking
	mouseLeftKey = dGetKeyState (KeyCode_L_BUTTON);
//	mouseLeftKey = dGetKeyState (KeyCode_L_BUTTON);

	if (!MousePick (world, mouse1, mouseLeftKey, 0.125f, 1.0f)) {
		// we are not in mouse pick mode, then we are in camera tracking mode
		if (mouseLeftKey) {
			// when click left mouse button the first time, we reset the camera
			// convert the mouse x position to delta yaw angle
			if (mouse1.x > (mouse0.x + 1)) {
				yawAngle += 1.0f * 3.141592f / 180.0f;
				if (yawAngle > (360.0f * 3.141592f / 180.0f)) {
					yawAngle -= (360.0f * 3.141592f / 180.0f);
				}
			} else if (mouse1.x < (mouse0.x - 1)) {
				yawAngle -= 1.0f * 3.141592f / 180.0f;
				if (yawAngle < 0.0f) {
					yawAngle += (360.0f * 3.141592f / 180.0f);
				}
			}

			if (mouse1.y > (mouse0.y + 1)) {
				rollAngle += 1.0f * 3.141592f / 180.0f;
				if (rollAngle > (80.0f * 3.141592f / 180.0f)) {
					rollAngle = 80.0f * 3.141592f / 180.0f;
				}
			} else if (mouse1.y < (mouse0.y - 1)) {
				rollAngle -= 1.0f * 3.141592f / 180.0f;
				if (rollAngle < -(80.0f * 3.141592f / 180.0f)) {
					rollAngle = -80.0f * 3.141592f / 180.0f;
				}
			}
			dMatrix cameraDirMat (dRollMatrix(rollAngle) * dYawMatrix(yawAngle));
			cameraDir = cameraDirMat.m_front;
		}
	}

	// save mouse position and left mouse key state for next frame
	mouse0 = mouse1;

	// camera control
	if (dGetKeyState (KeyCode_w)) {
		cameraEyepoint += cameraDir.Scale (CAMERA_SPEED / 60.0f);
	} else if (dGetKeyState (KeyCode_s)) {
		cameraEyepoint -= cameraDir.Scale (CAMERA_SPEED / 60.0f);
	}

	if (dGetKeyState (KeyCode_d)) {
		dVector up (0.0f, 1.0f, 0.0f);
		dVector right (cameraDir * up);
		cameraEyepoint += right.Scale (CAMERA_SPEED / 60.0f);
	} else if (dGetKeyState (KeyCode_a)) {
		dVector up (0.0f, 1.0f, 0.0f);
		dVector right (cameraDir * up);
		cameraEyepoint -= right.Scale (CAMERA_SPEED / 60.0f);
	}
} 

void AddBoxes(SceneManager* system, dFloat mass, const dVector& origin, const dVector& size, int xCount, int zCount, dFloat spacing, PrimitiveType type, int materialID)
{
	int i;
	int j;
	dFloat x;
	dFloat z;


	dMatrix matrix (GetIdentityMatrix());
	//	dVector origin (cameraEyepoint + cameraDir.Scale (80.0f));
	for (i = 0; i < xCount; i ++) {
		x = origin.m_x + (i - xCount / 2) * spacing;
		for (j = 0; j < zCount; j ++) {
			z = origin.m_z + (j - zCount / 2) * spacing;

			matrix.m_posit.m_x = x;
			matrix.m_posit.m_y = FindFloor (system->m_world, x, z) + 5.0f;
			matrix.m_posit.m_z = z;

			CreateGenericSolid (system->m_world, system, mass, matrix, size, type, materialID);
		}
	}
}

int OnAABBOverlap (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex)
{
	_ASSERTE (body0);
	_ASSERTE (body1);
	_ASSERTE (NewtonBodyGetUserData(body0));
	_ASSERTE (NewtonBodyGetUserData(body1));
	return 1;
}

// this callback is called for every contact between the two bodies
//int GenericContactProcess (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, dFloat timestep, int threadIndex)
void GenericContactProcess (const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
/*
	dFloat speed0;
	dFloat speed1;
	SpecialEffectStruct* currectEffect;

	// get the pointer to the special effect structure
	currectEffect = (SpecialEffectStruct *)NewtonMaterialGetMaterialPairUserData (material);

	// save the conatact information
	NewtonMaterialGetContactPositionAndNormal (material, &currectEffect->m_position.m_x, &currectEffect->m_normal.m_x);
	NewtonMaterialGetContactTangentDirections (material, &currectEffect->m_tangentDir0.m_x, &currectEffect->m_tangentDir1.m_x);


	// Get the maximum normal speed of this impact. this can be used for positioning collision sound
	speed0 = NewtonMaterialGetContactNormalSpeed (material);
	if (speed0 > currectEffect->m_contactMaxNormalSpeed) {
		// save the position of the contact (for 3d sound of particles effects)
		currectEffect->m_contactMaxNormalSpeed = speed0;
	}

	// get the maximum of the two sliding contact speed
	speed0 = NewtonMaterialGetContactTangentSpeed (material, 0);
	speed1 = NewtonMaterialGetContactTangentSpeed (material, 1);
	if (speed1 > speed0) {
		speed0 = speed1;
	}

	// Get the maximum tangent speed of this contact. this can be used for particles(sparks) of playing scratch sounds 
	if (speed0 > currectEffect->m_contactMaxTangentSpeed) {
		// save the position of the contact (for 3d sound of particles effects)
		currectEffect->m_contactMaxTangentSpeed = speed0;
	}
*/

//NewtonJointRecord info;
//NewtonJointGetInfo (contactJoint, &info);

	for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact)) {
		dFloat speed;
		dVector point;
		dVector normal;	
		dVector dir0;	
		dVector dir1;	
		NewtonMaterial* material;

		material = NewtonContactGetMaterial (contact);
		NewtonMaterialGetContactPositionAndNormal (material, &point.m_x, &normal.m_x);
		NewtonMaterialGetContactTangentDirections (material, &dir0.m_x, &dir1.m_x);
		speed = NewtonMaterialGetContactNormalSpeed(material);
		speed = NewtonMaterialGetContactNormalSpeed(material);
		// play sound base of the contact speed.
		//
	}

	// the application can implement some kind of contact debug here
	ShowContacts (contactJoint);
}