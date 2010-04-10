//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// simple 4d vector class
//********************************************************************

// HeightFieldPrimitive.h: interface for the HeightFieldPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HeightFieldPrimitive_H____INCLUDED_)
#define AFX_HeightFieldPrimitive_H____INCLUDED_


#include "Newton.h"
#include "RenderPrimitive.h"


class HeightFieldPrimitive: public RenderPrimitive   
{
	public:
	HeightFieldPrimitive(NewtonWorld* nWorld);
	virtual ~HeightFieldPrimitive();

	NewtonBody* GetRigidBody() const;

	static void Destructor (const NewtonBody* body);
	static void DebugCallback (const NewtonBody* bodyWithTreeCollision, const NewtonBody* body, int faceID, int vertexCount, const dFloat* vertex, int vertexstrideInBytes);

	private:
	NewtonBody* m_level;
};

#endif 

