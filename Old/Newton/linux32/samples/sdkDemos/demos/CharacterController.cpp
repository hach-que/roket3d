//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// simple 4d vector class
//********************************************************************

#include <toolbox_stdafx.h>
#include "RenderPrimitive.h"
#include "CustomUpVector.h"
#include "CustomPlayerController.h"
#include "dAnimationClip.h"
#include "dAnimationPose.h"
#include "dAnimationPoseBind.h"
#include "../NewtonDemos.h"
#include "../PhysicsUtils.h"
#include "../ToolBox/MousePick.h"
#include "../ToolBox/OpenGlUtil.h"
#include "../ToolBox/DebugDisplay.h"
#include "../ToolBox/LevelPrimitive.h"
#include "../ToolBox/PlaneCollision.h"
#include "../ToolBox/HeightFieldPrimitive.h"
#include "../ToolBox/UserHeightFieldCollision.h"


#define PLAYER_JOINT_ID 0xEF38AB01

class MyPlayeController: public CustomPlayerController
{
public:

	static MyPlayeController* Create (dSceneNode* parent, NewtonWorld* nWorld, int collisionShape, int materialID, const dVector& position, dVector& size)
	{
		dFloat heigh; 
		dFloat radius; 
		dFloat stepHeight; 
		NewtonBody* rigidBody;
		NewtonCollision* collision;

		heigh = size.m_y; 
		radius = size.m_x * 0.5f;
		stepHeight = size.m_y * 0.3f; 
		dMatrix alignmentMatrix (dRollMatrix(0.5f * 3.1416f));
		//collisionShape = 1;

		// create the main controller shape
		switch (collisionShape)
		{
			case 0:
			{
				// uses a capsule for collision
				collision = NewtonCreateCapsule(nWorld, radius, heigh, &alignmentMatrix[0][0]);
				break;
			}
			case 1:
			{
				// uses a ellipse for collision
				collision = NewtonCreateSphere(nWorld, heigh * 0.5f, radius, radius, &alignmentMatrix[0][0]);
				break;
			}
			default:
			{
				// uses a cylinder for collision
				collision = NewtonCreateCylinder(nWorld, radius, heigh, &alignmentMatrix[0][0]);
				break;
			}
		}

		//create the rigid body
		rigidBody = NewtonCreateBody (nWorld, collision);

		dFloat damp[3];
		damp[0] = 0.0f;
		damp[1] = 0.0f;
		damp[2] = 0.0f;
		// set the viscous damping the the minimum
		NewtonBodySetLinearDamping (rigidBody, 0.0f);
		NewtonBodySetAngularDamping (rigidBody, damp);

		// Set Material Id for this object
		NewtonBodySetMaterialGroupID (rigidBody, materialID);

		// save the pointer to the graphic object with the body.
		//NewtonBodySetUserData (rigidBody, visualRepresentation);

		// set a destructor for this rigid body
		NewtonBodySetDestructorCallback (rigidBody, PhysicsBodyDestructor);

		// set the transform call back function	NewtonBodySetTransformCallback (rigidBody, PhysicsSetTransform);
		NewtonBodySetTransformCallback (rigidBody, SetTransform);

		// set the force and torque call back function
		NewtonBodySetForceAndTorqueCallback (rigidBody, PhysicsApplyGravityForce);

		// force the player active permanently
		NewtonBodySetAutoSleep (rigidBody, 0);

		// set the mass matrix
		float mass;
		dVector center;
		dVector inertia;
		mass = 10.0f;
		NewtonConvexCollisionCalculateInertialMatrix (collision, &inertia[0], &center[0]);	
		NewtonBodySetMassMatrix (rigidBody, mass, mass * inertia.m_x, mass * inertia.m_y, mass * inertia.m_z);

		// set the matrix for both the rigid body and the graphic body
		dMatrix location (GetIdentityMatrix());
		location.m_posit = position;
		location.m_posit.m_w = 1.0f;
		NewtonBodySetMatrix (rigidBody, &location[0][0]);
		//		PhysicsSetTransform (rigidBody, &location[0][0], 0);

		// release the collision geometry when not need it
		NewtonReleaseCollision (nWorld, collision);

		// add and up vector constraint to help in keeping the body upright
		dVector upDirection (0.0f, 1.0f, 0.0f);
		//	return (MyPlayeController*) new CustomUpVector (&upDirection.m_x, rigidBody); 
		//	return new CustomPlayerController (&upDirection.m_x, rigidBody, 0.3f);
		return new MyPlayeController (&upDirection.m_x, rigidBody, 0.3f);
	}


	static CustomPlayerController* CreateBasicPlayer (dSceneNode* parent, NewtonWorld* nWorld, int collisionShape, int materialID, const dVector& position, dVector& size)
	{
		const NewtonBody* body;
		dGeometry* meshInstance;
		const NewtonCollision* shape;
		CustomPlayerController* controller;
		RenderPrimitive* visualRepresentation;


		controller = Create (parent, nWorld, collisionShape, materialID, position, size);

		// create the main controller shape
		body = controller->GetBody0();
		shape = NewtonBodyGetCollision(body);

		// create a simple visual representation using the collision shape
		//meshInstance = new dGeometry (shape, "earthmap.tga", "earthmap.tga", "earthmap.tga");
		meshInstance = new dGeometry (shape, "metal_30.tga", "metal_30.tga", "metal_30.tga");
		visualRepresentation = new RenderPrimitive (parent, GetIdentityMatrix(), meshInstance);
		meshInstance->Release();

		NewtonBodySetUserData (body, visualRepresentation);
		return controller;
	}


	static CustomPlayerController* CreateAnimatedPlayer (const char* meshName, dSceneNode* parent, NewtonWorld* world, int collisionShape, int materialID, const dVector& position)
	{
		dSceneNode* model;
		const NewtonBody* body;
		dLoaderContext context;
		MyPlayeController* controller;

		//const dGeometry* skinMesh;
		char fullPathName[2048];
		GetWorkingFileName (meshName, fullPathName);

		dMatrix matrix (GetIdentityMatrix());
		matrix.m_posit = cameraEyepoint;
		matrix.m_posit.m_x += 2.0f;
		matrix.m_posit.m_w = 1.0f;

		model = parent->LoadAsciiGraphicsFile(fullPathName, context);
		model->SetMatrix(matrix);

		// calculate the the model size, by calculation teh BBox of teh model
		dFloat pMin =  1.0e10f;
		dFloat pMax = -1.0e10f;
		for (dSceneNode* ptr = model->GetFirst(); ptr; ptr = ptr->GetNext()) {
			dGeometry* geomety;
			geomety = ptr->GetGeometry();
			if (geomety) {
				dVector p0;    
				dVector p1;    
				NewtonCollision* shape;
				dMatrix matrix (ptr->CalcGlobalMatrix(model));

				shape = NewtonCreateConvexHull (world, geomety->m_vertexCount, geomety->m_vertex, 3 * sizeof (dFloat), 1.0e-3f, NULL);

				dVector dirUp (matrix.UnrotateVector (dVector (0.0f, 1.0f, 0.0f, 0.0f))); 
				dVector dirDown (dirUp.Scale (-1.0f)); 
				NewtonCollisionSupportVertex (shape, &dirDown[0], &p0[0]);
				NewtonCollisionSupportVertex (shape, &dirUp[0],   &p1[0]);
				p0 = matrix.TransformVector(p0);
				p1 = matrix.TransformVector(p1);

				if (p1.m_y > pMax) {
					pMax = p1.m_y;
				}
				if (p0.m_y < pMin) {
					pMin = p0.m_y;
				}
				NewtonReleaseCollision (world, shape);
			}

			if (ptr == model) {
				break;
			} 
		}

		dFloat high = pMax - pMin;
		dVector size (high * 0.3f, high, high * 0.3f);
		controller = Create (parent, world, collisionShape, materialID, position, size);

		body = controller->GetBody0();
		NewtonBodySetUserData (body, model);


		controller->m_pivotOffset.m_y = -((pMax + pMin) * 0.5f);
		controller->t = 0.0f;


/*
		GetWorkingFileName ("gymnast_Action1_anim.xml", fullPathName);
		controller->m_animation = new dAnimationPoseBind (fullPathName);
		controller->m_animation->SetUpdateCallback (UdateBoneMatrix);
		// set the user data to the binding pose;
		for (dAnimationPoseBind::dListNode* node = controller->m_animation->GetFirst(); node; node = node->GetNext()) {
			const char* name;
			NewtonBindFrameToNode& poseBind = node->GetInfo();
			name = poseBind.m_sourceTranform->m_source->m_bindName;
			poseBind.m_userData = model->Find(name);
		}
*/
		return controller;
	}


	static void UdateBoneMatrix (void* userData, dFloat* posit, dFloat* rotation)
	{
		dSceneNode* node;
		node = (dSceneNode*) userData;

		dVector p (posit[0], posit[1], posit[2], 1.0f);
		dQuaternion r (rotation[0], rotation[1], rotation[2], rotation[3]); 
		node->SetMatrix (dMatrix (r, p));
	}


	MyPlayeController (const dVector& pin, NewtonBody* player, dFloat stairStepFactor)
		:CustomPlayerController(pin, player, stairStepFactor), m_pivotOffset(0.0f, 0.0f, 0.0f, 0.0f)
	{
		m_animation = NULL;
		SetJointID (PLAYER_JOINT_ID);
		NewtonBodySetTransformCallback (player, SetTransform);
	}

	~MyPlayeController()
	{
		if (m_animation) {
			m_animation->Release();
		}
	}

	// over load the player controller to apply some customizations
	virtual void SubmitConstrainst (dFloat timestep, int threadId)
	{
		if (m_isInJumpState) {
			// for example if the player is in the air we may want to apply some extra gravity to make fall faster

			dVector force;

			NewtonBodyGetForceAcc (GetBody0(), &force.m_x);

			// scale the down force
			force = force.Scale (2.0f);
			NewtonBodyAddForce (GetBody0(), &force.m_x);
		}

		//implement some debug display here
		CustomPlayerController::SubmitConstrainst (timestep, threadId);
	}



	static void SetTransform (const NewtonBody* body, const dFloat* matrix, int threadId)
	{

		//	MyPlayeController* myJoint;
		//	myJoint = NULL;
		dMatrix rootMatrix (*((dMatrix*) matrix));
		for (NewtonJoint* joint = NewtonBodyGetFirstJoint(body); joint; joint = NewtonBodyGetNextJoint(body, joint)) {
			MyPlayeController* player;
			player = (MyPlayeController*) NewtonJointGetUserData(joint);
			if (player->GetJointID() == PLAYER_JOINT_ID) {

				rootMatrix.m_posit += rootMatrix.RotateVector (player->m_pivotOffset);

				// upadte anumation
				//player->m_animation->GeneratePose (param);
				if (player->m_animation) {
					player->t += 1.0e-3f;
					if (player->t > 1.0f) {
						player->t = 0.0f;
					}
					player->m_animation->GeneratePose (player->t);
//					player->m_animation->UpdatePose();
				}
				break;
			}
		}

		PhysicsSetTransform (body, &rootMatrix[0][0], threadId);




		/*
		if (DebugDisplayOn()) {
		NewtonJoint* joint;
		MyPlayeController* player;
		for (joint = NewtonBodyGetFirstJoint(body); joint; joint = NewtonBodyGetNextJoint(body, joint)) {
		player = (MyPlayeController*) NewtonJointGetUserData(joint);
		if (player->GetJointID() == PLAYER_JOINT_ID) {
		dMatrix shapeMatrix (*((dMatrix*) matrix));
		DebugDrawCollision (player->GetDynamicsSensorShape (), shapeMatrix);

		shapeMatrix.m_posit.m_y += player->m_stairHeight * 0.5f;
		DebugDrawCollision (player->GetHorizontalSensorShape(), shapeMatrix);
		break;
		}
		}
		}
		*/
	}

	dVector m_pivotOffset;

	dFloat t;
	dAnimationPoseBind* m_animation;

};






static void SetDemoCallbacks (DemosSystem& system, dSceneNode* scene)
{
	system.m_control = Keyboard;
	system.m_autoSleep = AutoSleep;
	system.m_showIslands = SetShowIslands;
	system.m_showContacts = SetShowContacts; 
	system.m_setMeshCollision = SetShowMeshCollision;
	system.m_scene = scene;
}

static dSceneNode* LoadLevelAndSceneRoot (DemosSystem& system, const char* levelName, int optimized)
{
	NewtonWorld* world;
	NewtonBody* floorBody;
	dSceneNode* scene;

	world = system.m_world;

	// create the sky box,
	scene = new SkyBoxPrimitive (NULL);


	LevelPrimitive *level = new LevelPrimitive (scene, levelName, world, optimized);
	floorBody = level->m_level;


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
	//	NewtonMaterialSetSurfaceThickness(world, defaultID, defaultID, 0.0f);


	// set the island update callback
	NewtonSetIslandUpdateEvent (world, PhysicsIslandUpdate);


	// save th3 callback
	SetDemoCallbacks (system, scene);
	return scene;
}


#if 0
void PlayerController(DemosSystem& system)
{
	NewtonWorld* world;
	dSceneNode* scene;

	world = system.m_world;

	// create the sky box and the floor,
	//	scene = LoadLevelAndSceneRoot (system, "flatplane.xml", 1);
	//	scene = LoadLevelAndSceneRoot (system, "dungeon.xml", 1);
	//	scene = LoadLevelAndSceneRoot (system, "pitpool.xml", 1);
	//	scene = LoadLevelAndSceneRoot (system, "playground.xml", 1);
	scene = LoadLevelAndSceneRoot (system, "level2.xml", 1);

	// create a matrial carrier to to cou collision wit ethsi obejted
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (world, posit.m_x, posit.m_z) + 1.2f;

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);


	dVector location (cameraEyepoint);
	location.m_x += 2.0f;
	location.m_z -= 1.0f;

	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);

	CustomPlayerController* player;
	dVector size (0.75f, 2.0f, 0.75f, 0.0f);
	player = MyPlayeController::CreateBasicPlayer (system.m_scene, world, 0, defaultMaterialID, location, size); 
}


#else

void PlayerController(DemosSystem& system)
{
	NewtonWorld* world;
	dSceneNode* scene;

	world = system.m_world;

	// create the sky box and the floor,
	//	scene = LoadLevelAndSceneRoot (system, "flatplane.xml", 1);
	//	scene = LoadLevelAndSceneRoot (system, "dungeon.xml", 1);
	//	scene = LoadLevelAndSceneRoot (system, "pitpool.xml", 1);
	//	scene = LoadLevelAndSceneRoot (system, "playground.xml", 1);
	scene = LoadLevelAndSceneRoot (system, "level2.xml", 1);

	// create a matrial carrier to to cou collision wit ethsi obejted
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (world, posit.m_x, posit.m_z) + 1.2f;

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);


	dVector location (cameraEyepoint);
	location.m_x += 2.0f;
	location.m_z -= 1.0f;

	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
	//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);

	CustomPlayerController* player;
	player = MyPlayeController::CreateAnimatedPlayer ("gymnast.xml", system.m_scene, world, 0, defaultMaterialID, location); 
//    player = MyPlayeController::CreateAnimatedPlayer ("snowman.xml", system.m_scene, world, 0, defaultMaterialID, location); 
}


#endif
