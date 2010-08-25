//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// simple 4d vector class
//********************************************************************
#include <toolbox_stdafx.h>
#include "SkyBox.h"
//#include "Custom6DOF.h"
#include "CustomRagDoll.h"
#include "RenderPrimitive.h"
#include "dBone.h"
#include "../OGLMesh.h"
#include "../OGLModel.h"
#include "../SceneManager.h"
#include "../PhysicsUtils.h"
#include "../toolBox/MousePick.h"
#include "../toolBox/OpenGlUtil.h"
#include "../toolBox/DebugDisplay.h"



struct RAGGOLL_BONE_DEFINTION
{
	char m_boneName[32];
	char m_shapeType[32];
	dFloat m_mass;
	dFloat m_coneAngle;
	dFloat m_minTwistAngle;
	dFloat m_maxTwistAngle;

	dFloat m_pitch;
	dFloat m_yaw;
	dFloat m_roll;

	int m_collideWithNonImmidiateBodies;
};


static RAGGOLL_BONE_DEFINTION snowManDefinition[] =
{
	{"pelvis",		"sphere", 20.0f,  0.0f,  -0.0f,    0.0f, 0.0f,  0.0f,  0.0f, 1}, 
	{"torso",		"sphere",  8.0f, 30.0f,  -30.0f,  30.0f, 0.0f,  0.0f,  0.0f, 1}, 
	{"head",		"sphere",  5.0f, 30.0f, -30.0f,   30.0f, 0.0f,  0.0f,  0.0f, 1}, 
	{"rightArm",	"capsule", 5.0f, 80.0f,  -15.0f,  15.0f, 0.0f,  0.0f,  0.0f, 1}, 
	{"rightForeArm","capsule", 5.0f,  0.0f, -160.0f,   0.0f, 0.0f,  0.0f, 90.0f, 1}, 
	{"rightHand",	"box",     5.0f,  0.0f,  -30.0f,  30.0f, 0.0f,  90.0f, 0.0f, 1}, 

	{"leftArm",		"capsule", 5.0f, 80.0f,  -15.0f,  15.0f, 0.0f,  0.0f,  0.0f, 1}, 
	{"leftForeArm", "capsule", 5.0f,  0.0f,    0.0f, 160.0f, 0.0f,  0.0f, 90.0f, 1}, 
	{"leftHand",    "box",     5.0f,  0.0f,  -30.0f,  30.0f, 0.0f,  90.0f, 0.0f, 1}, 

	{"rightLeg",	"capsule",  8.0f, 80.0f,  -30.0f, 30.0f, 0.0f,  0.0f,  0.0f, 1}, 
	{"rightCalf",	"capsule",  5.0f,  0.0f, -150.0f,  0.0f, 0.0f, 90.0f,  0.0f, 1}, 
	{"rightFoot",	"box",	    2.0f,  0.0f,  -30.0f, 30.0f, 0.0f, 90.0f,  0.0f, 1}, 

	{"leftLeg",		"capsule",  8.0f, 80.0f,  -30.0f, 30.0f, 0.0f,  0.0f,  0.0f, 1}, 
	{"leftCalf",	"capsule",  5.0f,  0.0f, -150.0f,  0.0f, 0.0f, 90.0f,  0.0f, 1}, 
	{"leftFoot",	"box",	    2.0f,  0.0f,  -30.0f, 30.0f, 0.0f, 90.0f,  0.0f, 1}, 
};


static RAGGOLL_BONE_DEFINTION gymnastDefinition[] =
{
	{"PELVIS",	    "convex", 10.0f,  0.0f,  -0.0f,    0.0f, 0.0f,  0.0f,  0.0f, 1}, 

//	{"STOMACH",	    "convex",  8.0f, 30.0f,  -30.0f,  30.0f, 0.0f,  0.0f, 90.0f, 1}, 
	{"CHEST",	    "convex",  8.0f, 30.0f,  -30.0f,  30.0f, 0.0f,  0.0f, 90.0f, 0}, 
//	{"NECK",	    "convex",  8.0f, 30.0f,  -30.0f,  30.0f, 0.0f,  0.0f, 90.0f, 1}, 
	{"HEAD",	    "convex",  5.0f, 30.0f,  -30.0f,  30.0f, 0.0f,  0.0f, 90.0f, 1}, 

//	{"TIBULA_L",        "convex",  8.0f,	0.0f,  -15.0f, 15.0f, 0.0f,  0.0f, 0.0f, 0}, 
	{"ARM_L",	    "convex",  8.0f,	30.0f, -30.0f, 30.0f, 0.0f,  0.0f, 90.0f, 1}, 
	{"FOREARM_L",	"convex",  8.0f,	0.0f, -130.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1}, 

//	{"TIBULA_R",        "convex",  8.0f,	0.0f,  -15.0f, 15.0f, 0.0f,  0.0f, 0.0f, 0}, 
	{"ARM_R",	    "convex",  8.0f,	30.0f, -30.0f, 30.0f, 0.0f,  0.0f, 90.0f, 1}, 
	{"FOREARM_R",	"convex",  8.0f,	0.0f, -130.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1}, 

	{"FEMUR_R",	    "convex",  8.0f,  80.0f,  -30.0f, 30.0f, 0.0f,  0.0f, 90.0f, 1}, 
	{"CALF_R",	    "convex",	5.0f,  0.0f, -150.0f,   0.0f,  0.0f, 90.0f,  0.0f, 1}, 
	{"ANKLE_R",	    "convex",	2.0f,  0.0f,  -30.0f,	30.0f, 0.0f,  0.0f,  0.0f, 1}, 

	{"FEMUR_L",	    "convex",  8.0f,  80.0f,  -30.0f,	30.0f, 0.0f,   0.0f, 90.0f, 1}, 
	{"CALF_L",	    "convex",	5.0f,  0.0f, -150.0f,    0.0f,  0.0f,-90.0f,  0.0f, 1}, 
	{"ANKLE_L",	    "convex",	2.0f,  0.0f,  -30.0f,	30.0f, 0.0f,   0.0f,  0.0f, 1}, 
};



class RagDoll: public CustomRagDoll
{
	public:

	static RagDoll* Create (const char* name, int bonesCount, const RAGGOLL_BONE_DEFINTION* definition, SceneManager* system, NewtonWorld* nWorld, const dMatrix& matrix) 
	{
		OGLModel* model;
		const OGLMesh* skinMesh;
		char fullPathName[2048];
		
		GetWorkingFileName (name, fullPathName);

		model = new OGLModel;
		system->AddModel (model);

		OGLLoaderContext context;
		model->Load (fullPathName, context);

		// set all matrices from root bone to mesh root to identity
		dBone* rootBone = model->FindBone (definition[0].m_boneName);
		model->SetMatrix (rootBone->CalcGlobalMatrix() * matrix);
		for (dBone* node = rootBone; node; node = node->GetParent()) {
				node->SetMatrix(GetIdentityMatrix());
		}

		// find the skin mesh, if this is a skinned model
		skinMesh = NULL;
//		for (OGLModel::dMeshList::dListNode* tmp = model->m_meshList.GetFirst(); tmp; tmp = tmp->GetNext()) { 
		for (ModelComponentList<dList<dMesh*> >::dListNode* list = model->m_meshList.GetFirst(); list; list = list->GetNext()) {
			for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
				if (node->GetInfo()->GetType() == OGLMesh::D_SKIN_MESH) {
					skinMesh = (OGLMesh*) node->GetInfo();
					break;
				}
			}
		}


		// Create the Ragoll
		RagDoll *ragDoll;
		ragDoll = new RagDoll(model);

		int stackIndex;
		dBone* nodeStack[32];
		stackIndex = 1;
		nodeStack[0] = model->FindBone(0);
		while (stackIndex) {
			dBone* node;
			stackIndex --;
			node = nodeStack[stackIndex];

			const char* name = node->GetName();
			for (int i = 0; i < bonesCount; i ++) {
				if (!strcmp (definition[i].m_boneName, name)) {
					ragDoll->AddBody (nWorld, node, definition[i], skinMesh); 
					break;
				}
			}

			for (node = node->GetChild(); node; node = node->GetSibling()) {
				nodeStack[stackIndex] = node;
				stackIndex ++;
			}
		}

		// set the force callback to all bodies
		for (int i = 0; i < ragDoll->GetBoneCount(); i ++) {
			const NewtonBody* bone;
			bone = ragDoll->GetBone(i);
			NewtonBodySetForceAndTorqueCallback (bone, PhysicsApplyGravityForce);
		}
/*
//const NewtonBody* xxx;
//xxx = m_ragDoll->GetBone(0);
//dMatrix matrixx;
//NewtonBodyGetMatrix (xxx, &matrixx[0][0]);
//new Custom6DOF (matrixx, matrixx, xxx, NULL);
		return ragDoll;
*/
return NULL;
	}

protected:
	RagDoll(OGLModel* model)
		:CustomRagDoll ()
	{
		m_model = model;;
	}

	void GetDimentions(const dBone* bone, dVector& origin, dVector& size) const
	{	
		OGLMesh* mesh;
		mesh = (OGLMesh*) m_model->FindMesh (bone->GetName());

		dFloat* const array = mesh->m_vertex;
		dVector pmin( 1.0e20f,  1.0e20f,  1.0e20f, 0.0f);
		dVector pmax(-1.0e20f, -1.0e20f, -1.0e20f, 0.0f);
		for (int i = 0; i < mesh->m_vertexCount; i ++) {
			pmin.m_x = array[i * 3 + 0] < pmin.m_x ? array[i * 3 + 0] : pmin.m_x;
			pmin.m_y = array[i * 3 + 1] < pmin.m_y ? array[i * 3 + 1] : pmin.m_y;
			pmin.m_z = array[i * 3 + 2] < pmin.m_z ? array[i * 3 + 2] : pmin.m_z;
													 
			pmax.m_x = array[i * 3 + 0] > pmax.m_x ? array[i * 3 + 0] : pmax.m_x;
			pmax.m_y = array[i * 3 + 1] > pmax.m_y ? array[i * 3 + 1] : pmax.m_y;
			pmax.m_z = array[i * 3 + 2] > pmax.m_z ? array[i * 3 + 2] : pmax.m_z;
		}

		size = (pmax - pmin).Scale (0.5f);
		origin = (pmax + pmin).Scale (0.5f);
		origin.m_w = 1.0f;

	}

	NewtonCollision* MakeSphere(NewtonWorld* nWorld, const dBone* bone) const
	{
		dVector size;
		dVector origin;

		dMatrix matrix (GetIdentityMatrix());
		GetDimentions(bone, matrix.m_posit, size);

		return NewtonCreateSphere (nWorld, size.m_x, size.m_x, size.m_x, &matrix[0][0]);
	}

	NewtonCollision* MakeCapsule(NewtonWorld* nWorld, const dBone* bone) const
	{
		dVector size;
		dVector origin;

		dMatrix matrix (GetIdentityMatrix());
		GetDimentions(bone, matrix.m_posit, size);

		return NewtonCreateCapsule (nWorld, size.m_y, 2.0f * size.m_x, &matrix[0][0]);
	}

	NewtonCollision* MakeBox(NewtonWorld* nWorld, const dBone* bone) const
	{
		dVector size;
		dVector origin;

		dMatrix matrix (GetIdentityMatrix());
		GetDimentions(bone, matrix.m_posit, size);

		return NewtonCreateBox (nWorld, 2.0f * size.m_x, 2.0f * size.m_y, 2.0f * size.m_z, &matrix[0][0]);
	}


	NewtonCollision* MakeConvexHull(NewtonWorld* nWorld, const dBone* bone, const OGLMesh* skin) const
	{
		int boneIndex;	
		int vertexCount;
		dVector points[2048];

		vertexCount = 0;
		boneIndex = bone->GetBoneID();
		_ASSERTE (skin->GetType() == dMesh::D_SKIN_MESH);

		// go over the vertex array and find and collect all vertice weghted by thsi bone.
		for(int i = 0; i < skin->m_vertexCount; i ++) {
			const dVector& weight = skin->m_weighList->m_vertexWeight[i];	
			const dMesh::dWeightList::dBoneWeightIndex& indices = skin->m_weighList->m_boneWeightIndex[i];

			for (int j = 0 ;  (j < 4) && (weight[j] > 0.125f); j ++) {
				// if the vertex i wighetd by this bone consider it part of the collision if the weight is the largest
				if (indices.m_index[j] == boneIndex) {
					points[vertexCount].m_x = skin->m_vertex[i * 3 + 0];
					points[vertexCount].m_y = skin->m_vertex[i * 3 + 1];
					points[vertexCount].m_z = skin->m_vertex[i * 3 + 2];
					vertexCount ++;
					break;
				}
			}
		}

		// here we hav ethe vertex array the are part of the collision shape
		_ASSERTE (vertexCount);

//		const dMatrix& matrix = skin->GetBindPoseMatrix(boneIndex);
		const dMatrix& matrix = skin->m_weighList->m_bindingMatrices[boneIndex];
		matrix.TransformTriplex (&points[0].m_x, sizeof (dVector), &points[0].m_x, sizeof (dVector), vertexCount);
		return NewtonCreateConvexHull (nWorld, vertexCount, &points[0].m_x, sizeof (dVector), 1.0e-3f, NULL);
	}


	void AddBody (NewtonWorld* nWorld, const dBone* bone, const RAGGOLL_BONE_DEFINTION& definition, const OGLMesh* skinMesh) 
	{
		int boneIndex;
		int parentIndeIndex;
		NewtonCollision* shape;

		shape = NULL;
		if (!strcmp (definition.m_shapeType, "sphere")) {
			shape = MakeSphere (nWorld, bone);
		} else if (!strcmp (definition.m_shapeType, "capsule")) {
			shape = MakeCapsule(nWorld, bone);
		} else if (!strcmp (definition.m_shapeType, "box")) {
			shape = MakeBox (nWorld, bone);
		} else {
			shape = MakeConvexHull(nWorld, bone, skinMesh);
		}

		// calculate the bone matrix
		dMatrix rootMatrix (bone->CalcGlobalMatrix() * m_model->m_matrix);

		// find the index of the bone parent
		parentIndeIndex = -1;
		for (dBone* parentNode = bone->GetParent(); parentNode && (parentIndeIndex == -1); parentNode = parentNode->GetParent()) {
			for (int i = 0; i <  GetBoneCount(); i ++) {
				if (parentNode == NewtonBodyGetUserData (GetBone (i))) {
					parentIndeIndex = i;
					break;
				}
			}
		} 

		dMatrix pinAndPivot (dPitchMatrix (definition.m_pitch * 3.141592f / 180.0f) * dYawMatrix (definition.m_yaw * 3.141592f / 180.0f) * dRollMatrix (definition.m_roll * 3.141592f / 180.0f));
		pinAndPivot = pinAndPivot * rootMatrix;
		boneIndex = AddBone (nWorld, parentIndeIndex, (void*) bone, rootMatrix, definition.m_mass, pinAndPivot, shape);


		SetCollisionState (boneIndex, definition.m_collideWithNonImmidiateBodies);
		SetBoneConeLimits (boneIndex, definition.m_coneAngle * 3.141592f / 180.0f);
		SetBoneTwistLimits (boneIndex, definition.m_minTwistAngle * 3.141592f / 180.0f, definition.m_maxTwistAngle * 3.141592f / 180.0f);

		// set the offset matrix 
		NewtonReleaseCollision (nWorld, shape);
	}


	void ApplyBoneMatrix (int boneIndex, void* userData, const dMatrix& matrix) const
	{
		dBone* boneNode;
		dBone* parentBone;

		if (boneIndex == 0) {
			m_model->m_matrix = matrix;
		} else {

			boneNode = (dBone*) userData;
			// check if the parent of this bone is also a bone, body1 is the parentBone
			parentBone = (dBone*) NewtonBodyGetUserData (GetParentBone (boneIndex));
			if (boneIndex && (boneNode->GetParent() != parentBone)) {
				// this is not and immdeiate bone calculate the offset matrix
				dBone* parent;
				parent = boneNode->GetParent();
				dMatrix offset (parent->GetMatrix());
				for (parent = parent->GetParent(); parent != parentBone; parent = parent->GetParent()) {
					offset = offset * parent->GetMatrix();
				}

				dMatrix localMatrix (matrix * offset.Inverse());
				boneNode->SetMatrix (localMatrix);

			} else {
				boneNode->SetMatrix (matrix);
			}
		}

	}


	void SubmitConstrainst (dFloat timestep, int threadIndex)
	{
		CustomRagDoll::SubmitConstrainst (timestep, threadIndex);

		for (int i = 1; i < GetBoneCount(); i ++) {
			const NewtonCustomJoint* joint;
			joint = GetJoint(i);
			ShowJointInfo(joint);
		}
	}

	OGLModel* m_model;
};


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


void DescreteRagDoll (SceneManager& system)
{

	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);


	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (system.m_world, 0.0f, 0.0f) + 2.0f;
	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);


	int bonesCount;
	bonesCount = sizeof (snowManDefinition) / sizeof (snowManDefinition[0]);
	for (int x = 0; x < 3; x ++) {
		for (int z = 0; z < 3; z ++) {
			dVector point (cameraEyepoint + dVector (x * 3.0f + 5.0f, 0.0f, z * 3.0f, 1.0f));
			point.m_w = 1.0f;
			dMatrix matrix (GetIdentityMatrix());
			matrix.m_posit = point;
			matrix.m_posit.m_y = FindFloor (system.m_world, point.m_x, point.m_z) + 1.2f;

			RagDoll* ragdoll;
			ragdoll = RagDoll::Create ("snowman.mdl", bonesCount, snowManDefinition, &system, system.m_world, matrix);
		}
	}
}

void SkinRagDoll (SceneManager& system)
{
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (system.m_world, 0.0f, 0.0f) + 4.0f;
	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);

	int bonesCount;
	bonesCount = sizeof (gymnastDefinition) / sizeof (gymnastDefinition[0]);
	for (int x = 0; x < 3; x ++) {
		for (int z = 0; z < 3; z ++) {
			dVector point (cameraEyepoint + dVector (x * 3.0f + 5.0f, 0.0f, z * 3.0f, 1.0f));
			point.m_w = 1.0f;
			dMatrix matrix (GetIdentityMatrix());
			matrix.m_posit = point;
			matrix.m_posit.m_y = FindFloor (system.m_world, point.m_x, point.m_z) + 1.2f;
			RagDoll* ragdoll;
			ragdoll = RagDoll::Create ("gymnast.mdl", bonesCount, gymnastDefinition, &system, system.m_world, matrix);
		}
	}
}




