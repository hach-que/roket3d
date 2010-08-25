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

#ifndef __SKY_BOX__
#define __SKY_BOX__

#include <toolbox_stdafx.h>
#include "../OGLModel.h"

class SkyBox: public OGLModel
{
	public:
	SkyBox();
	~SkyBox();

	void Render();

	private:
	GLuint m_shaderProgram;
	GLuint m_textures[6];
	dVector m_size;
};

#endif 

