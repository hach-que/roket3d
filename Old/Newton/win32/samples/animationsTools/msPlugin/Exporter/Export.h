#pragma once


#include "msPlugIn.h"

class dModel;
struct msModel;


class Exporter : public cMsPlugIn
{
	public:
	Exporter ();
    virtual ~Exporter ();


    int             GetType ();
    const char *    GetTitle ();
    int             Execute (msModel* pModel);

	void ConvertMeshToSkins(dModel* model);
	void MergeEqualMaterials(dModel* model);
    void AddSkeleton (msModel* pModel, dModel* model);
	
};
