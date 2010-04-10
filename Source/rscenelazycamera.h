#ifndef HEADER_RSCENELAZYCAMERA
#define HEADER_RSCENELAZYCAMERA
namespace scene
{
	class RSceneLazyCamera : public RObject
	{
		private:
			// mouse registry
			bool mouseDownL;
			bool mouseDownM;
			bool mouseDownR;
			cIrrNumber lastWheelMovement;
			irr::core::position2d<cIrrNumber> cursor;
			irr::core::position2d<cIrrNumber> cursorOld;
			irr::core::position2d<cIrrNumber> cursorDelta;
			
			 // camera registry 
			cIrrNumber cameraOrbit;
			cIrrNumber cameraAngle;
			cIrrNumber cameraDistance;
			cIrrNumber cameraOrbitOld;
			cIrrNumber cameraAngleOld;
			irr::scene::ICameraSceneNode* camera;
			
			// player registry 
			cIrrNumber playerX;
			cIrrNumber playerY;
			cIrrNumber playerZ;
			cIrrNumber playerCompass;
			cIrrNumber playerTurnTo;
			cIrrNumber playerTurnSpeed;
			cIrrNumber playerMoveSpeed;
			
			// vectors
			cIrrVector3D playerRotation;
			cIrrVector3D playerPosition;
			cIrrVector3D cameraTarget;
			
			bool objectExists;
			
		
		
		
		
		public:
			RSceneLazyCamera(lua_State* L, bool instantedByUser);
			void initalize(irr::scene::ISceneManager*& scenemgr, irr::scene::ISceneNode* node, int& id, const char*& name);
			int setCamera(irr::scene::ICameraSceneNode* object);
			irr::scene::ICameraSceneNode* getCamera();


			// lazy camera functions
			int setZoomDistance(lua_State* L);
			int getZoomDistance(lua_State* L);
			//int setAngleLimits(lua_State* L);
			//int getAngleLimits(lua_State* L);
			int setCameraRotation(lua_State* L);
			int getCameraRotation(lua_State* L);
			int setCameraAngle(lua_State* L);
			int getCameraAngle(lua_State* L);
			int startCameraBasedOnMouse(lua_State* L);
			int setCameraBasedOnMouse(lua_State* L);
			int updatePlayerObject(lua_State* L);
			int handleMoveLeft(lua_State* L);
			int handleMoveRight(lua_State* L);
			int handleMoveForward(lua_State* L);
			int handleMoveBackward(lua_State* L);
			
			
			// lazy camera operating functions
			cIrrVector3D sphericalXYZ(cIrrNumber compassAngle, cIrrNumber elevationAngle, cIrrNumber radius);
			irr::core::stringw int2string(int intToConvert);
			
			static const char *className;
			static const Luna < RSceneLazyCamera >::FunctionType Functions[];
	};
}
#endif
