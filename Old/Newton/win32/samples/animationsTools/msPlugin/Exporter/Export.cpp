#include "stdafx.h"
#include "dTree.h"
#include "dMatrix.h"
#include "Export.h"
#include <dMesh.h>
#include <dBone.h>
#include <dModel.h>


#define SCALE 40.0f

struct SortVertex
{
	float m_x;
	float m_y;
	float m_z;
	float m_nx;
	float m_ny;
	float m_nz;
	float m_u;
	float m_v;
	float m_material;
	float weidhts[4];
	int	weidhtsIndex[4];
};


Exporter::Exporter ()
{
}

Exporter::~Exporter ()
{
}

int Exporter::GetType ()
{
	return cMsPlugIn::eTypeExport;
}

const char* Exporter::GetTitle ()
{
	return "Netwon Exporter...";
}


void ApplyGlobalRotation (dModel *model, const dMatrix& matrix)
{
	dMatrix rot (matrix);
	rot.m_posit = dVector (0.0f, 0.0f, 0.0f, 1.0f);  

	for (ModelComponentList<dList<dMesh*> >::dListNode* list = model->m_meshList.GetFirst(); list; list = list->GetNext()) {
		for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			dMesh* geom = node->GetInfo();
			matrix.TransformTriplex (geom->m_vertex, 3 * sizeof (dFloat), geom->m_vertex, 3 * sizeof (dFloat), geom->m_vertexCount);
			rot.TransformTriplex (geom->m_normal, 3 * sizeof (dFloat), geom->m_normal, 3 * sizeof (dFloat), geom->m_vertexCount);
		}
	}

	int stackIndex;
	dBone* stack[1024];

	stackIndex = 0;
	for (ModelComponentList<dList<dBone*> >::dListNode* list = model->m_skeleton.GetFirst(); list; list = list->GetNext()) {
		for (dList<dBone*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			stack[stackIndex] = node->GetInfo();
			stackIndex ++;
		}
	}

	while (stackIndex) {
		dBone* bone;
		stackIndex --;
		bone = stack[stackIndex];

		bone->SetMatrix (matrix.Inverse() * bone->GetMatrix() * matrix);
	
		for (bone = bone->GetChild(); bone; bone = bone->GetSibling()) {
			stack[stackIndex] = bone;
			stackIndex ++;
		}
	}
}





void Exporter::AddSkeleton (msModel* pModel, dModel* model)
{
	int boneCount;
	
	boneCount = 0;
	for (ModelComponentList<dList<dBone*> >::dListNode* list = model->m_skeleton.GetFirst(); list; list = list->GetNext()) {
		for (dList<dBone*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			for (dBone* bone = node->GetInfo()->GetFirst(); bone; bone = bone->GetNext()) {
				boneCount ++;
			}
		}
	}

	for (int i = 0; i < boneCount; i ++) {
		int boneIndex;
		msBone* pBone;
		dBone* srcBone;

		srcBone = model->FindBone(i);
		_ASSERTE (srcBone);

		boneIndex = msModel_AddBone (pModel);
		pBone = msModel_GetBoneAt (pModel, boneIndex);

		msBone_SetName (pBone, srcBone->GetName());
		if (srcBone->GetParent()) {
			msBone_SetParentName (pBone, srcBone->GetParent()->GetName());
		}

		msVec3 position, rotation;
		dMatrix matrix (srcBone->GetMatrix());

		position[0] = matrix.m_posit.m_x * SCALE;
		position[1] = matrix.m_posit.m_y * SCALE;
		position[2] = matrix.m_posit.m_z * SCALE;

		dVector angle (matrix.GetXYZ_EulerAngles());
		rotation[0] = angle.m_x; 
		rotation[1] = angle.m_y; 
		rotation[2] = angle.m_z; 

		msBone_SetPosition (pBone, position);
		msBone_SetRotation (pBone, rotation);
	}

}

void Exporter::ConvertMeshToSkins(dModel* model)
{
	int meshCount;

	int vertexCount;
	dMesh* meshes[256];

	meshCount = 0;
	vertexCount = 0;
	for (ModelComponentList<dList<dMesh*> >::dListNode* list = model->m_meshList.GetFirst(); list; list = list->GetNext()) {
		for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			if (node->GetInfo()->GetType() == dMesh::D_STATIC_MESH) {
				meshes[meshCount] = node->GetInfo();
				vertexCount += meshes[meshCount]->m_vertexCount;
				meshCount ++;
			}
		}
	}
	if (meshCount) {
		dMesh* skin;
		int vertexIndex;
		
		vertexIndex = 0;
		skin = new dMesh (dMesh::D_SKIN_MESH);

		strcpy (skin->m_name, model->FindBone(0)->GetName());

		skin->AllocVertexData (vertexCount);
		for (int i = 0; i < meshCount; i ++) {
			dMesh* mesh;
			dBone* bone;

			mesh = meshes[i];
			memcpy (&skin->m_vertex[vertexIndex * 3], mesh->m_vertex, 3 * mesh->m_vertexCount * sizeof (dFloat));
			memcpy (&skin->m_normal[vertexIndex * 3], mesh->m_normal, 3 * mesh->m_vertexCount * sizeof (dFloat));
			memcpy (&skin->m_uv[vertexIndex * 2], mesh->m_uv, 2 * mesh->m_vertexCount * sizeof (dFloat));

			bone = model->FindBone (mesh->m_boneID);
			dMatrix matrix (bone->CalcGlobalMatrix());

			matrix.TransformTriplex (&skin->m_vertex[vertexIndex * 3], 3 * sizeof (dFloat), &skin->m_vertex[vertexIndex * 3], 3 * sizeof (dFloat), mesh->m_vertexCount);

			matrix.m_posit = dVector (0.0f, 0.0f, 0.0f, 1.0f);
			matrix.TransformTriplex (&skin->m_normal[vertexIndex * 3], 3 * sizeof (dFloat), &skin->m_normal[vertexIndex * 3], 3 * sizeof (dFloat), mesh->m_vertexCount);

			for (int j = 0; j < mesh->m_vertexCount; j ++) {
				skin->m_weighList->m_vertexWeight[j + vertexIndex][0] = 1.0f;
				skin->m_weighList->m_boneWeightIndex[j + vertexIndex].m_index[0] = mesh->m_boneID;
			}

			for (dMesh::dListNode* node = mesh->GetFirst(); node; node = node = node->GetNext()) {
				dSubMesh& srcMesh = node->GetInfo();
				dSubMesh& subMesh = *skin->AddSubMesh();

				subMesh.m_ambient = srcMesh.m_ambient;
				subMesh.m_diffuse = srcMesh.m_diffuse;
				subMesh.m_specular = srcMesh.m_specular;
				subMesh.m_shiness = srcMesh.m_shiness;
				strcpy (subMesh.m_textureName, srcMesh.m_textureName);
				subMesh.AllocIndexData (srcMesh.m_indexCount);

				for (int j = 0; j < srcMesh.m_indexCount; j ++) {
					subMesh.m_indexes[j] = unsigned short (srcMesh.m_indexes[j] + vertexIndex); 
				}
			}

			vertexIndex += mesh->m_vertexCount;

			model->RemoveMesh (mesh);
		}

		model->AddMesh (skin);
		skin->Release();
	}
}


void Exporter::MergeEqualMaterials(dModel* model)
{

	for (ModelComponentList<dList<dMesh*> >::dListNode* list = model->m_meshList.GetFirst(); list; list = list->GetNext()) {
		for (dList<dMesh*>::dListNode* meshNode = list->GetInfo().m_data.GetFirst(); meshNode; meshNode = meshNode->GetNext()) { 
			dMesh* skin;

			skin = meshNode->GetInfo();
			for (dMesh::dListNode* node = skin->GetFirst(); node; node = node->GetNext()) {
				int indexCount = node->GetInfo().m_indexCount;
				for (dMesh::dListNode* node1 = node->GetNext(); node1; node1 = node1->GetNext()) {
					if (!strcmp (node->GetInfo().m_textureName, node1->GetInfo().m_textureName)) {
						dVector error (node->GetInfo().m_ambient - node1->GetInfo().m_ambient);
						if (error % error < 1.0e-4f) {
							dVector error (node->GetInfo().m_diffuse - node1->GetInfo().m_diffuse);
							if (error % error < 1.0e-4f) {
								dVector error (node->GetInfo().m_specular - node1->GetInfo().m_specular);
								if (error % error < 1.0e-4f) {
									indexCount += node1->GetInfo().m_indexCount;
									node1->GetInfo().m_textureHandle = 1;
								}
							}
						}
					}
				}

				if (indexCount > node->GetInfo().m_indexCount) {
					dMesh::dListNode* newNode;
					node->GetInfo().m_textureHandle = 1;
					
					newNode = skin->Addtop();
					dSubMesh& srcMesh = node->GetInfo();
					dSubMesh& subMesh = newNode->GetInfo();

					subMesh.m_ambient = srcMesh.m_ambient;
					subMesh.m_diffuse = srcMesh.m_diffuse;
					subMesh.m_specular = srcMesh.m_specular;
					subMesh.m_shiness = srcMesh.m_shiness;
					strcpy (subMesh.m_textureName, srcMesh.m_textureName);
					subMesh.AllocIndexData (indexCount);

					indexCount = 0;
					node = node->GetPrev();
					dMesh::dListNode* nextNode;
					for (dMesh::dListNode* node1 = node; node1; node1 = nextNode) {
						nextNode = node1->GetNext();
						if (node1->GetInfo().m_textureHandle == 1) {
							memcpy (&subMesh.m_indexes[indexCount], node1->GetInfo().m_indexes, sizeof (unsigned short) * node1->GetInfo().m_indexCount);
							indexCount += node1->GetInfo().m_indexCount;
							skin->Remove (node1);
						}
					}
				}
			}
		}
	}
}


BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    MessageBox(hwnd, "Hi!", "This is a message", 
                        MB_OK | MB_ICONEXCLAMATION);
                break;
                case IDCANCEL:
                    MessageBox(hwnd, "Bye!", "This is also a message", 
                        MB_OK | MB_ICONEXCLAMATION);
                break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}


int Exporter::Execute (msModel *pModel)
{
	if (!pModel)
		return -1;

	//
	// choose filename
	//
	OPENFILENAME ofn;
	memset (&ofn, 0, sizeof (OPENFILENAME));

	char szFile[MS_MAX_PATH];
	char szFileTitle[MS_MAX_PATH];
	char szDefExt[32] = "mdl";
	char szFilter[128] = "Newton Files (*.mdl)\0*.mdl\0\0";
	szFile[0] = '\0';
	szFileTitle[0] = '\0';

	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.lpstrDefExt = szDefExt;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MS_MAX_PATH;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = MS_MAX_PATH;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	ofn.lpstrTitle = "Export Netwon MDL model";

	if (!::GetSaveFileName (&ofn))
		return 0;

	dModel model;


	dBone* root;
	int boneCount;

	root = NULL;
	boneCount = msModel_GetBoneCount(pModel);
	for (int i = 0; i < boneCount; i ++) {
		dBone* bone;
		msBone* msBone;

		bone = NULL;
		msBone = msModel_GetBoneAt (pModel, i);

		char name[256];
		
		if (!root) {
			root = new dBone (NULL);
			bone = root;
		} else {
			dBone* parent;
			msBone_GetParentName (msBone, name, sizeof (name) - 1);
			parent = root->Find(name);
			bone = new dBone (parent);
		}

		msBone_GetName (msBone, name, sizeof (name) - 1);
		bone->SetNameID (name);
		bone->m_boneID = i;

		msVec3 position;
		msVec3 rotation;
		msBone_GetPosition (msBone, position);
		msBone_GetRotation (msBone, rotation);

		dMatrix matrix (dPitchMatrix (rotation[0]) * dYawMatrix (rotation[1]) * dRollMatrix (rotation[2]));
		matrix.m_posit = dVector (position[0] / SCALE, position[1] / SCALE, position[2] / SCALE, 1.0f);
		bone->SetMatrix (matrix);
	}

	int uniqueMaterials;
	int uniqueArray[256];
	int indirectMaterial[256];

	uniqueMaterials = 0;
	for (int i = 0; i < msModel_GetMaterialCount(pModel); i ++) {
		indirectMaterial[i] = i;
		uniqueArray[uniqueMaterials] = i;
		uniqueMaterials ++;

		msMaterial* material;
		material = msModel_GetMaterialAt (pModel, i);

		for (int j = 0; j < i; j ++) {
			msMaterial* material1;
			material1 = msModel_GetMaterialAt (pModel, j);

			if ((material1->Ambient[0] == material->Ambient[0]) && 
				(material1->Ambient[1] == material->Ambient[1]) && 
				(material1->Ambient[2] == material->Ambient[2]) && 
				(material1->Diffuse[0] == material->Diffuse[0]) && 
				(material1->Diffuse[0] == material->Diffuse[0]) && 
				(material1->Diffuse[1] == material->Diffuse[1]) && 
				(material1->Diffuse[2] == material->Diffuse[2]) && 
				(material1->Specular[0] == material->Specular[0]) && 
				(material1->Specular[1] == material->Specular[1]) && 
				(material1->Specular[2] == material->Specular[2])) {
				if (!strcmp (material1->szDiffuseTexture, material->szDiffuseTexture))  {
					indirectMaterial[i] = j;
					uniqueMaterials --;
					break;
				}
			}
		}
	}


	ModelComponent<dList<dBone*> >& boneList = model.m_skeleton.Append()->GetInfo();
	char skelName[256];
	dModel::GetFileName (szFile, skelName);
	char* ptr = strrchr (skelName, '.');
	sprintf (ptr, ".skl");
	strcpy (boneList.m_name, skelName);
	boneList.m_data.Append (root);

	int meshCount;
	meshCount = msModel_GetMeshCount(pModel);

	ModelComponent<dList<dMesh*> >& meshList = model.m_meshList.Append()->GetInfo();
	char meshName[256];
	dModel::GetFileName (szFile, meshName);
	ptr = strrchr (meshName, '.');
	sprintf (ptr, ".msh");
	strcpy (meshList.m_name, meshName);


	int trianglesCount;
	int *indices;
	SortVertex* vertex;

	trianglesCount = 0;
	indices = new int [65536 * 3];
	vertex = new SortVertex[65536 * 3];
	memset (vertex, 0, 65536 * 3 * sizeof (SortVertex));
	for (int i = 0; i < meshCount; i ++) {
		msMesh* srcMesh;
		srcMesh = msModel_GetMeshAt (pModel, i);
		
		int triangles;
		int materialIndex;
		triangles = msMesh_GetTriangleCount (srcMesh);
		materialIndex = indirectMaterial[msMesh_GetMaterialIndex (srcMesh)];
		for (int j = 0; j < triangles; j ++) {
			msTriangle* face;
			face = msMesh_GetTriangleAt (srcMesh, j);

			word faceIndices[3];
			word normalIndices[3];
			msTriangle_GetVertexIndices (face, faceIndices);
			msTriangle_GetNormalIndices (face, normalIndices);
			for (int k = 0; k < 3; k ++) {
				msVertex* p;    
				msVec3 normal;

				p = msMesh_GetVertexAt (srcMesh, faceIndices[k]);
				msMesh_GetVertexNormalAt (srcMesh, normalIndices[k], normal);

				vertex[(trianglesCount + j) * 3 + k].m_x = p->Vertex[0] / SCALE;
				vertex[(trianglesCount + j) * 3 + k].m_y = p->Vertex[1] / SCALE;
				vertex[(trianglesCount + j) * 3 + k].m_z = p->Vertex[2] / SCALE;
				vertex[(trianglesCount + j) * 3 + k].m_nx = normal[0];
				vertex[(trianglesCount + j) * 3 + k].m_ny = normal[1];
				vertex[(trianglesCount + j) * 3 + k].m_nz = normal[2];
				vertex[(trianglesCount + j) * 3 + k].m_u = p->u;
				vertex[(trianglesCount + j) * 3 + k].m_v = 1.0f - p->v;
				vertex[(trianglesCount + j) * 3 + k].m_material = float (materialIndex);
				vertex[(trianglesCount + j) * 3 + k].weidhts[0] = 1.0f;
				vertex[(trianglesCount + j) * 3 + k].weidhtsIndex[0] = p->nBoneIndex;
			}
		}

		trianglesCount += triangles;
	}

	dMesh* mesh;
	mesh = new dMesh (dMesh::D_SKIN_MESH);
	sprintf (mesh->m_name, "%s", root->GetName());

	mesh->m_boneID = 0;
	meshList.m_data.Append (mesh);

	int vertexCount;
	vertexCount = dModel::PackVertexArray (&vertex[0].m_x, sizeof (SortVertex) / sizeof (float), sizeof (SortVertex), trianglesCount * 3, indices);
	mesh->AllocVertexData (vertexCount);
	for (int i = 0; i < vertexCount; i ++) {
		mesh->m_vertex[i * 3 + 0] = vertex[i].m_x;
		mesh->m_vertex[i * 3 + 1] = vertex[i].m_y;
		mesh->m_vertex[i * 3 + 2] = vertex[i].m_z;

		mesh->m_normal[i * 3 + 0] = vertex[i].m_nx;
		mesh->m_normal[i * 3 + 1] = vertex[i].m_ny;
		mesh->m_normal[i * 3 + 2] = vertex[i].m_nz;

		mesh->m_uv[i * 2 + 0] = vertex[i].m_u;
		mesh->m_uv[i * 2 + 1] = vertex[i].m_v;

		mesh->m_weighList->m_boneWeightIndex[i].m_index[0] = vertex[i].weidhtsIndex[0];
		mesh->m_weighList->m_vertexWeight[i][0] = vertex[i].weidhts[0];
	}

	for (int i = 0; i < uniqueMaterials; i ++) {
		int faceCount;
		faceCount = 0;

		for (int j = 0; j < trianglesCount; j ++) {
			_ASSERTE (vertex[indices[j * 3 + 0]].m_material == vertex[indices[j * 3 + 1]].m_material);
			_ASSERTE (vertex[indices[j * 3 + 0]].m_material == vertex[indices[j * 3 + 2]].m_material);
			if (int (vertex[indices[j * 3 + 0]].m_material) == uniqueArray[i]) {
				faceCount ++;
			}
		}

		int base;
		dSubMesh* subMesh;
		subMesh = mesh->AddSubMesh();

		base = 0;
		subMesh->AllocIndexData (faceCount * 3);

		msMaterial* material;
		material = msModel_GetMaterialAt (pModel, uniqueArray[i]);
		subMesh->m_ambient = dVector (material->Ambient[0], material->Ambient[1], material->Ambient[2], 1.0f);
		subMesh->m_diffuse = dVector (material->Diffuse[0], material->Diffuse[1], material->Diffuse[2], 1.0f);
		subMesh->m_specular = dVector (material->Specular[0], material->Specular[1], material->Specular[2], 1.0f);
		subMesh->m_shiness = material->fShininess;

		subMesh->m_textureName[0] = 0;
		if (material->szDiffuseTexture[0]) {
			strcpy (subMesh->m_textureName, material->szDiffuseTexture);
		}

		for (int j = 0; j < trianglesCount; j ++) {
			if (int (vertex[indices[j * 3 + 0]].m_material) == uniqueArray[i]) {
				subMesh->m_indexes[base * 3 + 0] =  indices[j * 3 + 0];
				subMesh->m_indexes[base * 3 + 1] =  indices[j * 3 + 1];
				subMesh->m_indexes[base * 3 + 2] =  indices[j * 3 + 2];
				base ++;
			}
		}
	}

	delete[] vertex;
	delete[] indices;

	dMatrix matrix (dYawMatrix (3.1416f * 0.5f));
	ApplyGlobalRotation (&model, matrix);
	model.Save (szFile, true, true, false);

	return 0;
}
