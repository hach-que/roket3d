#include "roket3d.h"
#include "E.Unmanaged/Audio/raudiocontroller.h"

namespace audio
{
	const char *RAudioController::className = TYPE_RAUDIOCONTROLLER;
	const Luna < RAudioController >::FunctionType RAudioController::Functions[] = {
		{"setListenerPosition", &RAudioController::setListenerPosition},
  {"setListenerOrientation", &RAudioController::setListenerOrientation},
  {"createFromFile", &RAudioController::createFromFile},
  {"update", &RAudioController::update},
  {0}
	};
	const Luna < RAudioController >::PropertyType RAudioController::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RAudioController::RAudioController(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RAudioControllers.  You must collect one from an RDevice.",LVL_ERR,L);
		isAudioSet=false;
	}

#if ENABLE_AUDIO
	int RAudioController::setAudioController(cAudio::IAudioManager* audio)
	{
		aud = audio;
		isAudioSet=true;
		return 0;
	}
	
	cAudio::IAudioManager* RAudioController::getAudioController()
	{
		if (isAudioSet)
			return aud;
		else
			return NULL;
	}
#else
	int RAudioController::setAudioController()
	{
		return 0;
	}
	
	void RAudioController::getAudioController()
	{
	}
#endif

	int RAudioController::setListenerPosition(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isAudioSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		aud->setListenerPos(vector3d->getObject().X,vector3d->getObject().Y,vector3d->getObject().Z);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioController::setListenerOrientation(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isAudioSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		aud->setListenerOrientation(vector3d->getObject().X,vector3d->getObject().Y,vector3d->getObject().Z);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioController::createFromFile(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isAudioSet);
		
		const char* identifier = getmandatoryargument_constchar(L,1);
		const char* filename = getmandatoryargument_constchar(L,2);
		bool stream = getoptionalargument(L,3,false);
		
		if (stream)
		{
			debug("Loading " << filename << " as stream.",LVL_INFO,L);
		}
		else
		{
			debug("Loading " << filename << " as full file.",LVL_INFO,L);
		}

		cAudio::IAudio* caud = aud->createFromFile(identifier,filename,stream);
		if (caud == NULL)
		{
			debug("Could not create sound object.  Make sure " << filename << " exists.",LVL_ERR,L);
			return 0;
		}

		audio::RAudioSound* raudio = Luna < audio::RAudioSound >::createNew(L);
		raudio->setAudioSound(caud);
		raudio->setRoketDevice(roketdevice);
		return 1;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioController::update(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isAudioSet);
		
		aud->update();
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	void RAudioController::setCIrrlichtDevice(irr::IrrlichtDevice* idevice)
	{
		irrdevice = idevice;
	}

	void RAudioController::setCRoketDevice(core::RDevice* rd)
	{
		roketdevice = rd;
	}
}
