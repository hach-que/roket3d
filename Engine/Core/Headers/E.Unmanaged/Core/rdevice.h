#ifndef HEADER_RDEVICE
#define HEADER_RDEVICE

#include "E.Unmanaged/Physics/Internal/roket_physicsmanager.h"

/*namespace Engine
{
	namespace Unmanaged
	{*/
		namespace core
		{
			class RDevice : public RObject
			{
				irr::IrrlichtDevice* irrdevice;
				physics::Roket_PhysicsManager* physdevice; // our custom system
#if ENABLE_AUDIO
		cAudio::IAudioManager* audio_controller;
#endif
				int lastFPS;
				bool deviceExists;
				bool wantsToRun;
				void runDevice();

				// settings
				irr::core::stringc shaderLocation;
				bool shadowsSetStencil;

				public:
					// video stuff
					irr::core::matrix4 transformation_matrix;
					irr::video::SMaterial current_material;
				
					int currentStep;
					// allow access to XEffects
					effectHandler* xeffect;

					RDevice(lua_State* L, bool instantedByUser);
					~RDevice();
					int getGUIEnvironment(lua_State* L);
					int getPhysicsManager(lua_State* L);
					int getSceneManager(lua_State* L);
					int getCursorControl(lua_State* L);
					int getVideoDriver(lua_State* L);
					int getAudioController(lua_State* L);
					
					irr::IrrlichtDevice* getIrrlichtDevice();
					int isRunning(lua_State* L);
					int close(lua_State* L);
					int setWindowCaption(lua_State* L);
					int setEventReceiver(lua_State* L);
					
					int getStep(lua_State* L);

					// shader settings (called by RSceneSpecialEffects)
					void setShaderLocation(const char* newLocation);
					irr::core::stringc getShaderLocation();
					void setStencilStatus(bool stencil);
					bool getStencilStatus();

					// c++ functions
					physics::Roket_PhysicsManager* getCPhysicsManager();
					
					static const char *className;
					static const Luna < RDevice >::FunctionType Functions[];
					static const Luna < RDevice >::PropertyType Properties[];
			};
//		}
//	}
}
#endif
