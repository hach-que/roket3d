#include "toolbox_stdafx.h"
#include "OGLModel.h"
#include "OGLMesh.h"
#include "ShaderPrograms.h"
#include "TargaToOpenGl.h"

dMatrix OGLModel::m_matrixPalete[64];

OGLLoaderContext::OGLLoaderContext()
{
}

OGLLoaderContext::~OGLLoaderContext()
{
}



dMesh* OGLLoaderContext::CreateMesh (int type)
{
	OGLMesh* mesh;

	mesh = new OGLMesh (dMesh::dMeshType (type));
	if (type == dMesh::D_STATIC_MESH) {
		mesh->m_shaderProgram = ShaderPrograms::GetCache().m_diffuseEffect;
	} else {
		mesh->m_shaderProgram = ShaderPrograms::GetCache().m_skinningDiffuseEffect;
	}
	return mesh;
}

void OGLLoaderContext::LoaderFixup (dModel* model)
{
//	for (dModel::dMeshList::dListNode* node = model->m_meshList.GetFirst(); node; node = node->GetNext()) {
//		for (dMesh::dListNode* subMesh = node->GetInfo()->GetFirst(); subMesh; subMesh = subMesh->GetNext()) {
	for (ModelComponentList<dList<dMesh*> >::dListNode* list = model->m_meshList.GetFirst(); list; list = list->GetNext()) {
		for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			OGLMesh* oglMesh = (OGLMesh*) node->GetInfo();
			oglMesh->OptimizeForRender ();

			for (dMesh::dListNode* subMesh = oglMesh->GetFirst(); subMesh; subMesh = subMesh->GetNext()) {
				dSubMesh& segment = subMesh->GetInfo();
				segment.m_textureHandle = LoadTexture(segment.m_textureName);
			}
		}
	}
}


OGLModel::OGLModel(void)
	:m_matrix (GetIdentityMatrix())
{
}

OGLModel::~OGLModel(void)
{
}


void OGLModel::SetMatrix (const dMatrix& matrix)
{
	m_matrix = matrix;
}


void OGLModel::Render()
{
	if (m_skeleton.GetCount()) {
		UpdateMatrixPalette (m_matrix, m_matrixPalete, sizeof (m_matrixPalete) / sizeof (dMatrix));
		for (ModelComponentList<dList<dMesh*> >::dListNode* list = m_meshList.GetFirst(); list; list = list->GetNext()) {
			for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
				OGLMesh* mesh;
				mesh = (OGLMesh*)node->GetInfo();
				if (mesh->GetType() == dMesh::D_STATIC_MESH) {
					glPushMatrix();
					glMultMatrix(&m_matrixPalete[mesh->m_boneID][0][0]);
					mesh->Render (NULL);
					glPopMatrix();
				} else {
					mesh->Render (m_matrixPalete);
				}
			}
		}
	} else {
		glPushMatrix();
		glMultMatrix(&m_matrix[0][0]);
		for (ModelComponentList<dList<dMesh*> >::dListNode* list = m_meshList.GetFirst(); list; list = list->GetNext()) {
			for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
				OGLMesh* mesh;
				mesh = (OGLMesh*)node->GetInfo();
				mesh->Render (NULL);
			}
		}
		glPopMatrix();
	}
}
