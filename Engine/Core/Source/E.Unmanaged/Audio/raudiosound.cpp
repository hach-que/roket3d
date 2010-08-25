#include "roket3d.h"
#include "raudiosound.h"

namespace audio
{
	const char *RAudioSound::className = TYPE_RAUDIOSOUND;
	const Luna < RAudioSound >::FunctionType RAudioSound::Functions[] = {
		{"play", &RAudioSound::play},
  {"play2D", &RAudioSound::play2D},
  {"play3D", &RAudioSound::play3D},
  {"pause", &RAudioSound::pause},
  {"stop", &RAudioSound::stop},
  {"seek", &RAudioSound::seek},

  {"setPosition", &RAudioSound::setPosition},
  {"setVelocity", &RAudioSound::setVelocity},
  {"setDirection", &RAudioSound::setDirection},
  {"setPitch", &RAudioSound::setPitch},
  {"setVolume", &RAudioSound::setVolume},
  {"setStrength", &RAudioSound::setStrength},
  {"setLooping", &RAudioSound::setLooping},

  {"isPlaying", &RAudioSound::isPlaying},

  {"destroy", &RAudioSound::destroy},

  {0}
	};
	const Luna < RAudioSound >::PropertyType RAudioSound::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};

	RAudioSound::~RAudioSound()
	{
		if (isSoundSet)
		{
			isSoundSet=false;
#if ENABLE_AUDIO
			caudio_sound->release();
#endif
		}
	}
	
	RAudioSound::RAudioSound(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RAudioSounds.  You must collect one from an RAudioController using the create functions.",LVL_ERR,L);
		isSoundSet=false;
		isPrecious = true; // Lua should not destroy these.
#if ENABLE_AUDIO
		caudio_sound = NULL;
#endif
	}

#if ENABLE_AUDIO
	void RAudioSound::setAudioSound(cAudio::IAudio* cas)
	{
		isSoundSet = true;
		caudio_sound = cas;
	}
#endif

	void RAudioSound::setRoketDevice(core::RDevice* rd)
	{
		roketdevice = rd;
	}

#if ENABLE_AUDIO
	cAudio::IAudio* RAudioSound::getAudioSound()
	{
		if (isSoundSet)
			return caudio_sound;
		else
			return NULL;
	}
#endif

	// functions
	int RAudioSound::play(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		caudio_sound->play();
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::play2D(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		bool loop = getoptionalargument(L,1,false);
		caudio_sound->play2d(loop);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::play3D(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		bool loop = getoptionalargument(L,2,false);
		caudio_sound->play3d(loop,vector3d->getObject().X,vector3d->getObject().Y,vector3d->getObject().Z);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::pause(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		caudio_sound->pause();
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::stop(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		caudio_sound->stop();
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::seek(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		cNumber seconds = getmandatoryargument_cNumber(L,1);
		caudio_sound->seek(seconds);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	// settings
	int RAudioSound::setPosition(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		caudio_sound->setPosition(vector3d->getObject().X,vector3d->getObject().Y,vector3d->getObject().Z);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::setVelocity(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		caudio_sound->setVelocity(vector3d->getObject().X,vector3d->getObject().Y,vector3d->getObject().Z);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::setDirection(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		caudio_sound->setDirection(vector3d->getObject().X,vector3d->getObject().Y,vector3d->getObject().Z);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::setPitch(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		cNumber pitch = getmandatoryargument_cNumber(L,1);
		caudio_sound->setPitch(pitch);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::setStrength(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		cNumber strength = getmandatoryargument_cNumber(L,1);
		caudio_sound->setStrength(strength);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::setVolume(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		cNumber volume = getmandatoryargument_cNumber(L,1);
		caudio_sound->setVolume(volume);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::setDopplerStrength(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		cNumber strength = getmandatoryargument_cNumber(L,1);
		caudio_sound->setDopplerStrength(strength);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::setDopplerVelocity(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		core::RVector3D *vector3d = Luna<core::RVector3D>::check(L, 1);
		caudio_sound->setDopplerVelocity(vector3d->getObject().X,vector3d->getObject().Y,vector3d->getObject().Z);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	int RAudioSound::setLooping(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		bool loop = getmandatoryargument_bool(L,1);
		caudio_sound->loop(loop);
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	// get settings
	int RAudioSound::isPlaying(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);
		
		lua_pushboolean(L,caudio_sound->playing());
		return 1;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}

	// destroy
	int RAudioSound::destroy(lua_State* L)
	{
#if ENABLE_AUDIO
		ifistrue(isSoundSet);

		caudio_sound->release();
		caudio_sound = NULL;
		isSoundSet = false;
		return 0;
		
		endifisvalid();
#else
		debug("Audio is disabled in this build.", LVL_WARN, L);
		return 0;
#endif
	}
	
}
