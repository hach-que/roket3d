#ifndef __OGL_MODEL_H__
#define __OGL_MODEL_H__

#include <dModel.h>



class OGLLoaderContext: public dLoaderContext
{
	public:
	OGLLoaderContext();
	~OGLLoaderContext();
	virtual dMesh* CreateMesh (int type);
	virtual void LoaderFixup (dModel* model);
};


class OGLModel: public dModel
{
public:
	OGLModel(void);
	virtual ~OGLModel(void);

	virtual void Render();
	void SetMatrix (const dMatrix& matrix);

	dMatrix m_matrix;

	static dMatrix m_matrixPalete[64];
};

#endif