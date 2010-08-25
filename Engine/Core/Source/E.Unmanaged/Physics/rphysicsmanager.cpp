#include "roket3d.h"
#include "rphysicsmanager.h"

namespace physics
{
	const char *RPhysicsManager::className = "RPhysicsManager";
	const Luna < RPhysicsManager >::FunctionType RPhysicsManager::Functions[] = {
		{"updateAll", &RPhysicsManager::updateAll},
		{"setMinimumFPS", &RPhysicsManager::setMinimumFPS},
		{"setSolver", &RPhysicsManager::setSolver},
		{"setFrictionModel", &RPhysicsManager::setFrictionModel},
		{"createJoint", &RPhysicsManager::createJoint},
		
		{0}
	};
	const Luna < RPhysicsManager >::PropertyType RPhysicsManager::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RPhysicsManager::RPhysicsManager(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RPhysicsManagers.  You must collect one from an RDevice.",LVL_ERR,L);
	}
	
	int RPhysicsManager::updateAll(lua_State* L)
	{
		ifistrue(objectExists);
		
		obj->updateAll();
		return 0;
		
		endifisvalid();
	}

	int RPhysicsManager::setMinimumFPS(lua_State* L)
	{
		ifistrue(objectExists);
		
		obj->setMinimumFPS(getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}

	int RPhysicsManager::setSolver(lua_State* L)
	{
		ifistrue(objectExists);
		
		int setting = getmandatoryargument_cNumber(L,1);
		if (setting==0 || setting==1 || setting==2)
		{
			obj->setSolver(setting);
		}
		else
		{
			debug("Invalid setting for physics solver.",LVL_ERR,L);
		}
		return 0;
		
		endifisvalid();
	}

	int RPhysicsManager::setFrictionModel(lua_State* L)
	{
		ifistrue(objectExists);
		
		int setting = getmandatoryargument_cNumber(L,1);
		if (setting==0 || setting==1)
		{
			obj->setFrictionModel(setting);
		}
		else
		{
			debug("Invalid setting for physics friction model.",LVL_ERR,L);
		}
		return 0;
		
		endifisvalid();
	}

	int RPhysicsManager::createJoint(lua_State* L)
	{
		ifistrue(objectExists);

		// get joint type
		int jointtype = getmandatoryargument_cNumber(L,1);

		// get parent and child
		Roket_PhysicsBody* parent = NULL; // beacuse the following function may drop out because of a lua
		Roket_PhysicsBody* child  = NULL; // error, we want to MAKE SURE that we have a valid value to compare against
		child = checkBody(L,2);
		if (!lua_isnoneornil(L,4)) // if the 3rd argument is not nil and exists.                    
		{
			parent = checkBody(L,3);
		}

		if (parent == NULL)
		{
			debug("Joint will be connected to the world.",LVL_WARN,L);
		}
		if (child == NULL)
			return 0;

		// check joint type
		if (jointtype<0 || jointtype>4)
			debug("Invalid joint type specified.",LVL_ERR,L);
		ePhysicsJointType jointtype_ = static_cast<ePhysicsJointType>(jointtype);

		// define all the variables
		core::RVector3D* pivot;
		core::RVector3D* pin;
		core::RVector3D* pin0;
		core::RVector3D* pin1;
		RPhysicsJoint* pjoint;
		Roket_PhysicsJointBall* rjoint_ball;
		Roket_PhysicsJointHinge* rjoint_hinge;
		Roket_PhysicsJointSlider* rjoint_slider;
		Roket_PhysicsJointCorkscrew* rjoint_corkscrew;
		Roket_PhysicsJointUniversal* rjoint_universal;

		// create joint
		switch (jointtype_)
		{
			case E_JOINT_BALL_AND_SOCKET:
				// get pivot point
				pivot = Luna<core::RVector3D>::check(L,4);

				pjoint = Luna < physics::RPhysicsJoint >::createNew(L);
				rjoint_ball = new physics::Roket_PhysicsJointBall(
																obj->getNewtonWorld(),
																pivot->getObject(),
																child,
																parent,
																pjoint);
				pjoint->setObjectBallAndSocket(rjoint_ball);
				return 1;
				break;
			case E_JOINT_HINGE:
				// get pivot point
				pivot = Luna<core::RVector3D>::check(L,4);
				pin = Luna<core::RVector3D>::check(L,5);

				pjoint = Luna < physics::RPhysicsJoint >::createNew(L);
				rjoint_hinge = new physics::Roket_PhysicsJointHinge(
																obj->getNewtonWorld(),
																pivot->getObject(),
																pin->getObject(),
																child,
																parent,
																pjoint);
				pjoint->setObjectHinge(rjoint_hinge);
				return 1;
				break;
			case E_JOINT_SLIDER:
				// get pivot point
				pivot = Luna<core::RVector3D>::check(L,4);
				pin = Luna<core::RVector3D>::check(L,5);

				pjoint = Luna < physics::RPhysicsJoint >::createNew(L);
				rjoint_slider = new physics::Roket_PhysicsJointSlider(
																obj->getNewtonWorld(),
																pivot->getObject(),
																pin->getObject(),
																child,
																parent,
																pjoint);
				pjoint->setObjectSlider(rjoint_slider);
				return 1;
				break;
			case E_JOINT_CORKSCREW:
				// get pivot point
				pivot = Luna<core::RVector3D>::check(L,4);
				pin = Luna<core::RVector3D>::check(L,5);

				pjoint = Luna < physics::RPhysicsJoint >::createNew(L);
				rjoint_corkscrew = new physics::Roket_PhysicsJointCorkscrew(
																obj->getNewtonWorld(),
																pivot->getObject(),
																pin->getObject(),
																child,
																parent,
																pjoint);
				pjoint->setObjectCorkscrew(rjoint_corkscrew);
				return 1;
				break;
			
			case E_JOINT_UNIVERSAL:
				// get pivot point
				pivot = Luna<core::RVector3D>::check(L,4);
				pin0 = Luna<core::RVector3D>::check(L,5);
				pin1 = Luna<core::RVector3D>::check(L,6);

				pjoint = Luna < physics::RPhysicsJoint >::createNew(L);
				rjoint_universal = new physics::Roket_PhysicsJointUniversal(
																obj->getNewtonWorld(),
																pivot->getObject(),
																pin0->getObject(),
																pin1->getObject(),
																child,
																parent,
																pjoint);
				pjoint->setObjectUniversal(rjoint_universal);
				return 1;
				break;
			default:
				debug("Joint type not implemented.",LVL_ERR,L);
				return 0;
				break;
		}
		return 0;
		
		endifisvalid();
	}

	/*
	int RPhysicsManager::getMaterialHandler(lua_State* L)
	{
		ifistrue(objectExists);
		
		// code
		physics::RPhysicsMaterialHandler* handler = Luna < physics::RPhysicsMaterialHandler >::createNew(L);
		handler->setObject(obj->getMaterialHandler());
		return 1;
		
		endifisvalid();
	}
	*/
	
	int RPhysicsManager::setObject(Roket_PhysicsManager* object, core::RDevice* rd)
	{
		obj=object;
		roketdevice =rd;
		objectExists=true;
		return 0;
	}
	
	Roket_PhysicsManager* RPhysicsManager::getObject()
	{
		if (objectExists)
			return obj;
		else
			return NULL;
	}

	Roket_PhysicsBody* RPhysicsManager::checkBody(lua_State* L, int pos)
	{
		scene::RSceneNode* node = Luna<scene::RMeshSceneNode>::lightcheck(L,pos);
		if (node == NULL)
			node = Luna<scene::RTerrainSceneNode>::lightcheck(L,pos);
		if (node == NULL)
			node = Luna<scene::RSceneNode>::check(L,pos); // last one should use check to throw an actual error
		if (node != NULL)
			return node->physBody;
		else
			return NULL;
		/*
		lua_gettablevalue(L,pos+1,0);
		showstack(L);
		bool isvalidsceneelement = false;
		if (lua_multicheck(L, TYPE_RSCENENODE)!=NULL) isvalidsceneelement=true;
 		if (lua_multicheck(L, TYPE_RSCENEMESHNODE)!=NULL) isvalidsceneelement=true;
		if (lua_multicheck(L, TYPE_RSCENETERRAINNODE)!=NULL) isvalidsceneelement=true;
		
		// NOTE: This cannot be done via Luna::check because it checks multiple types.
		ifdebug(!isvalidsceneelement,INVALID_ARGUMENT(scene::RSceneNode::className,pos).c_str(),LVL_ERR,L);
		showstack(L);
		void* point = lua_touserdata(L, -1);
		if (point != NULL)
		{
			scene::RSceneNode *node = reinterpret_cast<scene::RSceneNode*>(point);
			return node->physBody;
		}
		else
		{
			return NULL;
		}
		*/
	}
}
