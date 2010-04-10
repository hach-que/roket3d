//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// simple demo list vector class with iterators
//********************************************************************


// ScenePrimitive.h: interface for the ScenePrimitive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE_PRIMITIVE_H__)
#define AFX_SCENE_PRIMITIVE_H__

#include "Newton.h"
#include "../OGLModel.h"


class ScenePrimitive: public OGLModel  
{

	struct VisualProxy
	{	
		dMatrix m_matrix;
		OGLMesh* m_mesh;
		NewtonSceneProxy* m_sceneProxi;
	};

	public:
	ScenePrimitive(const NewtonWorld* nWorld, const char* name, int optimized);
	virtual ~ScenePrimitive();
	NewtonBody* GetRigidBody() const {return m_level;}

	private:

	void AddCollisionTree (NewtonCollision* scene, const NewtonWorld* nWorld, const char* name, int optimized);
	void AddPrimitives (NewtonCollision* scene, const NewtonWorld* nWorld);


	static void DebugCallback (const NewtonBody* bodyWithTreeCollision, const NewtonBody* body,	int faceID, int vertexCount, const dFloat* vertex, int vertexstrideInBytes);

	virtual void Render();

	public:
	NewtonBody* m_level;
	dList<VisualProxy> m_pieces; 
};

#endif 
