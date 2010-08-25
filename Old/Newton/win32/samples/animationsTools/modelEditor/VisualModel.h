#ifndef __OGL_MESH_H__
#define __OGL_MESH_H__
#include "OGLModel.h"
#include <dTree.h>
#include "dBonesToPoseBinding.h"

class VisualModel: public OGLModel
{
	public:
	VisualModel(const char* name);
	~VisualModel(void);

	void FrameCamera () const;

	void SetFrame (dFloat param);		
	void SelectAnimation(int index);
	void DeleteAnimation(int index);



	static void UpdateBoneFormAnimation (void* userData, dFloat* posit, dFloat* rotation);
	dTree<dAnimationClip*, int> m_animationsMenu; 
	dBonesToPoseBinding* m_player;
};


#endif