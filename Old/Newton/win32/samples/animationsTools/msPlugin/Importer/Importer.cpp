#include "stdafx.h"
#include "dTree.h"
#include "dMatrix.h"
#include "Importer.h"
#include <dMesh.h>
#include <dBone.h>
#include <dModel.h>


#define SCALE 40.0f

Importer::Importer ()
{
}

Importer::~Importer ()
{
}

int Importer::GetType ()
{
	return cMsPlugIn::eTypeImport;
}

const char* Importer::GetTitle ()
{
	return "Netwon Importer...";
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


struct SortVertex
{
	
	float m_x;
	float m_y;
	float m_z;
	float m_u;
	float m_v;
	float weidhts[4];
	int	weidhtsIndex[4];
};



void Importer::AddSkeleton (msModel* pModel, dModel* model)
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


void Importer::ConvertMeshToSkins(dModel* model)
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


void Importer::MergeEqualMaterials(dModel* model)
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


int Importer::Execute (msModel *pModel)
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
	ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrTitle = "Newton Import MDL model";

	if (!::GetOpenFileName (&ofn))
		return 0;

	dModel* model;	
	dLoaderContext context;
	model = new dModel ();
	model->Load (szFile, context);

	dMatrix matrix (dYawMatrix (-3.1416f * 0.5f));
	ApplyGlobalRotation (model, matrix);

	ConvertMeshToSkins(model);
	MergeEqualMaterials(model);

	// create all unique materials
	int mat_name_ID;
	mat_name_ID = 0;
	dTree<int, int> uniqueMaterials;

	for (ModelComponentList<dList<dMesh*> >::dListNode* list = model->m_meshList.GetFirst(); list; list = list->GetNext()) {
		for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			for (dMesh::dListNode* segNode = node->GetInfo()->GetFirst(); segNode; segNode = segNode->GetNext()) {

				dSubMesh& segment = segNode->GetInfo();
				
				int CRC;
				int matIndex;
				char name[256];
				sprintf (name, "%s_%d", node->GetInfo()->m_name, mat_name_ID);
				CRC = dCRC (name);
				mat_name_ID ++;

				msMaterial* mat;
				matIndex = msModel_AddMaterial (pModel);
				uniqueMaterials.Insert (matIndex, CRC);
				mat = msModel_GetMaterialAt (pModel, matIndex);

				msMaterial_SetName (mat, name);

				float scale = 1.0f;
				msVec4 diffuse;
				diffuse[0] = segment.m_diffuse.m_x * scale;
				diffuse[1] = segment.m_diffuse.m_y * scale;
				diffuse[2] = segment.m_diffuse.m_z * scale;
				diffuse[3] = 0.0f;
				msMaterial_SetDiffuse (mat, diffuse);

				msVec4 ambient;
				ambient[0] = segment.m_ambient.m_x * scale;
				ambient[1] = segment.m_ambient.m_y * scale;
				ambient[2] = segment.m_ambient.m_z * scale;
				ambient[3] = 0.0f;
				msMaterial_SetAmbient (mat, ambient);


				msVec4 specular;
				specular[0] = segment.m_specular.m_x * scale;
				specular[1] = segment.m_specular.m_y * scale;
				specular[2] = segment.m_specular.m_z * scale;
				specular[3] = 0.0f;
				msMaterial_SetSpecular (mat, specular);
				msMaterial_SetShininess (mat, segment.m_shiness);
//				msMaterial_SetShininess (mat, 60);

				if (segment.m_textureName[0]) {
					msMaterial_SetDiffuseTexture (mat, segment.m_textureName);
				}
			}
		}
	}

	mat_name_ID = 0;
	for (ModelComponentList<dList<dMesh*> >::dListNode* list = model->m_meshList.GetFirst(); list; list = list->GetNext()) {
		for (dList<dMesh*>::dListNode* node = list->GetInfo().m_data.GetFirst(); node; node = node->GetNext()) { 
			dFloat vMoodule;
			vMoodule = 0;

			dMesh* geo = node->GetInfo();
			for (dMesh::dListNode* segNode = geo->GetFirst(); segNode; segNode = segNode->GetNext()) {

				int *postIndex;		
				int *normalIndex;		
				dVector* normal;
				SortVertex* sortPosit;
				
				dSubMesh& segment = segNode->GetInfo();

				normalIndex = new int[segment.m_indexCount];
				postIndex = new int[segment.m_indexCount];
				sortPosit = new SortVertex[segment.m_indexCount];
				normal = new dVector[segment.m_indexCount];

				for (int i = 0; i < segment.m_indexCount; i ++) {
					int index;
					index = segment.m_indexes[i];

					memset (&sortPosit[i], 0, sizeof (SortVertex));
					sortPosit[i].m_x = geo->m_vertex[index * 3 + 0];
					sortPosit[i].m_y = geo->m_vertex[index * 3 + 1];
					sortPosit[i].m_z = geo->m_vertex[index * 3 + 2];
					sortPosit[i].m_u = geo->m_uv[index * 2 + 0];
					sortPosit[i].m_v = geo->m_uv[index * 2 + 1];

					normal[i].m_x = geo->m_normal[index * 3 + 0];
					normal[i].m_y = geo->m_normal[index * 3 + 1];
					normal[i].m_z = geo->m_normal[index * 3 + 2];

					sortPosit[i].weidhts[0] = 1.0f;
					sortPosit[i].weidhtsIndex[0] = geo->m_boneID;
					if (geo->m_weighList) {
						for (int j = 0; j < 4; j ++) {
							sortPosit[i].weidhts[j] = geo->m_weighList->m_vertexWeight[index][j];
							sortPosit[i].weidhtsIndex[j] = geo->m_weighList->m_boneWeightIndex[index].m_index[j];
						}
					}

					vMoodule = (dAbs (sortPosit[i].m_v) > vMoodule) ? dAbs (sortPosit[i].m_v) : vMoodule;  
				}

				if (vMoodule == dFloor (vMoodule)) {
					vMoodule = dFloor (vMoodule);
				} else {
					vMoodule = dFloor (vMoodule) + 1.0f;
				}

				int vCount;
				int nCount;
				vCount = dModel::PackVertexArray (&sortPosit[0].m_x, 5, sizeof (SortVertex), segment.m_indexCount, postIndex);
				nCount = dModel::PackVertexArray (&normal[0].m_x, 3, sizeof (dVector), segment.m_indexCount, normalIndex);

				msMesh *pMesh;
				pMesh = msModel_GetMeshAt (pModel, msModel_AddMesh (pModel));
				msMesh_SetName (pMesh, geo->m_name);

				char name[256];
				sprintf (name, "%s_%d", node->GetInfo()->m_name, mat_name_ID);
				mat_name_ID ++;

				dTree<int, int>::dTreeNode* matNode;
				matNode = uniqueMaterials.Find (dCRC (name));
				if (matNode) {
					msMesh_SetMaterialIndex (pMesh, matNode->GetInfo());
				}

				for (int i = 0; i < vCount; i ++) {
					int index;
					msVec2 uv;
					msVertex vertex;
					msVertex *pVertex;

					index = msMesh_AddVertex (pMesh);
					pVertex = msMesh_GetVertexAt (pMesh, index);

					vertex.Vertex[0] = sortPosit[i].m_x * SCALE;
					vertex.Vertex[1] = sortPosit[i].m_y * SCALE;
					vertex.Vertex[2] = sortPosit[i].m_z * SCALE;
					msVertex_SetVertex (pVertex, vertex.Vertex);

					uv[0] = sortPosit[i].m_u;
					uv[1] = vMoodule - sortPosit[i].m_v;
					msVertex_SetTexCoords (pVertex, uv);

					int boneIndex0 = sortPosit[i].weidhtsIndex[0];
					int boneIndex1 = sortPosit[i].weidhtsIndex[1];
					int boneIndex2 = sortPosit[i].weidhtsIndex[2];
					int boneIndex3 = sortPosit[i].weidhtsIndex[3];

					int weight0 = int (sortPosit[i].weidhts[0] * 100);
					int weight1 = int (sortPosit[i].weidhts[1] * 100);
					int weight2 = int (sortPosit[i].weidhts[2] * 100);
					int weight3 = int (sortPosit[i].weidhts[3] * 100);

					msVertex_SetBoneIndex (pVertex, boneIndex0);


					if ((weight0 > 0) && (weight0 < 100)){
						msVertexEx*  extraVertex;
						extraVertex = msMesh_GetVertexExAt (pMesh, index);

						msVertexEx_SetBoneWeights (extraVertex, 0, weight0);

						msVertexEx_SetBoneIndices (extraVertex, 0, boneIndex1);
						if (weight1 > 0) {
							msVertexEx_SetBoneWeights (extraVertex, 1, weight1);

							msVertexEx_SetBoneIndices (extraVertex, 1, boneIndex2);
							if (weight2 > 0) {
								msVertexEx_SetBoneWeights (extraVertex, 2, weight2);

								msVertexEx_SetBoneIndices (extraVertex, 2, boneIndex3);
							}
						}
					}
				}

				for (int i = 0; i < nCount; i ++) {
					int index;
					msVec3 vertex;
					
					vertex[0] = normal[i].m_x;
					vertex[1] = normal[i].m_y;
					vertex[2] = normal[i].m_z;

					vertex[0] = 1;
					vertex[1] = 0;
					vertex[2] = 0;

					index = msMesh_AddVertexNormal (pMesh);
					msMesh_SetVertexNormalAt (pMesh, index, vertex);
				}

				for (int i = 0; i < segment.m_indexCount; i += 3) {
					word nIndices[3];
					msTriangle *pTriangle;
					pTriangle = msMesh_GetTriangleAt (pMesh, msMesh_AddTriangle (pMesh));

					nIndices[0] = word (postIndex[i + 0]);
					nIndices[1] = word (postIndex[i + 1]);
					nIndices[2] = word (postIndex[i + 2]);
					msTriangle_SetVertexIndices (pTriangle, nIndices);

					nIndices[0] = word (normalIndex[i + 0]);
					nIndices[1] = word (normalIndex[i + 1]);
					nIndices[2] = word (normalIndex[i + 2]);
					msTriangle_SetNormalIndices (pTriangle, nIndices);
					msTriangle_SetSmoothingGroup (pTriangle, 1);
					
				}
				delete[] normal;
				delete[] sortPosit;
				delete[] postIndex;
				delete[] normalIndex;
			}
		}
	}

	AddSkeleton (pModel, model);
	delete model;
	return 0;
}
