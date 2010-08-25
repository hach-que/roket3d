#include "roket3d.h"
#include "rscenelazycamera.h"

namespace scene
{
	const char *RSceneLazyCamera::className = "RSceneLazyCamera";
	const Luna < RSceneLazyCamera >::FunctionType RSceneLazyCamera::Functions[] = {
  {"setZoomDistance", &RSceneLazyCamera::setZoomDistance},
  {"getZoomDistance", &RSceneLazyCamera::getZoomDistance},
  {"setCameraRotation", &RSceneLazyCamera::setCameraRotation},
  {"getCameraRotation", &RSceneLazyCamera::getCameraRotation},
  {"setCameraAngle", &RSceneLazyCamera::setCameraAngle},
  {"getCameraAngle", &RSceneLazyCamera::getCameraAngle},
  {"startCameraBasedOnMouse", &RSceneLazyCamera::startCameraBasedOnMouse},
  {"setCameraBasedOnMouse", &RSceneLazyCamera::setCameraBasedOnMouse},
  {"updatePlayerObject", &RSceneLazyCamera::updatePlayerObject},
  {"handleMoveLeft", &RSceneLazyCamera::handleMoveLeft},
  {"handleMoveRight", &RSceneLazyCamera::handleMoveRight},
  {"handleMoveForward", &RSceneLazyCamera::handleMoveForward},
  {"handleMoveBackward", &RSceneLazyCamera::handleMoveBackward},
		{0}
	};
	
	// lazy camera functions
	int RSceneLazyCamera::setZoomDistance(lua_State* L)	// set the zoom distance from camera to player
	{
		ifistrue(objectExists);
		
		cameraDistance = (cIrrNumber)lua_tonumber(L,1);
		camera->setPosition(sphericalXYZ(cameraOrbit,cameraAngle,cameraDistance)); 
		return 0;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::getZoomDistance(lua_State* L)	// get
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L, cameraDistance);
		return 1;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::setCameraRotation(lua_State* L)	// set the camera's rotation around the player object
	{
		ifistrue(objectExists);
		
		cameraOrbit = (cIrrNumber)lua_tonumber(L,1);
		camera->setPosition(sphericalXYZ(cameraOrbit,cameraAngle,cameraDistance)); 
		return 0;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::getCameraRotation(lua_State* L)	// get
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L, cameraOrbit);
		return 1;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::setCameraAngle(lua_State* L)	// set the camera's angle relative to the ground
	{
		ifistrue(objectExists);
		
		cameraAngle = (cIrrNumber)lua_tonumber(L,1);
		camera->setPosition(sphericalXYZ(cameraOrbit,cameraAngle,cameraDistance));
		return 0;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::getCameraAngle(lua_State* L)	// get
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L, cameraAngle);
		return 1;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::startCameraBasedOnMouse(lua_State* L)	// resets the temporary camera values
	{
		ifistrue(objectExists);
		
		lua_gettablevalue(L,2,1); double x = (double)lua_tonumber(L,-1); lua_pop(L,1);
		lua_gettablevalue(L,2,2); double y = (double)lua_tonumber(L,-1); lua_pop(L,1);
		cursorOld.X = x;
		cursorOld.Y = y;
		cursorDelta.X = 0;
		cursorDelta.Y = 0;
		cameraOrbitOld = cameraOrbit;
		cameraAngleOld = cameraAngle;
		camera->setPosition(sphericalXYZ(cameraOrbit,cameraAngle,cameraDistance));
		return 0;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::setCameraBasedOnMouse(lua_State* L)	// update the camera's position based on the
									// mouse position
	{
		ifistrue(objectExists);
		
		lua_gettablevalue(L,2,1); double x = (double)lua_tonumber(L,-1); lua_pop(L,1);
		lua_gettablevalue(L,2,2); double y = (double)lua_tonumber(L,-1); lua_pop(L,1);
		cursor.X = x;
		cursor.Y = y;
		cursorDelta.X = cursor.X - cursorOld.X;
		cursorDelta.Y = cursor.Y - cursorOld.Y;
		if(cursorDelta.Y > 100) cursorDelta.Y = 100;
		if(cursorDelta.Y < -100) cursorDelta.Y = -100;
		cameraOrbit = (int)(cameraOrbitOld + cursorDelta.X) % 360;
		cameraAngle = (int)(cameraAngleOld + cursorDelta.Y) % 360;
		if(cameraAngle > 88) cameraAngle = 88;
		if(cameraAngle < -88) cameraAngle = -88;
		camera->setPosition(sphericalXYZ(cameraOrbit,cameraAngle,cameraDistance));
		return 0;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::updatePlayerObject(lua_State* L)		// update a SceneNode's position and rotation
									// based on the player values in the camera
	{
		ifistrue(objectExists);
		
		// get player object
		lua_gettablevalue(L,2,0);
		bool invalid;
		// NOTE: This cannot be done via Luna::check because it checks multiple types.
		invalid=(luaL_testudata(L,-1,TYPE_RSCENEMESHNODE)==NULL && luaL_testudata(L,-1,TYPE_RSCENENODE)==NULL);
		ifdebug(invalid,INVALID_ARGUMENT(TYPE_RSCENEMESHNODE,1).c_str(),LVL_ERR,L);
		scene::RSceneNode *inode = *((scene::RSceneNode **)lua_touserdata(L, -1));
			
		lua_pop(L,1);
		
		inode->getNode()->setPosition(playerPosition);
		inode->getNode()->setRotation(playerRotation);
		camera->setPosition(cameraTarget);
		
		return 0;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::handleMoveLeft(lua_State* L)	// move the player (camera values) left.  you
								// should call updatePlayerObject after this
								// (although it's probably better to call it in
								//  the game's step event)
	{
		return 0;
	}
	
	int RSceneLazyCamera::handleMoveRight(lua_State* L)	// move right
	{
		return 0;
	}
	
	int RSceneLazyCamera::handleMoveForward(lua_State* L)	// move forward
	{
		ifistrue(objectExists);
		
		// player movement vector
		cIrrVector3D playerStep = sphericalXYZ(cameraOrbit,0,playerMoveSpeed);
		playerX -= playerStep.X;
		playerY -= playerStep.Y;
		playerZ -= playerStep.Z;
		
		// player rotation brain torture
		
		playerTurnTo = cameraOrbit - 90;
		if(playerTurnTo < 0) playerTurnTo += 360;
		if(playerTurnTo >= 360) playerTurnTo -= 360;
		
		if(playerCompass < 0) playerCompass = playerCompass + 360;
		if(playerCompass >= 360) playerCompass = playerCompass - 360;
		
		cIrrNumber playerTurnDir = 0;
		cIrrNumber playerTurnDelta = 0;
		
		if(playerCompass > playerTurnTo)
		{
			if(playerCompass - playerTurnTo < 180){
				playerTurnDir = -1;
				playerTurnDelta = playerCompass - playerTurnTo;
			} else {
				playerTurnDir = 1;
				playerTurnDelta = (360 - playerCompass) + playerTurnTo;
			}
		}
		
		if(playerCompass < playerTurnTo)
		{
			if(playerTurnTo - playerCompass < 180){
				playerTurnDir = 1;
				playerTurnDelta = playerTurnTo - playerCompass;
			} else {
				playerTurnDir = -1;
				playerTurnDelta = (360 - playerTurnTo) + playerCompass;
			}
		}
		
		cIrrNumber playerTurnAmount;
		
		if(playerTurnDelta < playerTurnSpeed){
			playerTurnAmount = playerTurnDelta;
		} else {
			playerTurnAmount = playerTurnSpeed;
		}
		playerCompass += (playerTurnAmount * playerTurnDir); 
		
		playerRotation = cIrrVector3D(0,playerCompass,0);
		playerPosition = cIrrVector3D(playerX,playerY,playerZ);
		cameraTarget = cIrrVector3D(playerX,playerY,playerZ);
		
		return 0;
		
		endifisvalid();
	}
	
	int RSceneLazyCamera::handleMoveBackward(lua_State* L)	// move backward
	{
		return 0;
	}
	
	// lazy camera operating functions
	cIrrVector3D RSceneLazyCamera::sphericalXYZ(cIrrNumber compassAngle, cIrrNumber elevationAngle, cIrrNumber radius)
	{
		compassAngle = compassAngle * -1;
		elevationAngle = elevationAngle * -1;
		elevationAngle = elevationAngle + 90;
		cIrrNumber x = radius * cos(compassAngle * PI/180.0f ) * sin(elevationAngle * PI/180.0f );
		cIrrNumber z = radius * sin(compassAngle * PI/180.0f ) * sin(elevationAngle * PI/180.0f );
		cIrrNumber y = radius * cos(elevationAngle * PI/180.0f );
		cIrrVector3D result;
		result.X = x;
		result.Y = y;
		result.Z = z;
		return result;
	}
	
	irr::core::stringw RSceneLazyCamera::int2string(int intToConvert)
	{
		char temp[50];
		sprintf(temp,"%d",intToConvert);
		return temp;
	}

	void RSceneLazyCamera::initalize(irr::scene::ISceneManager*& scenemgr, irr::scene::ISceneNode* node, int& id, const char*& name)
	{
		camera = scenemgr->addCameraSceneNode(node, sphericalXYZ(cameraOrbit,cameraAngle,cameraDistance), node->getPosition(),id);
		camera->RoketSetName(name);
		objectExists=true;
	}
	
	// standard functions
	RSceneLazyCamera::RSceneLazyCamera(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RSceneLazyCameras.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		cameraOrbit = 45; 
		cameraAngle = 0; 
		cameraDistance = 30; 
		cameraOrbitOld = 0; 
		cameraAngleOld = 0; 
		playerX = 0; 
		playerY = 0; 
		playerZ = 0; 
		playerCompass = 30; 
		playerTurnTo = 0; 
		playerTurnSpeed = 1; 
		playerMoveSpeed = 0.01;
	}
	
	int RSceneLazyCamera::setCamera(irr::scene::ICameraSceneNode* object)
	{
		camera=object;
		objectExists=true;
		return 0;
	}
	
	irr::scene::ICameraSceneNode* RSceneLazyCamera::getCamera()
	{
		if (objectExists)
			return camera;
		else
			return NULL;
	}
}
