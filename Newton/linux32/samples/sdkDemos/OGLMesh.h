#ifndef __OGL_MESH_H__
#define __OGL_MESH_H__

#include <dMesh.h>
//#include <Newton.h>



class OGLMesh: public dMesh
{
public:
	OGLMesh(dMesh::dMeshType type);
	OGLMesh(const NewtonCollision* collision, const char* texture0, const char* texture1, const char* texture2);
	~OGLMesh(void);

	NewtonMesh* BuildMesh () const;	
	void BuildFromMesh (const NewtonMesh* mesh);
	void Render(const dMatrix* matrixPallete = NULL);
	void OptimizeForRender ();

	private:
	void RenderSoftwareSimpleMesh() const;
	void RenderHardwareSimpleMesh() const;
	void RenderSoftwareSkin(const dMatrix* matrixPallete) const;
	void RenderHardwareSkin(const dMatrix* matrixPallete) const;

	

	public:
	GLuint m_indexBuffer;
	GLuint m_vertexBuffer;
	GLuint m_shaderProgram;

	static dMatrix m_matrixPalete[];
	static dFloat m_weigtedVertex[];
	static dFloat m_weigtedNormal[];

};

#endif

