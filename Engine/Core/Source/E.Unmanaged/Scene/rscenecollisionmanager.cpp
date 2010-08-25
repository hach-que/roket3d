#include "roket3d.h"
#include "E.Unmanaged/Scene/rscenecollisionmanager.h"

namespace scene
{
	
	const char *RSceneCollisionManager::className = TYPE_RSCENECOLLISIONMANAGER;
	const Luna < RSceneCollisionManager >::FunctionType RSceneCollisionManager::Functions[] = {
		{"getCollisionPoint", &RSceneCollisionManager::getCollisionPoint},
//   {"getCollisionResultPosition", &RSceneCollisionManager::getCollisionResultPosition},
  {"getRayFromScreenCoordinates", &RSceneCollisionManager::getRayFromScreenCoordinates},
  {"getSceneNodeFromCameraBB", &RSceneCollisionManager::getSceneNodeFromCameraBB},
  {"getSceneNodeFromRayBB", &RSceneCollisionManager::getSceneNodeFromRayBB},
  {"getSceneNodeFromScreenCoordinatesBB", &RSceneCollisionManager::getSceneNodeFromScreenCoordinatesBB},
  {"getScreenCoordinatesFrom3DPosition", &RSceneCollisionManager::getScreenCoordinatesFrom3DPosition},
		{0}
	};
	const Luna < RSceneCollisionManager >::PropertyType RSceneCollisionManager::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RSceneCollisionManager::RSceneCollisionManager(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RSceneCollisionManagers.  You must collect one from an RSceneManager.",LVL_ERR,L);
		managerExists = false;
	}
	
	int RSceneCollisionManager::getCollisionPoint(lua_State* L)
	{
		core::RRay3D *ray3d = Luna<core::RRay3D>::check(L, 1);
		irr::core::line3df irrray = ray3d->getObject();
		double sx = irrray.start.X;
		double sy = irrray.start.Y;
		double sz = irrray.start.Z;
		double ex = irrray.end.X;
		double ey = irrray.end.Y;
		double ez = irrray.end.Z;
		
		// needs an RSceneNode to do collisions with
		// TODO: Check argument check is correct.
		scene::RMeshSceneNode* msh = Luna<scene::RMeshSceneNode>::check(L,1);
		irr::scene::IAnimatedMeshSceneNode * inode = msh->getNode();
		
		irr::core::line3d<cIrrNumber> ray = irr::core::line3d<cIrrNumber>(sx,sy,sz,ex,ey,ez);
		irr::scene::ITriangleSelector * selector = scn->createTriangleSelector(inode->getMesh()->getMesh(0), inode);
		cIrrVector3D outCollisionPoint;
		irr::core::triangle3df outTriangle;
		
		cmanager->getCollisionPoint(ray,selector,outCollisionPoint,outTriangle);
		
		// Return collision point
		core::RVector3D* vector = Luna < core::RVector3D >::createNew(L);
		vector->setObject(outCollisionPoint);
		
		// Return triangle
		core::RTriangle3D* triangle = Luna < core::RTriangle3D >::createNew(L);
		triangle->setObject(outTriangle);
		
		return 2;
	}
	
	int RSceneCollisionManager::getRayFromScreenCoordinates(lua_State* L)
	{
		core::RVector2D *vector2d = Luna<core::RVector2D>::check(L, 1);
		cIrrVector2D irrvector2d = vector2d->getObject();
		double x = irrvector2d.X;
		double y = irrvector2d.Y;
		
		irr::core::line3df ray = cmanager->getRayFromScreenCoordinates(irr::core::position2d<int>(x,y));
		
		core::RRay3D* returnray = Luna < core::RRay3D >::createNew(L);
		returnray->setObject(ray);
		
		return 1;
	}
	
	/*
	virtual ISceneNode *   getSceneNodeFromCameraBB (ICameraSceneNode *camera, s32 idBitMask=0, bool bNoDebugObjects=false)=0
	virtual ISceneNode *   getSceneNodeFromRayBB (core::line3d< f32 > ray, s32 idBitMask=0, bool bNoDebugObjects=false)=0
	virtual ISceneNode *   getSceneNodeFromScreenCoordinatesBB (core::position2d< s32 > pos, s32 idBitMask=0, bool bNoDebugObjects=false)=0
	virtual core::position2d<
	s32 >   getScreenCoordinatesFrom3DPosition (core::vector3df pos, ICameraSceneNode *camera=0)=0
	*/
	
	int RSceneCollisionManager::getSceneNodeFromCameraBB(lua_State* L)
	{
		irr::scene::ISceneNode * node = cmanager->getSceneNodeFromCameraBB(scn->getActiveCamera());
		if (node==0)
		{
			lua_pushnil(L);
			return 1;
		}
		if (node->RoketGetUserData()==0)
		{
			lua_pushnil(L);
			return 1;
		}
		
		safeVP * data = reinterpret_cast<safeVP*>(node->RoketGetUserData());
		if (data->type()=="RSceneNode")
		{
			Luna <scene::RSceneNode>::createFromExisting(L,
					reinterpret_cast<scene::RSceneNode*>(data->voidPointer()));
			return 1;
		}
		else if (data->type()=="RMeshSceneNode")
		{
			Luna <scene::RMeshSceneNode>::createFromExisting(L,
					reinterpret_cast<scene::RMeshSceneNode*>(data->voidPointer()));
			return 1;
		}
		return 1;
	}
	
	int RSceneCollisionManager::getSceneNodeFromRayBB(lua_State* L)
	{
		core::RRay3D *ray3d = Luna<core::RRay3D>::check(L, 1);

		irr::core::line3df irrray = ray3d->getObject();
		cNumber sx = irrray.start.X;
		cNumber sy = irrray.start.Y;
		cNumber sz = irrray.start.Z;
		cNumber ex = irrray.end.X;
		cNumber ey = irrray.end.Y;
		cNumber ez = irrray.end.Z;
		
		irr::scene::ISceneNode * node = cmanager->getSceneNodeFromRayBB(irr::core::line3d<cIrrNumber>(sx,sy,sz,ex,ey,ez));
		if (node==0)
		{
			lua_pushnil(L);
			return 1;
		}
		if (node->RoketGetUserData()==0)
		{
			lua_pushnil(L);
			return 1;
		}
		
		safeVP * data = reinterpret_cast<safeVP*>(node->RoketGetUserData());
		if (data->type()=="RSceneNode")
		{
			Luna <scene::RSceneNode>::createFromExisting(L,
					reinterpret_cast<scene::RSceneNode*>(data->voidPointer()));
			return 1;
		}
		else if (data->type()=="RMeshSceneNode")
		{
			Luna <scene::RMeshSceneNode>::createFromExisting(L,
					reinterpret_cast<scene::RMeshSceneNode*>(data->voidPointer()));
			return 1;
		}
	}
	
	int RSceneCollisionManager::getSceneNodeFromScreenCoordinatesBB(lua_State* L)
	{
		core::RVector2D *vector2d = Luna<core::RVector2D>::check(L, 2);
		cIrrVector2D irrvector2d = vector2d->getObject();
		double x = irrvector2d.X;
		double y = irrvector2d.Y;
		
		irr::scene::ISceneNode * node = cmanager->getSceneNodeFromScreenCoordinatesBB(irr::core::position2d<int>(x,y));
		if (node==0)
		{
			debug("No scene node found at " << x << "," << y << ".  Nil was returned.",LVL_WARN,L);
			lua_pushnil(L);
			return 1;
		}
		if (node->RoketGetUserData()==0)
		{
			debug("Invalid scene node found at " << x << "," << y << ".  Nil was returned.",LVL_WARN,L);
			lua_pushnil(L);
			return 1;
		}
		
		safeVP * data = reinterpret_cast<safeVP*>(node->RoketGetUserData());
		if (data->type()=="RSceneNode")
		{
			debug("Found RSceneNode at " << x << "," << y << ".",LVL_INFO,L);
			Luna <scene::RSceneNode>::createFromExisting(L,
					reinterpret_cast<scene::RSceneNode*>(data->voidPointer()));
			return 1;
		}
		else if (data->type()=="RMeshSceneNode")
		{
			debug("Found RMeshSceneNode at " << x << "," << y << ".",LVL_INFO,L);
			Luna <scene::RMeshSceneNode>::createFromExisting(L,
					reinterpret_cast<scene::RMeshSceneNode*>(data->voidPointer()));
			return 1;
		}
		else
		{
			debug("Unable to recognise scene node found at " << x << "," << y << ".  Nil was returned.",LVL_WARN,L);
			return 0;
		}
	}
	
	int RSceneCollisionManager::getScreenCoordinatesFrom3DPosition(lua_State* L)
	{
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		
		cIrrVector3D irrvec = vector3d->getObject();
		double x = irrvec.X;
		double y = irrvec.Y;
		double z = irrvec.Z;
		
		irr::core::position2d<irr::s32> point =  cmanager->getScreenCoordinatesFrom3DPosition(cIrrVector3D(x,y,z));

		lua_newtable(L);
		lua_pushstring(L, "x"); lua_pushnumber(L, point.X); lua_settable (L, -3); lua_pop(L, 2);
		lua_pushstring(L, "y"); lua_pushnumber(L, point.Y); lua_settable (L, -3); lua_pop(L, 2);

		return 1;
	}
	
	void RSceneCollisionManager::setCollisionManager(irr::scene::ISceneCollisionManager* newmanager)
	{
		cmanager = newmanager;
	}
	
	void RSceneCollisionManager::setSceneManager(irr::scene::ISceneManager* newscn)
	{
		scn = newscn;
	}
	
	irr::scene::ISceneCollisionManager* RSceneCollisionManager::getCollisionManager()
	{
		return cmanager;
	}
}
