#pragma once


#include "msPlugIn.h"

class dModel;
struct msModel;


class Importer : public cMsPlugIn
{
	public:
	Importer ();
    virtual ~Importer ();


    int             GetType ();
    const char *    GetTitle ();
    int             Execute (msModel* pModel);

	void ConvertMeshToSkins(dModel* model);
	void MergeEqualMaterials(dModel* model);
    void AddSkeleton (msModel* pModel, dModel* model);
	
};
