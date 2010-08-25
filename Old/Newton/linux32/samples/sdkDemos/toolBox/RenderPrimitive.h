//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// 
//********************************************************************


// RenderPrimitive.h: interface for the RenderPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENDERPRIMITIVE_H__0C9BDF8F_FF8D_4F5A_9692_4E76E3C8878A__INCLUDED_)
#define AFX_RENDERPRIMITIVE_H__0C9BDF8F_FF8D_4F5A_9692_4E76E3C8878A__INCLUDED_

#include <toolbox_stdafx.h>
#include "../OGLModel.h"


class OGLMesh;
#define CAMERA_NAME	"cameraNode"
struct NewtonWorld;


class RenderPrimitive: public OGLModel
{
	public:
	RenderPrimitive ();
	RenderPrimitive(const dMatrix& matrix, OGLMesh* geometry);
	virtual ~RenderPrimitive();
	OGLMesh *GetMesh() const;
	void SetMesh(OGLMesh *mesh);

	
	void CreateVisualEffect ();
	public:

	int m_effectTexture;
	NewtonMesh* m_specialEffect;

	dFloat m_density;
	dFloat m_controlVolume;
	dFloat m_destrutableStrength;
	dFloat m_modifierScaleAngleX;
	dFloat m_modifierScaleAngleY;
	dFloat m_modifierScaleAngleZ;
	dFloat m_modifierSkewAngleY;
};

/*
class CameraNode: public dModel
{
	public:
	CameraNode (dModel* parent);

	void Render() const;
	dFloat m_fov;
	dFloat m_aspect;
	dFloat m_farPlane;
	dFloat m_nearPlane;
};
*/

#endif 

