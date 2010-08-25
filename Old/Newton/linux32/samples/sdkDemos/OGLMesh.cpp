#include "toolbox_stdafx.h"
#include "dBone.h"
#include "OGLMesh.h"
#include "TargaToOpenGl.h"
#include "ShaderPrograms.h"

dMatrix OGLMesh::m_matrixPalete[64];
dFloat OGLMesh::m_weigtedVertex[3 * 8096];
dFloat OGLMesh::m_weigtedNormal[3 * 8096];


#define	SPECULAR_SHINESS	127

OGLMesh::OGLMesh(dMesh::dMeshType type)
	:dMesh(type)
{
	m_indexBuffer = 0;
	m_vertexBuffer = 0;
	m_shaderProgram = ShaderPrograms::GetCache().m_diffuseEffect;
}

OGLMesh::OGLMesh(const NewtonCollision* collision, const char* texture0, const char* texture1, const char* texture2)
	:dMesh(dMesh::D_STATIC_MESH)
{
	NewtonMesh* mesh;

	m_indexBuffer = 0;
	m_vertexBuffer = 0;
	m_shaderProgram = ShaderPrograms::GetCache().m_diffuseEffect;

	// create a helper mesh from the collision collision
	mesh = NewtonMeshCreateFromCollision(collision);

	// apply the vertex normals
	NewtonMeshCalculateVertexNormals(mesh, 30.0f * 3.141592f/180.0f);

	// apply uv projections
	NewtonCollisionInfoRecord info;
	NewtonCollisionGetInfo (collision, &info);
	switch (info.m_collisionType) 
	{
		case SERIALIZE_ID_SPHERE:
		{
			NewtonMeshApplySphericalMapping(mesh, LoadTexture (texture0));
			break;
		}

		case SERIALIZE_ID_CYLINDER:
		{
			NewtonMeshApplyCylindricalMapping(mesh, LoadTexture(texture0), LoadTexture(texture1));
			break;
		}

		case SERIALIZE_ID_CONE:
		case SERIALIZE_ID_CAPSULE:
		case SERIALIZE_ID_CHAMFERCYLINDER:
		{
			NewtonMeshApplySphericalMapping(mesh, LoadTexture(texture0));
			break;
		}

		default:
		{
			int tex0;
			int tex1;
			int tex2;

			tex0 = LoadTexture(texture0);
			tex1 = LoadTexture(texture1);
			tex2 = LoadTexture(texture2);
			NewtonMeshApplyBoxMapping(mesh, tex0, tex1, tex2);
			break;
		}
	}

	BuildFromMesh (mesh);

	// destroy helper mesh
	NewtonMeshDestroy(mesh);

	// optimiz thsi mesh for hardwere buffers if possible
	OptimizeForRender ();
}

OGLMesh::~OGLMesh(void)
{
	// delete VBO when program terminated
	if (m_vertexBuffer) {
		glDeleteBuffersARB(1, &m_indexBuffer);
		glDeleteBuffersARB(1, &m_vertexBuffer);
	}
}

void OGLMesh::OptimizeForRender ()
{
	if (m_shaderProgram) {
		// delete VBO when program terminated
		if (m_vertexBuffer) {
			glDeleteBuffersARB(1, &m_indexBuffer);
			glDeleteBuffersARB(1, &m_vertexBuffer);
		}

		
		glGenBuffersARB(1, &m_vertexBuffer);
		glGenBuffersARB(1, &m_indexBuffer);

		_ASSERTE (m_vertexBuffer);
		_ASSERTE (m_indexBuffer);
		if (m_indexBuffer) {
			int offset;
			int vertexSize;
			glGenBuffersARB(1, &m_vertexBuffer);

			glBindBufferARB (GL_ARRAY_BUFFER_ARB, m_vertexBuffer);

			vertexSize = (3 + 3 + 2) * sizeof (GLfloat);
			if (GetType() == D_SKIN_MESH) {
				vertexSize += sizeof (dVector);
				vertexSize += sizeof (dWeightList::dBoneWeightIndex);
			}

			glBufferDataARB (GL_ARRAY_BUFFER_ARB, m_vertexCount * vertexSize, NULL, GL_STATIC_DRAW_ARB);

			// vertex;
			offset = 0;
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, 3 * m_vertexCount * sizeof (GLfloat), m_vertex);

			// normal
			offset += 3 * m_vertexCount * sizeof (GLfloat);
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, 3 * m_vertexCount * sizeof (GLfloat), m_normal);

			// uv
			offset += 3 * m_vertexCount * sizeof (GLfloat);
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, 2 * m_vertexCount * sizeof (GLfloat), m_uv);

			if (GetType() == D_SKIN_MESH) {
				_ASSERTE (0);
	//			glGenBuffersARB(1, &m_vertexWeightBuffer);
	//			glGenBuffersARB(1, &m_BoneWeightIndexBuffer);
	//			glBindBufferARB (GL_ARRAY_BUFFER_ARB, m_vertexWeightBuffer);
	//			glBindBufferARB (GL_ARRAY_BUFFER_ARB, m_BoneWeightIndexBuffer);
	//			glBufferDataARB (GL_ARRAY_BUFFER_ARB, m_vertexCount * sizeof (dWeightList::dBoneWeightIndex), m_weighList->m_boneWeightIndex, GL_STATIC_DRAW_ARB);
	//			glBufferDataARB (GL_ARRAY_BUFFER_ARB, m_vertexCount * sizeof (dVector),  m_weighList->m_vertexWeight, GL_STATIC_DRAW_ARB);
			}

			int indexCount;
			indexCount = 0;
			for (dMesh::dListNode* nodes = GetFirst(); nodes; nodes = nodes->GetNext()) {
				dSubMesh& segment = nodes->GetInfo();
				indexCount += segment.m_indexCount;
			}

			
			glBindBufferARB (GL_ELEMENT_ARRAY_BUFFER_ARB, m_indexBuffer);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof (unsigned short) * indexCount, NULL, GL_STATIC_DRAW_ARB);
			indexCount = 0;
			for (dMesh::dListNode* nodes = GetFirst(); nodes; nodes = nodes->GetNext()) {
				dSubMesh& segment = nodes->GetInfo();
				glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexCount * sizeof (unsigned short), segment.m_indexCount * sizeof (unsigned short), segment.m_indexes);
				indexCount += segment.m_indexCount;
			}
		}
	}
}


NewtonMesh* OGLMesh::BuildMesh () const
{
	dFloat *uv;
	dFloat *point;
	dFloat *normal;
	NewtonMesh* mesh;

	// create a newton mesh
	mesh = NewtonMeshCreate();

	// add the triangles to the mesh 
	NewtonMeshBeginFace(mesh);

	point = m_vertex;
	normal = m_normal;
	uv = m_uv;
	for (dListNode* node = GetFirst(); node; node = node->GetNext()) {
		GLuint textureID;
		GLushort *indexLixt;
		const dSubMesh& info = node->GetInfo();

		indexLixt = info.m_indexes;
		textureID = info.m_textureHandle; 
		for (int i = 0; i < info.m_indexCount; i += 3){
			struct TrianglePoint
			{
				dFloat m_x;
				dFloat m_y;
				dFloat m_z;
				dFloat m_nx;
				dFloat m_ny;
				dFloat m_nz;
				dFloat m_u;
				dFloat m_v;
			} triangle[3];

			for (int j = 0; j < 3; j ++) {
				int k; 

				k = indexLixt[i + j] * 3;
				triangle[j].m_x = point[k + 0];
				triangle[j].m_y = point[k + 1];
				triangle[j].m_z = point[k + 2];

				k = indexLixt[i + j] * 3;
				triangle[j].m_nx = normal[k + 0];
				triangle[j].m_ny = normal[k + 1];
				triangle[j].m_nz = normal[k + 2];

				k = indexLixt[i + j] * 2;
				triangle[j].m_u = uv[k + 0];
				triangle[j].m_v = uv[k + 1];
			}

			NewtonMeshAddFace(mesh, 3, &triangle[0].m_x, sizeof (TrianglePoint), textureID);
		}
	}

	// finish mesh constructions
	NewtonMeshEndFace(mesh);

	return mesh;
}



void OGLMesh::BuildFromMesh (const NewtonMesh* mesh)
{
	int vertexCount;

	// extract vertex data  from the newton mesh		
	vertexCount = NewtonMeshGetVertexCount (mesh); 
	AllocVertexData(vertexCount);
	NewtonMeshGetVertexStreams (mesh, 
		3 * sizeof (dFloat), (dFloat*) m_vertex,
		3 * sizeof (dFloat), (dFloat*) m_normal,
		2 * sizeof (dFloat), (dFloat*) m_uv);

	// extract the materials index array for mesh
	for (int handle = NewtonMeshFirstMaterial (mesh); handle != -1; handle = NewtonMeshNextMaterial (mesh, handle)) {

		int material; 
		int indexCount; 
		dSubMesh* segment;

		material = NewtonMeshMaterialGetMaterial (mesh, handle); 
		indexCount = NewtonMeshMaterialGetIndexCount (mesh, handle); 

		segment = AddSubMesh();
		segment->m_textureHandle = (GLuint)material;

		segment->AllocIndexData (indexCount);
		NewtonMeshMaterialGetIndexStreamShort (mesh, handle, (short int*)segment->m_indexes); 
	}
}


void OGLMesh::Render(const dMatrix* matrixPallete)
{

	if (GetType() == dMesh::D_STATIC_MESH) {
		if (m_shaderProgram) {
			RenderHardwareSimpleMesh();
		} else {
			RenderSoftwareSimpleMesh();
		}
	} else {
		if (m_shaderProgram) {
			RenderHardwareSkin(matrixPallete);
		} else {
			RenderSoftwareSkin(matrixPallete);
		}
	}
}

void OGLMesh::RenderSoftwareSimpleMesh() const
{
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	glVertexPointer (3, GL_FLOAT, 0, m_vertex);
	glNormalPointer (GL_FLOAT, 0, m_normal);
	glTexCoordPointer (2, GL_FLOAT, 0, m_uv);

	for (dMesh::dListNode* nodes = GetFirst(); nodes; nodes = nodes->GetNext()) {
		dSubMesh& segment = nodes->GetInfo();

		glMaterialfv(GL_FRONT, GL_SPECULAR, &segment.m_specular.m_x);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &segment.m_ambient.m_x);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &segment.m_diffuse.m_x);
		glMaterialf(GL_FRONT, GL_SHININESS, segment.m_shiness);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if (segment.m_textureHandle) {
			glEnable(GL_TEXTURE_2D);		
			glBindTexture(GL_TEXTURE_2D, GLuint (segment.m_textureHandle));
		} else {
			glDisable(GL_TEXTURE_2D);
		}

		glDrawElements (GL_TRIANGLES, segment.m_indexCount, GL_UNSIGNED_SHORT, segment.m_indexes);
	}
	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// disable normal arrays
}


void OGLMesh::RenderHardwareSimpleMesh() const
{
	int indexCount;
	GLint textureEnableOnOff;
	glUseProgram(m_shaderProgram);

	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertexBuffer);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_indexBuffer); 

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glNormalPointer (GL_FLOAT, 0, (GLvoid *) (3 * m_vertexCount * sizeof (GLfloat)));
	glTexCoordPointer (2, GL_FLOAT, 0, (GLvoid *) (6 * m_vertexCount * sizeof (GLfloat)));

	textureEnableOnOff = glGetAttribLocation(m_shaderProgram, "textureEnableOnOff"); 

	indexCount = 0;
	for (dMesh::dListNode* nodes = GetFirst(); nodes; nodes = nodes->GetNext()) {
		dSubMesh& segment = nodes->GetInfo();

		glMaterialfv(GL_FRONT, GL_SPECULAR, &segment.m_specular.m_x);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &segment.m_ambient.m_x);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &segment.m_diffuse.m_x);
		glMaterialf(GL_FRONT, GL_SHININESS, segment.m_shiness);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if (segment.m_textureHandle) {
			glEnable(GL_TEXTURE_2D);		
			glBindTexture(GL_TEXTURE_2D, GLuint (segment.m_textureHandle));
			glVertexAttrib4f (textureEnableOnOff, 1.0f, 1.0f, 1.0f, 1.0f); 
		} else {
			glDisable(GL_TEXTURE_2D);
			glVertexAttrib4f (textureEnableOnOff, 0.0f, 0.0f, 0.0f, 0.0f); 
		}

//		glDrawElements (GL_TRIANGLES, segment.m_indexCount, GL_UNSIGNED_SHORT, (GLvoid*) (indexCount * sizeof (GLshort)));
		glDrawRangeElements (GL_TRIANGLES, 0, 32767, segment.m_indexCount, GL_UNSIGNED_SHORT, (GLvoid*) (indexCount * sizeof (GLshort))); 
		indexCount += segment.m_indexCount;
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0); 

	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// disable normal arrays
}


void OGLMesh::RenderSoftwareSkin(const dMatrix* matrixPallete) const
{
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	for (int i = 0; i < m_weighList->m_bonesCount; i ++) {
		int id;
		id = m_weighList->m_boneNodes[i]->m_boneID;
		m_matrixPalete[id] = m_weighList->m_bindingMatrices[id] * matrixPallete[id];
	}

	for (int i = 0; i < m_vertexCount; i ++) {
		dVector p (dFloat (0.0f), dFloat (0.0f), dFloat (0.0f), dFloat (0.0f));   
		dVector q (m_vertex[i * 3 + 0], m_vertex[i * 3 + 1], m_vertex[i * 3 + 2], dFloat (0.0f));

		const dVector& weight = m_weighList->m_vertexWeight[i];
		const dWeightList::dBoneWeightIndex& weightIndex = m_weighList->m_boneWeightIndex[i];
		for (int j = 0; (j < 4) && weight[j] > dFloat(0.0); j ++) {
			int bone;
			bone = weightIndex.m_index[j];
			p += m_matrixPalete[bone].TransformVector (q).Scale (weight[j]);
		}

		m_weigtedVertex[i * 3 + 0] = dFloat (p.m_x);
		m_weigtedVertex[i * 3 + 1] = dFloat (p.m_y);
		m_weigtedVertex[i * 3 + 2] = dFloat (p.m_z);

		dVector n (m_normal[i * 3 + 0], m_normal[i * 3 + 1], m_normal[i * 3 + 2], dFloat (0.0f));
		n = m_matrixPalete[weightIndex.m_index[0]].RotateVector (n);
		m_weigtedNormal[i * 3 + 0] = dFloat (n.m_x);
		m_weigtedNormal[i * 3 + 1] = dFloat (n.m_y);
		m_weigtedNormal[i * 3 + 2] = dFloat (n.m_z);
	}

	glVertexPointer (3, GL_FLOAT, 0, m_weigtedVertex);
	glNormalPointer (GL_FLOAT, 0, m_weigtedNormal);
	glTexCoordPointer (2, GL_FLOAT, 0, m_uv);

	for (dMesh::dListNode* nodes = GetFirst(); nodes; nodes = nodes->GetNext()) {
		dSubMesh& segment = nodes->GetInfo();

		glMaterialfv(GL_FRONT, GL_SPECULAR, &segment.m_specular.m_x);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &segment.m_ambient.m_x);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &segment.m_diffuse.m_x);
		glMaterialf(GL_FRONT, GL_SHININESS, segment.m_shiness);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		if (segment.m_textureHandle) {
			glEnable(GL_TEXTURE_2D);		
			glBindTexture(GL_TEXTURE_2D, GLuint (segment.m_textureHandle));
		} else {
			glDisable(GL_TEXTURE_2D);
		}

		glDrawElements (GL_TRIANGLES, segment.m_indexCount, GL_UNSIGNED_SHORT, segment.m_indexes);
	}
	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// disable normal arrays
}


void OGLMesh::RenderHardwareSkin(const dMatrix* matrixPallete) const
{
	GLint attribIndex;
	GLint attribWeights;
	GLint matrixPalleteID;
	GLint textureEnableOnOff;

	glUseProgram(m_shaderProgram);


	matrixPalleteID = glGetUniformLocation(m_shaderProgram, "matrixPallete"); 
	attribIndex = glGetAttribLocation(m_shaderProgram, "bonesIndices"); 
	attribWeights = glGetAttribLocation(m_shaderProgram, "bonesWeights"); 
													  
	for (int i = 0; i < m_weighList->m_bonesCount; i ++) {
		int id;
		id = m_weighList->m_boneNodes[i]->m_boneID;
		m_matrixPalete[id] = m_weighList->m_bindingMatrices[id] * matrixPallete[id];
	}

	glUniformMatrix4fv(matrixPalleteID, m_weighList->m_bonesCount, GL_FALSE, &m_matrixPalete[0][0][0]); 

	glEnableVertexAttribArray (attribIndex); 
	glEnableVertexAttribArray (attribWeights); 

	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	glVertexAttribPointer (attribIndex,   4,  GL_INT, GL_FALSE, 0, m_weighList->m_boneWeightIndex); 
	glVertexAttribPointer (attribWeights, 4, GL_FLOAT, GL_FALSE, 0, m_weighList->m_vertexWeight); 

	glVertexPointer (3, GL_FLOAT, 0, m_vertex);
	glNormalPointer (GL_FLOAT, 0, m_normal);
	glTexCoordPointer (2, GL_FLOAT, 0, m_uv);

	textureEnableOnOff = glGetAttribLocation(m_shaderProgram, "textureEnableOnOff"); 

	for (dMesh::dListNode* nodes = GetFirst(); nodes; nodes = nodes->GetNext()) {
		dSubMesh& segment = nodes->GetInfo();

		glMaterialfv(GL_FRONT, GL_SPECULAR, &segment.m_specular.m_x);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &segment.m_ambient.m_x);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &segment.m_diffuse.m_x);
		glMaterialf(GL_FRONT, GL_SHININESS, segment.m_shiness);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		if (segment.m_textureHandle) {
			glEnable(GL_TEXTURE_2D);		
			glBindTexture(GL_TEXTURE_2D, GLuint (segment.m_textureHandle));
			glVertexAttrib4f (textureEnableOnOff, 1.0f, 1.0f, 1.0f, 1.0f); 
		} else {
			glDisable(GL_TEXTURE_2D);
			glVertexAttrib4f (textureEnableOnOff, 0.0f, 0.0f, 0.0f, 0.0f); 
		}

		glDrawElements (GL_TRIANGLES, segment.m_indexCount, GL_UNSIGNED_SHORT, segment.m_indexes);
	}

	glDisableVertexAttribArray(attribIndex);
	glDisableVertexAttribArray(attribWeights); 
	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	// disable normal arrays
}

