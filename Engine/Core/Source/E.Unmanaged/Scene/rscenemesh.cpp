#include "roket3d.h"
#include "E.Unmanaged/Scene/rscenemesh.h"

namespace scene
{
	const char *RMesh::className = TYPE_RSCENEMESH;
	const Luna < RMesh >::FunctionType RMesh::Functions[] = {
		//{"drawAll", &RMesh::drawAll},
		{0}
	};
	const Luna < RMesh >::PropertyType RMesh::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RMesh::RMesh(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RMeshs.  You must collect one from an RSceneManager.",LVL_ERR,L);
	}
	
	void RMesh::setIrrlichtMesh(irr::scene::IAnimatedMesh* mesh)
	{
		base_self = mesh;
	}
	
	irr::scene::IAnimatedMesh* RMesh::getIrrlichtMesh()
	{
		return base_self;
	}
}
