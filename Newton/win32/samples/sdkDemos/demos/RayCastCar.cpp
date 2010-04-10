//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// simple 4d vector class
//********************************************************************
#include <toolbox_stdafx.h>
#include "SkyBox.h"
#include "dBone.h"
#include "RenderPrimitive.h"
#include "CustomDGRayCastCar.h"
#include "dAnimationClip.h"
#include "dPoseGenerator.h"
#include "dBonesToPoseBinding.h"
#include "../OGLMesh.h"
#include "../SceneManager.h"
#include "../PhysicsUtils.h"
#include "../toolBox/MousePick.h"
#include "../toolBox/OpenGlUtil.h"
#include "../toolBox/DebugDisplay.h"
#include "../toolBox/LevelPrimitive.h"
#include "../toolBox/PlaneCollision.h"
#include "../toolBox/HeightFieldPrimitive.h"
#include "../toolBox/UserHeightFieldCollision.h"



dFloat cameraDist = 7.0f;
static class BasicCar* player;
static class BasicCar* player2;
static class BasicCar* player3;
static void CarCameraKeyboard (SceneManager& me);
static void NoPlayerAutoSleep (SceneManager& me, int mode);


#define MAX_STEER_ANGLE				(30.0f * 3.1416f / 180.0f)
#define MAX_STEER_ANGLE_RATE		(0.15f)
#define MAX_TORQUE					(200.0f)

#define JEEP_MASS					(1280.0f)
#define F1_MASS					    (800.0f)
#define JEEP_TIRE_MASS				(20.0f)
//#define JEEP_CENTRE_OF_MASS_OFFSET	(0.5f)
#define JEEP_CENTRE_OF_MASS_OFFSET	(0.6f)
#define JEEP_SUSPENSION_LENGTH		(0.2f)
#define JEEP_SUSPENSION_SPRING		(140.0f)
#define F1_SUSPENSION_SPRING		(250.0f)
#define JEEP_SUSPENSION_DAMPER		(5.0f)

#define JEEP_USE_CONVEX_CAST		1
//#define JEEP_USE_CONVEX_CAST		0


#define CUSTOM_VEHICLE_JOINT_ID		0xF4ED



static void SetDemoCallbacks (SceneManager& system)
{
	system.m_control = CarCameraKeyboard;
	system.m_autoSleep = NoPlayerAutoSleep;
	system.m_showIslands = SetShowIslands;
	system.m_showContacts = SetShowContacts; 
	system.m_setMeshCollision = SetShowMeshCollision;
}

static LevelPrimitive* LoadLevelAndSceneRoot (SceneManager& system, const char* levelName, int optimized)
{
	NewtonWorld* world;
	NewtonBody* floorBody;
	LevelPrimitive* level;



	world = system.m_world;
	// /////////////////////////////////////////////////////////////////////
	//
	// create the sky box,
	system.AddModel (new SkyBox ());

	// Load a level geometry
	level = new LevelPrimitive (levelName, world, optimized);
	system.AddModel(level);
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
	SetDemoCallbacks (system);

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-40.0f, 10.0f, 0.0f));

	return level;
}




struct CAR_CONFIG
{
	struct TIRE
	{
		const char* m_boneName;
		const char* m_animName;
	};
	const char* m_name;

	TIRE front_right;
	TIRE front_left;
	TIRE rear_right;
	TIRE rear_left;
};

static CAR_CONFIG formulaOne = 
{
	"f1.mdl",
	{"FR_tire", "f1_fr.ani"},
	{"FL_tire", "f1_fl.ani"},
	{"RR_tire", "f1_rr.ani"},
	{"RL_tire", "f1_rl.ani"},
};



static CAR_CONFIG jeep = 
{
	"jeep.mdl",
	{"FR_tire", NULL},
	{"FL_tire", NULL},
	{"RR_tire", NULL},
	{"RL_tire", NULL},
};

class BasicCar: public CustomDGRayCastCar
{
	class TireAnimation
	{
		public:
		TireAnimation ()
		{
			m_tire = NULL;
			m_animation = NULL;
		}

		~TireAnimation ()
		{
			if (m_animation) {
				m_animation->Release();
			}
		}

		dBone* m_tire;	
		dBonesToPoseBinding* m_animation;
	};

	public:
	static BasicCar* Create (const CAR_CONFIG& config, SceneManager* system, NewtonWorld* nWorld, const dMatrix& matrix, dInt32 materailID)
	{
		dFloat Ixx;
		dFloat Iyy;
		dFloat Izz;
		dFloat mass;
		dBone* carRoot;
		OGLModel* carModel;
		NewtonBody* rigidBody;
		NewtonCollision* chassisCollision;

		// open the level data
		char fullPathName[2048];
		GetWorkingFileName (config.m_name, fullPathName);

		// load the model in a temp scene node
		carModel = new OGLModel;

		// set the matrix
		carModel->SetMatrix(matrix);
		system->AddModel (carModel);

		// load the model
		OGLLoaderContext context;
		carModel->Load(fullPathName, context);

		// create a scene node to make a single body
		carRoot = carModel->FindBone("body");


		// make the collision shape
		OGLMesh* geo;
		geo = (OGLMesh*) carModel->FindMesh (carRoot->GetName());
		chassisCollision = NewtonCreateConvexHull(nWorld, geo->m_vertexCount, geo->m_vertex, 3 * sizeof (dFloat), 0.1f, &carRoot->m_localMatrix[0][0]); 
	
		//create the rigid body
		rigidBody = NewtonCreateBody (nWorld, chassisCollision);

		// save the pointer to the graphic object with the body.
		NewtonBodySetUserData (rigidBody, carModel);

		// set the material group id for vehicle
		//	NewtonBodySetMaterialGroupID (m_vehicleBody, vehicleID);
		NewtonBodySetMaterialGroupID (rigidBody, materailID);

		// set a destructor for this rigid body
		NewtonBodySetDestructorCallback (rigidBody, PhysicsBodyDestructor);

		// set the force and torque call back function
		NewtonBodySetForceAndTorqueCallback (rigidBody, PhysicsApplyGravityForce);

		// set the transform call back function
		NewtonBodySetTransformCallback (rigidBody, SetTransform);

		// set the vehicle local coordinate system 
		dMatrix chassisMatrix;
		chassisMatrix.m_front = dVector (1.0f, 0.0f, 0.0f, 0);				// this is the vehicle direction of travel
		chassisMatrix.m_up	  = dVector (0.0f, 1.0f, 0.0f, 0.0f);			// this is the downward vehicle direction
		chassisMatrix.m_right = chassisMatrix.m_front * chassisMatrix.m_up;	// this is in the side vehicle direction (the plane of the wheels)
		chassisMatrix.m_posit = dVector (0.0f, 0.0f, 0.0f, 1.0f);

		// convert the chassis local system to global space
		chassisMatrix = chassisMatrix * matrix;

		// set the matrix for both the rigid body and the graphic body
		NewtonBodySetMatrix (rigidBody, &chassisMatrix[0][0]);

		dVector origin(0, 0, 0, 1);
		dVector inertia(0, 0, 0, 1);

		// calculate the moment of inertia and the relative center of mass of the solid
		NewtonConvexCollisionCalculateInertialMatrix (chassisCollision, &inertia[0], &origin[0]);	

		if (strcmp(config.m_name,"jeep.mdl")==0) { 
		  mass = JEEP_MASS;
		} else {
          mass = F1_MASS;
		}
		Ixx = mass * inertia[0];
		Iyy = mass * inertia[1];
		Izz = mass * inertia[2];

		// Set the vehicle Center of mass
		// the rear spoilers race the center of mass by a lot for a race car
		// we need to lower some more for the geometrical value of the y axis
		origin.m_y *= JEEP_CENTRE_OF_MASS_OFFSET;

		NewtonBodySetCentreOfMass (rigidBody, &origin[0]);
		// set the mass matrix
		NewtonBodySetMassMatrix (rigidBody, mass, Ixx, Iyy, Izz);


		// release the collision 
		NewtonReleaseCollision (nWorld, chassisCollision);	

		// create a vehicle joint with 4 tires
		BasicCar* carJoint;
		carJoint = new BasicCar (chassisMatrix, rigidBody, 4); 
		carJoint->m_tireOffsetMatrix = carRoot->CalcGlobalMatrix();

		// get radio and width of the tire
		dFloat witdh;
		dFloat radius;
		carJoint->CalculateTireDimensions ("FR_tire", witdh, radius);

		// add all tire to car

		carJoint->AddTire (config, config.front_right.m_boneName, witdh, radius, config.front_right.m_animName); 
		carJoint->AddTire (config, config.front_left.m_boneName, witdh, radius, config.front_left.m_animName); 
		carJoint->AddTire (config, config.rear_right.m_boneName, witdh, radius, config.rear_right.m_animName); 
		carJoint->AddTire (config, config.rear_left.m_boneName, witdh, radius, config.rear_left.m_animName); 
		return carJoint;
	}

	void ApplySteering (dFloat value)
	{

		if (value > 0.0f) {
			m_steerAngle += MAX_STEER_ANGLE_RATE;
			if (m_steerAngle > MAX_STEER_ANGLE) {
				m_steerAngle = MAX_STEER_ANGLE;
			}
		} else if (value < 0.0f) {
			m_steerAngle -= MAX_STEER_ANGLE_RATE;
			if (m_steerAngle < -MAX_STEER_ANGLE) {
				m_steerAngle = -MAX_STEER_ANGLE;
			}
		} else {
			if (m_steerAngle > 0.0f) {
				m_steerAngle -= MAX_STEER_ANGLE_RATE;
				if (m_steerAngle < 0.0f) {
					m_steerAngle = 0.0f;
				}
			} else if (m_steerAngle < 0.0f) {
				m_steerAngle += MAX_STEER_ANGLE_RATE;
				if (m_steerAngle > 0.0f) {
					m_steerAngle = 0.0f;
				}
			} 
		}

		dFloat speed;
		speed = dAbs (GetSpeed())*0.1f;
		if (speed>1.0f) {
          speed = 1.0f;
		}
		dFloat value2 = value;
		if (value2 > 0.0f) {
			//value2 = -200.0f * (1.0f - 45 * speed / 60.0f);
			value2 = -(200.0f*speed) * (1.0f - 45.0f / 75.0f);
		} else if (value < 0.0f) {
			//value2 = 200.0f * (1.0f - 45 * speed / 60.0f);
			value2 = (200.0f*speed) * (1.0f - 45.0f / 75.0f);
		} else {
			value2 = 0.0f;
		}

		SetTireSteerAngle(0, m_steerAngle, value2);
		SetTireSteerAngle(1, m_steerAngle, value2);
//		if (m_isMonsterTruck) {
//			SetTireSteerAngle(2, -m_steerAngle);
//			SetTireSteerAngle(3, -m_steerAngle);
//		}
	}

	void ApplyBrake (dFloat value)
	{
		SetTireBrake(0, value * 5000.0f);
		SetTireBrake(1, value * 5000.0f);
	 	SetTireBrake(2, value * 5000.0f);
		SetTireBrake(3, value * 5000.0f);
	}

	void ApplyTorque (dFloat value)
	{
		dFloat speed;

		speed = dAbs (GetSpeed());
		if (value > 0.0f) {
			value = -MAX_TORQUE * (1.0f - speed / 200.0f);
		} else if (value < 0.0f) {
			value = MAX_TORQUE * (1.0f - speed / 200.0f);//0.5f * (1.0f - speed / 200.0f);
		} else {
			value = 0.0f;
		}

		SetTireTorque(2, value);
		SetTireTorque(3, value);
//		if (m_isMonsterTruck) {
//			SetTireTorque(0, value);
//			SetTireTorque(1, value);
//		}
	}



protected:
	BasicCar (const dMatrix& chassisMatrix, NewtonBody* carBody, int maxTiresCount)
		:CustomDGRayCastCar (maxTiresCount, 1, 0, 2, 2, chassisMatrix, carBody) 
	{
		m_steerAngle = 0.0f;
		// assign a type information to locate the joint in the callbacks
		SetJointID (CUSTOM_VEHICLE_JOINT_ID);
	}

	~BasicCar()
	{
		for (int i = 0; i < GetTiresCount(); i ++) {
			TireAnimation* tireAnim;
			const CustomDGRayCastCar::Tire& tire = GetTire (i);
			tireAnim = (TireAnimation*) tire.m_userData;
			delete tireAnim;
		}
	}

	//	virtual void SetTorque (dFloat torque);
	//	virtual void SetSteering (dFloat angle);

	private:
	void AddTire (const CAR_CONFIG& config, const char* boneName, dFloat width, dFloat radius, const char* animName)
	{
		OGLModel* carRoot;
		TireAnimation* tire;

		tire = new TireAnimation;

		carRoot = (OGLModel*) NewtonBodyGetUserData(GetBody0());
		tire->m_tire = carRoot->FindBone(boneName);

		// if teh tire have an anomation...
		if (animName) {
			char fullPathName[2048];
			GetWorkingFileName (animName, fullPathName);
			dBonesToPoseBinding* bind;
			bind = new dBonesToPoseBinding (fullPathName);

			bind->SetUpdateCallback (UdateSuspentionParts);


			// set the user data to the binding pose;
			for (dBonesToPoseBinding::dListNode* node = bind->GetFirst(); node; node = node->GetNext()) {
				const char* name;
				dBindFrameToNode& poseBind = node->GetInfo();
				name = poseBind.m_sourceTranform->m_source->m_bindName;
				poseBind.m_userData = tire->m_tire->GetParent()->Find(name);
			}

			tire->m_animation = bind;
		}

		// add this tire, get local position and rise it by the suspension length 
		dMatrix matrix (tire->m_tire->CalcGlobalMatrix());
		dVector tirePosition (matrix .m_posit);
		tirePosition += matrix .m_up.Scale (JEEP_SUSPENSION_LENGTH);
		if (strcmp(config.m_name,"jeep.mdl")==0) { 
          //printf("jeep");
		  AddSingleSuspensionTire (tire, tirePosition, JEEP_TIRE_MASS, radius, width, JEEP_SUSPENSION_LENGTH, JEEP_SUSPENSION_SPRING, JEEP_SUSPENSION_DAMPER, JEEP_USE_CONVEX_CAST);
		} else {
		  //printf("f1");	  
          AddSingleSuspensionTire (tire, tirePosition, JEEP_TIRE_MASS, radius, width, JEEP_SUSPENSION_LENGTH, F1_SUSPENSION_SPRING, JEEP_SUSPENSION_DAMPER, JEEP_USE_CONVEX_CAST);
		}
	}


	void CalculateTireDimensions (const char* tireName, dFloat& witdh, dFloat& radius) const
	{
		dVector extremePoint;
		NewtonWorld* world;
		//dSceneNode* carRoot;
		OGLModel* carModel;
		dBone* tirePart;
		OGLMesh* geo;
		NewtonCollision* collision;

		// find the the tire visual mesh 
		world = NewtonBodyGetWorld(GetBody0());
		carModel = (OGLModel*) NewtonBodyGetUserData(GetBody0());

		tirePart = carModel->FindBone (tireName);
		geo = (OGLMesh*) carModel->FindMesh(tireName);

		// make a convex hull collition shape to assist in calculation of the tire shape size
		collision = NewtonCreateConvexHull(world, geo->m_vertexCount, geo->m_vertex, 3 * sizeof (dFloat), 0.0f, NULL); 

		dMatrix tireMatrix (tirePart->CalcGlobalMatrix() * carModel->m_matrix);
//		dMatrix tireMatrix (tirePart->CalcGlobalMatrix());

		// fin the support points tha can be use to define the tire collision mesh
		dVector upDir (tireMatrix.UnrotateVector(dVector (0.0f, 1.0f, 0.0f, 0.0f)));
		NewtonCollisionSupportVertex (collision, &upDir[0], &extremePoint[0]);
		radius = upDir % extremePoint;

		dVector withdDir (tireMatrix.UnrotateVector(carModel->m_matrix.m_right));
		NewtonCollisionSupportVertex (collision, &withdDir[0], &extremePoint[0]);
		witdh = withdDir % extremePoint;

		withdDir = withdDir.Scale (-1.0f);
		NewtonCollisionSupportVertex (collision, &withdDir[0], &extremePoint[0]);
		witdh += withdDir % extremePoint;

		NewtonReleaseCollision (world, collision);	
	}

	static void UdateSuspentionParts (void* userData, dFloat* posit, dFloat* rotation)
	{
		dBone* node;
		node = (dBone*) userData;

		dVector p (posit[0], posit[1], posit[2], 1.0f);
		dQuaternion r (rotation[0], rotation[1], rotation[2], rotation[3]); 
		node->SetMatrix (dMatrix (r, p));
	}


	static void SetTransform  (const NewtonBody* body, const dFloat* matrix, int threadIndex)
	{
		BasicCar* car;
		NewtonJoint* joint;
		

		// set the transform of the main body
		PhysicsSetTransform (body, matrix, threadIndex);

		// find the car joint attached to the body
		car = NULL;
		for (joint = NewtonBodyGetFirstJoint(body); joint; joint = NewtonBodyGetNextJoint(body, joint)) {
			car = (BasicCar*) NewtonJointGetUserData(joint);
			if (car->GetJointID() == CUSTOM_VEHICLE_JOINT_ID) {
				break;
			}
		}


		if (car) {
			int count;
			// position all visual tires matrices


			const dMatrix& carMatrix = *((dMatrix*)matrix);
			dMatrix rootMatrixInv (car->m_tireOffsetMatrix * carMatrix) ;
			rootMatrixInv = rootMatrixInv.Inverse();
			count = car->GetTiresCount();
			for (int i = 0; i < count; i ++) {

				dBone* tireNode;
				TireAnimation* tireAnim;

				const CustomDGRayCastCar::Tire& tire = car->GetTire (i);

				tireAnim = (TireAnimation*) tire.m_userData;
				tireNode = tireAnim->m_tire;

				// if the tire has an animation update the animation matriices
				if (tireAnim->m_animation) {
					dFloat param;
					param = (1.0f - car->GetParametricPosition(i));
					
					if (param > 1.0f) {
						param = 1.0f;
					}
					if (param < 0.0f) {
						param = 0.0f;
					}

					tireAnim->m_animation->GeneratePose (param);
					tireAnim->m_animation->UpdatePose();
				}

				// calculate the tire local matrix
				dMatrix matrix (car->CalculateTireMatrix(i) * rootMatrixInv);
				tireNode->SetMatrix(matrix);

				if (DebugDisplayOn()) {
					dMatrix tireBaseMatrix (car->CalculateSuspensionMatrix(i, tire.m_posit) * car->GetChassisMatrixLocal() * carMatrix);
					DebugDrawCollision (tire.m_shape, tireBaseMatrix, dVector (1.0f, 1.0f, 0.0f, 1.0f));

//					tireBaseMatrix = car->CalculateSuspensionMatrix(i, 0.0) * carMatrix;
//					dVector span (tireBaseMatrix.TransformVector(dVector (0.0f, -tire.m_posit, 0.0f, 0.0f)));
//					dVector span (tireBaseMatrix.TransformVector(dVector (0.0f, -1.5f, 0.0f, 0.0f)));
//					DebugDrawLine (tireBaseMatrix.m_posit, span, dVector (1.0f, 0.0f, 0.0f, 1.0f));
					dVector span (tire.m_contactPoint + tire.m_contactNormal.Scale (1.0f));
					DebugDrawLine (tire.m_contactPoint, span, dVector (1.0f, 0.0f, 0.0f, 1.0f));
				}
			}
		}
	}



	dFloat m_steerAngle;
	dMatrix m_tireOffsetMatrix;
};


static void NoPlayerAutoSleep (SceneManager& me, int mode)
{
	// ste auto speep mode 
	 AutoSleep (me, mode);

	 // make sure teh player do no go to auto sleep node
	 if (player) {
		 NewtonBodySetAutoSleep (player->GetBody0(), 0);
	 }
}

static void CarCameraKeyboard (SceneManager& me)
{
	NewtonWorld* world;

	world = me.m_world;


	// read the mouse position and set the camera direction

	dMOUSE_POINT mouse1;
	dInt32 mouseLeftKey;
	static dMOUSE_POINT mouse0;

	GetCursorPos(mouse1);
	
	// this section control the camera object picking
	mouseLeftKey = dGetKeyState (KeyCode_L_BUTTON);

	if (!MousePick (world, mouse1, mouseLeftKey, 0.125f, 1.0f)) {
		// we are not in mouse pick mode, then we are in camera tracking mode
		if (mouseLeftKey) {
			// when click left mouse button the first time, we reset the camera
			// convert the mouse x position to delta yaw angle
			if (mouse1.x < (mouse0.x - 1)) {
				yawAngle += 1.0f * 3.1416f / 180.0f;
				if (yawAngle > (360.0f * 3.1416f / 180.0f)) {
					yawAngle -= (360.0f * 3.1416f / 180.0f);
				}
			} else if (mouse1.x > (mouse0.x + 1)) {
				yawAngle -= 1.0f * 3.1416f / 180.0f;
				if (yawAngle < 0.0f) {
					yawAngle += (360.0f * 3.1416f / 180.0f);
				}
			}

			if (mouse1.y < (mouse0.y - 1)) {
				rollAngle += 1.0f * 3.1416f / 180.0f;
				if (rollAngle > (80.0f * 3.1416f / 180.0f)) {
					rollAngle = 80.0f * 3.1416f / 180.0f;
				}
			} else if (mouse1.y > (mouse0.y + 1)) {
				rollAngle -= 1.0f * 3.1416f / 180.0f;
				if (rollAngle < -(80.0f * 3.1416f / 180.0f)) {
					rollAngle = -80.0f * 3.1416f / 180.0f;
				}
			}
		}
	}

	// save mouse position and left mouse key state for next frame
	mouse0 = mouse1;



	dMatrix target;
	NewtonBodyGetMatrix (player->GetBody0(), &target[0][0]);

	dQuaternion rot (dMatrix (dYawMatrix(yawAngle) * dRollMatrix(rollAngle) * target));
	static dQuaternion prevRot (rot);
	if (rot.DotProduct (prevRot) < 0.0f) {
		rot.Scale (-1.0f);
	}
	prevRot = prevRot.Slerp (rot, 0.15f);
	dMatrix cameraDirMat (prevRot, dVector (0.0f, 0.0f, 0.0f, 1.0f));
	
	cameraDir = cameraDirMat.m_front;

	dVector eyePoint (target.m_posit - cameraDir.Scale (cameraDist));
	SetCamera (eyePoint, target.m_posit);

	if (dGetKeyState (KeyCode_MINUS)) {
		cameraDist += (CAMERA_SPEED / 60.0f);
		if (cameraDist > 30.0f) {
			cameraDist = 30.0f;
		}
	} else if (dGetKeyState (KeyCode_PLUS)) {
		cameraDist -= (CAMERA_SPEED / 60.0f);
		if (cameraDist < 3.0f) {
			cameraDist = 3.0f;
		}
	}

	// hand brakes torque
	if (dGetKeyState (KeyCode_SPACE)) {
		player->ApplyBrake(1.0f);
	}

	if (dGetKeyState (KeyCode_w)) {
		player->ApplyTorque(1.0f);
	} else if (dGetKeyState (KeyCode_s)) {
		player->ApplyTorque(-1.0f);
	} else {
		player->ApplyTorque(0.0f);
	}

	if (dGetKeyState (KeyCode_d)) {
		player->ApplySteering (-1.0f);
	} else if (dGetKeyState (KeyCode_a)) {
		player->ApplySteering (1.0f);
	} else {
		player->ApplySteering (0.0f);
	}
} 



void RayCastCar (SceneManager& system)
{
	NewtonWorld* world;
	dBone* startPoint;
	LevelPrimitive* scene;

	world = system.m_world;

	// create the sky box and the floor,
//	scene = LoadLevelAndSceneRoot (system, "newtontrack.mdl", 1);
	scene = LoadLevelAndSceneRoot (system, "newtontrack.mdl", 1);

	// create a material 
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	// find the start point
	dMatrix start (GetIdentityMatrix());
	startPoint = scene->FindBone ("startPoint");
	if (startPoint) {
		start = startPoint->CalcGlobalMatrix() * scene->m_matrix;
	}

	start.m_posit.m_y = FindFloor (world, start.m_posit.m_x, start.m_posit.m_z) + 2.0f;
	dMatrix start2 = start;
    start2.m_posit.m_y += 5;
	dMatrix start3 = start2;
    start3.m_posit.m_y += 1;
	start3.m_posit.m_x += 5;
	InitEyePoint (start.m_front, start.m_posit - start.m_front.Scale (5.0f));

	yawAngle = 0.0f * 3.1416f/180.0f;
	rollAngle = -20.0f * 3.1416f/180.0f;

//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);

	BasicCar* car1;
	BasicCar* car2;
	BasicCar* car3;

//	car = BasicCar::Create (formulaOne, &system, system.m_world, start, defaultMaterialID);
//	player = car;

//	start.m_posit += dVector(0.0f, 0.0f, -3.0f, 0.0f);
	car1 = BasicCar::Create (jeep, &system, system.m_world, start, defaultMaterialID);
	player = car1;

	car2 = BasicCar::Create (jeep, &system, system.m_world, start2, defaultMaterialID);
    player2 = car2;

	car3 = BasicCar::Create (formulaOne, &system, system.m_world, start3, defaultMaterialID);
	player3 = car3;
//	start.m_posit += dVector(0.0f, 0.0f, 6.0f, 0.0f);
//	car = BasicCar::Create (jeep, &system, system.m_world, start, defaultMaterialID);
}


