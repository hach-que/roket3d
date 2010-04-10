// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CSphereSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "CGeometryCreator.h"
#include "S3DVertex.h"
#include "os.h"

namespace irr
{
namespace scene
{

//! constructor
CSphereSceneNode::CSphereSceneNode(f32 radius, u32 polyCountX, u32 polyCountY, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, const core::vector3df& rotation, const core::vector3df& scale)
: IMeshSceneNode(parent, mgr, id, position, rotation, scale), Mesh(0),
	Radius(radius), PolyCountX(polyCountX), PolyCountY(polyCountY)
{
	#ifdef _DEBUG
	setDebugName("CSphereSceneNode");
	#endif

#ifdef _IRR_MOD_PERPIXEL_BASIC
	Shadow = 0;
	ShadowMesh = CGeometryCreator::createSphereMesh(radius, 10, 4);
#endif

	Mesh = CGeometryCreator::createSphereMesh(radius, polyCountX, polyCountY);
}



//! destructor
CSphereSceneNode::~CSphereSceneNode()
{
	if (Mesh)
		Mesh->drop();

#ifdef _IRR_MOD_PERPIXEL_BASIC
	if (ShadowMesh)
		ShadowMesh->drop();

	if (Shadow)
		Shadow->drop();
#endif
}


//! renders the node.
void CSphereSceneNode::render()
{
	if (!Mesh)
		return;

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

#ifdef _IRR_MOD_PERPIXEL_BASIC
	++PassCount;

	if (Shadow && PassCount==1)
	{
		Shadow->updateShadowVolumes();
	}
#endif

	if (Mesh && driver)
	{
		driver->setMaterial(Mesh->getMeshBuffer(0)->getMaterial());
		driver->drawMeshBuffer(Mesh->getMeshBuffer(0));
		if ( DebugDataVisible & scene::EDS_BBOX )
		{
			video::SMaterial m;
			m.Lighting = false;
			driver->setMaterial(m);
			driver->draw3DBox(Mesh->getMeshBuffer(0)->getBoundingBox(), video::SColor(255,255,255,255));
		}
	}
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CSphereSceneNode::getBoundingBox() const
{
	return Mesh ? Mesh->getBoundingBox() : Box;
}


void CSphereSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
		PassCount = 0;
	}

	ISceneNode::OnRegisterSceneNode();
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CSphereSceneNode::getMaterial(u32 i)
{
	if (i>0 || !Mesh)
		return ISceneNode::getMaterial(i);
	else
		return Mesh->getMeshBuffer(i)->getMaterial();
}


//! returns amount of materials used by this scene node.
u32 CSphereSceneNode::getMaterialCount() const
{
	return 1;
}


//! Writes attributes of the scene node.
void CSphereSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

	out->addFloat("Radius", Radius);
	out->addInt("PolyCountX", PolyCountX);
	out->addInt("PolyCountY", PolyCountY);
}


//! Reads attributes of the scene node.
void CSphereSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	f32 oldRadius = Radius;
	u32 oldPolyCountX = PolyCountX;
	u32 oldPolyCountY = PolyCountY;

	Radius = in->getAttributeAsFloat("Radius");
	PolyCountX = in->getAttributeAsInt("PolyCountX");
	PolyCountY = in->getAttributeAsInt("PolyCountY");
	// legacy values read for compatibility with older versions
	u32 polyCount = in->getAttributeAsInt("PolyCount");
	if (PolyCountX ==0 && PolyCountY == 0)
		PolyCountX = PolyCountY = polyCount;

	Radius = core::max_(Radius, 0.0001f);

	if ( !core::equals(Radius, oldRadius) || PolyCountX != oldPolyCountX || PolyCountY != oldPolyCountY)
	{
		if (Mesh)
			Mesh->drop();
		Mesh = CGeometryCreator::createSphereMesh(Radius, PolyCountX, PolyCountY);
	}

	ISceneNode::deserializeAttributes(in, options);
}

#ifdef _IRR_MOD_PERPIXEL_BASIC
//! Creates shadow volume scene node as child of this node
//! and returns a pointer to it.
IShadowVolumeSceneNode* CSphereSceneNode::addShadowVolumeSceneNode(const IMesh* shadowMesh,
						s32 id, bool zfailmethod, f32 infinity)
{
	if (!SceneManager->getVideoDriver()->queryFeature(video::EVDF_STENCIL_BUFFER))
		return 0;

	if (Shadow)
	{
		os::Printer::log("This node already has a shadow.", ELL_WARNING);
		return 0;
	}

	if (!shadowMesh)
		shadowMesh = ShadowMesh; // if null is given, use the mesh of node

	Shadow = new CShadowVolumeSceneNode(shadowMesh, this, SceneManager, id,  zfailmethod, infinity);
	return Shadow;
}
#endif

//! Creates a clone of this scene node and its children.
ISceneNode* CSphereSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent)
		newParent = Parent;
	if (!newManager)
		newManager = SceneManager;

	CSphereSceneNode* nb = new CSphereSceneNode(Radius, PolyCountX, PolyCountY, newParent, 
		newManager, ID, RelativeTranslation);

	nb->cloneMembers(this, newManager);
	nb->getMaterial(0) = Mesh->getMeshBuffer(0)->getMaterial();

	nb->drop();
	return nb;
}

} // end namespace scene
} // end namespace irr

