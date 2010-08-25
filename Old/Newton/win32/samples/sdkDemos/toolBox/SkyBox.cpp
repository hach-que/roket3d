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
#include <toolbox_stdafx.h>
#include "OpenGlUtil.h"
#include "dTree.h"
#include "dCRC.h"
#include "../OGLMesh.h"
#include "TargaToOpenGl.h"
#include "SkyBox.h"
#include "ShaderPrograms.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SkyBox::SkyBox()
	:OGLModel ()
{
	dFloat boxsize;

	boxsize = 200.0f;

	m_shaderProgram = ShaderPrograms::GetCache().m_decalEffect;

	m_matrix = GetIdentityMatrix();

	m_size = dVector (boxsize, boxsize, boxsize);
	m_textures[0] = LoadTexture("NewtonSky0001.tga");
	m_textures[1] = LoadTexture("NewtonSky0002.tga");
	m_textures[2] = LoadTexture("NewtonSky0003.tga");
	m_textures[3] = LoadTexture("NewtonSky0004.tga");
	m_textures[4] = LoadTexture("NewtonSky0005.tga");
	m_textures[5] = LoadTexture("NewtonSky0006.tga");
}

void SkyBox::Render()
{
	dMatrix matrix;

	if (m_shaderProgram) {
		glUseProgram(m_shaderProgram);
	}


	// get the model viewMatrix; 
	glGetFloat (GL_MODELVIEW_MATRIX, &matrix[0][0]);

	dMatrix skyMatrix (m_matrix);
	skyMatrix.m_posit = matrix.UnrotateVector (matrix.m_posit.Scale (-1.0f));
	skyMatrix.m_posit.m_y += 50.0f; 

	glColor3f(1.0f, 1.0f, 1.0f);
	
	glPushMatrix();
	glMultMatrix(&skyMatrix[0][0]);

//	glDisable(GL_DEPTH_TEST);
//	glDepthMask(GL_FALSE);
	glDisable (GL_LIGHTING);

	// set up the cube's texture
	glEnable(GL_TEXTURE_2D);


	dFloat padd;
	padd = 1.0e-3f;

	//	uncommenting this will make a perfect sky box texture stitch, unfortunately
	//  this option ios not supported by all version of opengl	
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// the cube will just be drawn as six quads for the sake of simplicity
	// for each face, we specify the quad's normal (for lighting), then
	// specify the quad's 4 vertices's and associated texture coordinates
	// front
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f - padd, 1.0f - padd); glVertex3f(-m_size.m_x,  m_size.m_y, m_size.m_z);
	glTexCoord2f(0.0f + padd, 1.0f - padd); glVertex3f( m_size.m_x,  m_size.m_y, m_size.m_z);
	glTexCoord2f(0.0f + padd, 0.0f + padd); glVertex3f( m_size.m_x, -m_size.m_y, m_size.m_z);
	glTexCoord2f(1.0f - padd, 0.0f + padd); glVertex3f(-m_size.m_x, -m_size.m_y, m_size.m_z);
	glEnd();


	// left
	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f - padd, 1.0f - padd); glVertex3f(-m_size.m_x,  m_size.m_y, -m_size.m_z);
	glTexCoord2f(0.0f + padd, 1.0f - padd); glVertex3f(-m_size.m_x,  m_size.m_y,  m_size.m_z);
	glTexCoord2f(0.0f + padd, 0.0f + padd); glVertex3f(-m_size.m_x, -m_size.m_y,  m_size.m_z);
	glTexCoord2f(1.0f - padd, 0.0f + padd); glVertex3f(-m_size.m_x, -m_size.m_y, -m_size.m_z);
	glEnd();


	// right
	glBindTexture(GL_TEXTURE_2D, m_textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f - padd, 1.0f - padd); glVertex3f(m_size.m_x,   m_size.m_y,  m_size.m_z);
	glTexCoord2f(0.0f + padd, 1.0f - padd); glVertex3f(m_size.m_x,   m_size.m_y, -m_size.m_z);
	glTexCoord2f(0.0f + padd, 0.0f + padd); glVertex3f(m_size.m_x,  -m_size.m_y, -m_size.m_z);
	glTexCoord2f(1.0f - padd, 0.0f + padd); glVertex3f(m_size.m_x,  -m_size.m_y,  m_size.m_z);
	glEnd();


	// back
	glBindTexture(GL_TEXTURE_2D, m_textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f - padd, 1.0f - padd); glVertex3f( m_size.m_x,  m_size.m_y, -m_size.m_z);
	glTexCoord2f(0.0f + padd, 1.0f - padd); glVertex3f(-m_size.m_x,  m_size.m_y, -m_size.m_z);
	glTexCoord2f(0.0f + padd, 0.0f + padd); glVertex3f(-m_size.m_x, -m_size.m_y, -m_size.m_z);
	glTexCoord2f(1.0f - padd, 0.0f + padd); glVertex3f( m_size.m_x, -m_size.m_y, -m_size.m_z);
	glEnd();

	// bottom
	glBindTexture(GL_TEXTURE_2D, m_textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f + padd, 0.0f + padd); glVertex3f(-m_size.m_x, -m_size.m_y,  m_size.m_z);
	glTexCoord2f(0.0f + padd, 1.0f - padd); glVertex3f( m_size.m_x, -m_size.m_y,  m_size.m_z);
	glTexCoord2f(1.0f - padd, 1.0f - padd); glVertex3f( m_size.m_x, -m_size.m_y, -m_size.m_z);
	glTexCoord2f(1.0f - padd, 0.0f + padd); glVertex3f(-m_size.m_x, -m_size.m_y, -m_size.m_z);
	glEnd();	 	

	// top
	glBindTexture(GL_TEXTURE_2D, m_textures[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f + padd, 1.0f - padd); glVertex3f(-m_size.m_x,  m_size.m_y, -m_size.m_z);
	glTexCoord2f(0.0f + padd, 0.0f + padd); glVertex3f( m_size.m_x,  m_size.m_y, -m_size.m_z);
	glTexCoord2f(1.0f - padd, 0.0f + padd); glVertex3f( m_size.m_x,  m_size.m_y,  m_size.m_z);
	glTexCoord2f(1.0f - padd, 1.0f - padd); glVertex3f(-m_size.m_x,  m_size.m_y,  m_size.m_z);
	glEnd();	 


//	glDepthMask(GL_TRUE);
//	glEnable(GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);

	// render the rest of the hierarchy
	glPopMatrix();
}
	
SkyBox::~SkyBox()
{
}


