//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// 
//********************************************************************


// RenderPrimitive.cpp: implementation of the RenderPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SHADERS_PROGRAMS__
#define __SHADERS_PROGRAMS__

#include <toolbox_stdafx.h>
#include "OpenGlUtil.h"


class ShaderPrograms
{
	public:
	ShaderPrograms(void);
	~ShaderPrograms(void);

	static ShaderPrograms& GetCache ();

	bool CreateAllEffects();

	private:
	GLuint CreateShaderEffect (const char* name);
	void LoadShaderCode (const char* name, char *buffer);

	public:
	GLuint m_solidColor;
	GLuint m_decalEffect;
	GLuint m_diffuseEffect;
	GLuint m_skinningDiffuseEffect;
};


#endif