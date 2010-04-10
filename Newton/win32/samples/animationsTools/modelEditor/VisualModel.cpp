#include "toolbox_stdafx.h"
#include "VisualModel.h"
#include <dMesh.h>
#include <dBone.h>
#include "OpenGlUtil.h"
#include <TargaToOpenGl.h>
#include <dPoseGenerator.h>
#include <dAnimationClip.h>

extern dFloat rollAngle;
extern dFloat yawAngle;
extern dVector cameraDir;
extern dVector cameraEyepoint;

#define CAM_DISTANCE_SCALE (3.0f)
#define CAM_ROLL_ANGLE  ( -20.0f * 3.1416f / 180.0f) 
#define CAM_PITCH_ANGLE (-135.0f * 3.1416f / 180.0f) 


VisualModel::VisualModel(const char* name)
{
	OGLLoaderContext context;
	Load (name, context);

	FrameCamera();
	m_player = NULL;
}


VisualModel::~VisualModel(void)
{
	if (m_player) {
		m_player->Release();
	}

	for (ModelComponentList<dList<dMesh*> >::dListNode* list = m_meshList.GetFirst(); list; list = list->GetNext()) {
		for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			for (dMesh::dListNode* subMeshNode = node->GetInfo()->GetFirst(); subMeshNode; subMeshNode = subMeshNode->GetNext()) {
				UnloadTexture (subMeshNode->GetInfo().m_textureHandle);
			}
		}
	}
}

void VisualModel::FrameCamera () const
{
	dMatrix pallete[256];

	UpdateMatrixPalette (m_matrix, pallete, sizeof (pallete) / sizeof (pallete[0]));

	dVector p0 (  1.0e10f,  1.0e10f,  1.0e10f, 0.0f);
	dVector p1 ( -1.0e10f, -1.0e10f, -1.0e10f, 0.0f);

	for (ModelComponentList<dList<dMesh*> >::dListNode* list = m_meshList.GetFirst(); list; list = list->GetNext()) {
		for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			const dMesh& mesh = *node->GetInfo();
			const dMatrix& matrix = pallete[mesh.m_boneID];

			for (int i = 0; i < mesh.m_vertexCount; i ++) {

				dVector p (matrix.TransformVector (dVector (mesh.m_vertex[i * 3 + 0], mesh.m_vertex[i * 3 + 1], mesh.m_vertex[i * 3 + 2], 1.0f)));

				if (p.m_x < p0.m_x) p0.m_x = p.m_x;
				if (p.m_y < p0.m_y) p0.m_y = p.m_y;
				if (p.m_z < p0.m_z) p0.m_z = p.m_z;

				if (p.m_x > p1.m_x) p1.m_x = p.m_x;
				if (p.m_y > p1.m_y) p1.m_y = p.m_y;
				if (p.m_z > p1.m_z) p1.m_z = p.m_z;
			}
		}
	}

	dFloat radius; 
	
	dVector size ((p1 - p0).Scale (0.5f));

	radius = CAM_DISTANCE_SCALE * dSqrt (size % size);
	rollAngle = CAM_ROLL_ANGLE; 
	yawAngle = CAM_PITCH_ANGLE; 

	dMatrix cameraDirMat (dRollMatrix(rollAngle) * dYawMatrix(yawAngle));
	cameraDir = cameraDirMat.m_front;

	dVector target ((p1 + p0).Scale (0.5f));
	cameraEyepoint = target - cameraDir.Scale (radius);
}


void VisualModel::UpdateBoneFormAnimation (void* userData, dFloat* posit, dFloat* rotation)
{
	dBone* node;
	node = (dBone*) userData;

	dVector p (posit[0], posit[1], posit[2], 1.0f);
	dQuaternion r (rotation[0], rotation[1], rotation[2], rotation[3]); 
	node->SetMatrix (dMatrix (r, p));
}


void VisualModel::SelectAnimation(int index)
{
	dTree<dAnimationClip*, int>::dTreeNode* node;

	node = m_animationsMenu.Find (index);
	_ASSERTE (node);

	if (m_player) {
		m_player->Release();
	}
	m_player = new dBonesToPoseBinding (node->GetInfo());

	m_player->SetUpdateCallback (UpdateBoneFormAnimation);

	// set the user data to the binding pose;
	for (dBonesToPoseBinding::dListNode* node = m_player->GetFirst(); node; node = node->GetNext()) {
		const char* name;
		dBindFrameToNode& poseBind = node->GetInfo();
		name = poseBind.m_sourceTranform->m_source->m_bindName;
		poseBind.m_userData = FindBone(name);
	}
}

void VisualModel::DeleteAnimation(int index)
{
	if (m_player) {	
		m_animationsMenu.Remove(index);
		RemoveAnimation(m_player->GetPoseGenerator()->GetAnimationClip());
		m_player->Release();
		m_player = NULL;
	}
}

void VisualModel::SetFrame (dFloat param)
{
	if (m_player) {
		m_player->GeneratePose (param);
		m_player->UpdatePose();
	}
}