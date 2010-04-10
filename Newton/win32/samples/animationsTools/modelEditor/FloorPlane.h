#ifndef _FLOOR_PLANE_
#define _FLOOR_PLANE_

#include "OGLModel.h"

class FloorPlane: public OGLModel
{
public:
	FloorPlane(void);
	~FloorPlane(void);

	virtual void Render();

	GLuint m_shaderProgram;
};

#endif