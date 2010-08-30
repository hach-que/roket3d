#include "roket3d.h"
#include "E.Unmanaged/Core/eventreciever.h"
#include "E.Unmanaged/Core/rdevice.h"

namespace core
{
	const char *RDevice::className = TYPE_RDEVICE;
// 	const char *RDevice::className = "RDevice";
	const Luna < RDevice >::FunctionType RDevice::Functions[] = {
		{"getGUIEnvironment", &RDevice::getGUIEnvironment},
  {"getSceneManager", &RDevice::getSceneManager},
  {"getVideoDriver", &RDevice::getVideoDriver},
  {"getCursorControl", &RDevice::getCursorControl},
  {"getPhysicsManager", &RDevice::getPhysicsManager},
  {"getAudioController", &RDevice::getAudioController},
  {"getStep", &RDevice::getStep},
  {"isRunning", &RDevice::isRunning},
  {"close", &RDevice::close},
  {"setWindowCaption", &RDevice::setWindowCaption},
  {"setEventReceiver", &RDevice::setEventReceiver},
		{0}
	};
	const Luna < RDevice >::PropertyType RDevice::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RDevice::RDevice(lua_State* L, bool instantedByUser)
	{
		deviceExists = false;
		wantsToRun = false;

		core::RVector2D *vector2d = Luna<core::RVector2D>::check(L, 0);

		irr::core::dimension2d<irr::s32> window_dimensions;
		window_dimensions.Width = vector2d->getObjectAsDimension().Width;
		window_dimensions.Height = vector2d->getObjectAsDimension().Height;
		if (window_dimensions.Width < 1 || window_dimensions.Height < 1)
		{
			window_dimensions = irr::core::dimension2d<irr::s32>(640,480);
			debug(INTERNAL_ERROR("Invalid size specified for window.  Defaulting to 640x480").c_str(),LVL_WARN,L);
		}
		currentStep = 0;
		int bits = (int)getoptionalargument(L,1,16);
		bool fullscreen = getoptionalargument(L,2,false);
		bool stencilbuffer = getoptionalargument(L,3,false);
		bool vsync = getoptionalargument(L,4,false);

#if defined(WIN32) || defined(WIN64)
			irrdevice = irr::createDevice(irr::video::EDT_DIRECT3D9,
								window_dimensions,
								bits,
								fullscreen,
								stencilbuffer,
								vsync
						     );
			if (irrdevice==0x00000000)
			{
				// attempt to use DirectX 8
				irrdevice = irr::createDevice(irr::video::EDT_DIRECT3D8,
								window_dimensions,
								bits,
								fullscreen,
								stencilbuffer,
								vsync
						     );
			}
			/*
			if (irrdevice==0x00000000)
			{
				// attempt to use OpenGL
				irrdevice = irr::createDevice(irr::video::EDT_BURNINGSVIDEO,
								window_dimensions,
								bits,
								fullscreen,
								stencilbuffer,
								vsync
						     );
			}
			*/
			if (irrdevice==0x00000000)
			{
				debug(INTERNAL_ERROR("No suitable graphics engine found").c_str(),LVL_ERR,L);
			}
#else
			irrdevice = irr::createDevice(irr::video::EDT_OPENGL,
								window_dimensions,
								bits,
								fullscreen,
								stencilbuffer,
								vsync
						     );
#endif
		if (irrdevice!=0x00000000)
		{
			deviceExists = true;
			wantsToRun = true;
			shadowsSetStencil = stencilbuffer;
			// setup video driver stuff
			transformation_matrix = irr::core::matrix4();
			current_material = irr::video::SMaterial();
			// create our physics device
			physdevice = new physics::Roket_PhysicsManager();
#if ENABLE_AUDIO
			audio_controller = cAudio::getAudioManager();
			char *trick[1];
			trick[0] = "test";
			audio_controller->init(1, trick);
#endif
			irrdevice->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);
			irr::core::dimension2d<irr::s32> ScreenRTT =	
				// Comment JUST the next line if "getVendorInfo()" is not defined.
				!irrdevice->getVideoDriver()->getVendorInfo().equals_ignore_case("NVIDIA Corporation") ? irr::core::dimension2d<irr::s32>(512,512) : 
				irrdevice->getVideoDriver()->getScreenSize();
			xeffect = new effectHandler(irrdevice, irr::core::dimension2d<irr::s32>(4096,4096), "shaders", ScreenRTT);
		}
		else
		{
			debug(INTERNAL_ERROR("Unable to create the graphical device").c_str(),LVL_ERR,L);
		}
	}
	
	RDevice::~RDevice()
	{
		if (deviceExists)
		{
			delete physdevice;
#if ENABLE_AUDIO
			audio_controller->release();
			audio_controller->shutDown();
			audio_controller = NULL;
#endif
			irrdevice->closeDevice();
			irrdevice->drop();
			deviceExists=false;
		}
	}

	int RDevice::getGUIEnvironment(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);
		
		gui::RGUIEnvironment* env = Luna < gui::RGUIEnvironment >::createNew(L);
		env->setEnv(irrdevice->getGUIEnvironment());
		return 1;
		
		endifisvalid();
	}
	
	int RDevice::getPhysicsManager(lua_State* L)
	{
		ifistrue(deviceExists && (physdevice!=0) && wantsToRun);
		
		physics::RPhysicsManager* phys = Luna < physics::RPhysicsManager >::createNew(L);
		phys->setObject(physdevice, this);
		return 1;
		
		endifisvalid();
	}

	physics::Roket_PhysicsManager* RDevice::getCPhysicsManager()
	{
		return physdevice;
	}

	int RDevice::getCursorControl(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);
		
		core::RCursorControl* cur = Luna < core::RCursorControl >::createNew(L);
		cur->setObject(irrdevice->getCursorControl());
		return 1;
		
		endifisvalid();
	}
	
	int RDevice::getSceneManager(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);
		
		scene::RSceneManager* scn = Luna < scene::RSceneManager >::createNew(L);
		scn->setScene(irrdevice->getSceneManager());
		scn->setCPhysicsDevice(physdevice);
		scn->setCIrrlichtDevice(irrdevice);
		scn->setCRoketDevice(this);
		scn->setCXEffect(xeffect);
		return 1;
		
		endifisvalid();
	}

	int RDevice::getAudioController(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);
		
		audio::RAudioController* aud = Luna < audio::RAudioController >::createNew(L);
#if ENABLE_AUDIO
		aud->setAudioController(audio_controller);
#else
		aud->setAudioController();
#endif
		aud->setCIrrlichtDevice(irrdevice);
		aud->setCRoketDevice(this);
		return 1;
		
		endifisvalid();
	}
	
	int RDevice::getStep(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);
		
		lua_pushnumber(L,currentStep);
		return 1;
		
		endifisvalid();
	}
	
	int RDevice::getVideoDriver(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);
		
		video::RVideoDriver* drv = Luna < video::RVideoDriver >::createNew(L);
		drv->setVideoDriver(irrdevice->getVideoDriver());
		drv->setCRoketDevice(this);
		drv->setCXEffect(xeffect);
		return 1;
		
		endifisvalid();
	}
	
	void RDevice::runDevice()
	{
		if (deviceExists && wantsToRun && irrdevice!=0)
		{
			irrdevice->yield();
			wantsToRun = irrdevice->run();
			irrdevice->yield();
		}
	}
	
	int RDevice::isRunning(lua_State* L)
	{
		if (deviceExists)
		{
			currentStep+=1;
			lua_pushboolean(L,irrdevice->run());
			return 1;
		}
		else
		{
			lua_pushboolean(L,false);
			return 1;
		}
	}
	
	int RDevice::setEventReceiver(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);

		// Get object
		// TODO: Check argument check is correct.
		RoketEventReceiver *revent = Luna<RoketEventReceiver>::check(L, 1);
		irrdevice->setEventReceiver(revent);
		
		return 0;
		
		endifisvalid();
	}
	
	int RDevice::close(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);
		
		delete physdevice;
#if ENABLE_AUDIO
		audio_controller->release();
		audio_controller->shutDown();
#endif
		irrdevice->closeDevice();
		irrdevice->drop();
		deviceExists=false;
		wantsToRun=false;
		
		return 0;
		
		endifisvalid();
	}
	
	int RDevice::setWindowCaption(lua_State* L)
	{
		ifistrue(deviceExists && wantsToRun);
		
		// TODO: Check argument check is correct.
		irrdevice->setWindowCaption(ConvertToWideChar(getmandatoryargument_constchar(L,1)));
		return 0;
		
		endifisvalid();
	}
	
	irr::IrrlichtDevice* RDevice::getIrrlichtDevice()
	{
		if (deviceExists && wantsToRun)
			return irrdevice;
		else
			return 0;
	}

	void RDevice::setShaderLocation(const char* newLocation)
	{
		shaderLocation = newLocation;
	}

	irr::core::stringc RDevice::getShaderLocation()
	{
		return shaderLocation;
	}

	void RDevice::setStencilStatus(bool stencil)
	{
		shadowsSetStencil = stencil;
	}

	bool RDevice::getStencilStatus()
	{
		return shadowsSetStencil;
	}
}
