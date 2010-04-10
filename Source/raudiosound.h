#ifndef HEADER_RAUDIOSOUND
#define HEADER_RAUDIOSOUND

namespace audio
{
	class RAudioSound : public RObject
	{
		// Newton Physics Objects
		bool isSoundSet;
		core::RDevice* roketdevice;

		public:
#if ENABLE_AUDIO
			cAudio::IAudio* caudio_sound;
#endif
			RAudioSound(lua_State* L, bool instantedByUser);
			~RAudioSound();
#if ENABLE_AUDIO
			virtual void setAudioSound(cAudio::IAudio* cas);
			virtual cAudio::IAudio* getAudioSound();
#endif
			int play(lua_State* L);
			int play2D(lua_State* L);
			int play3D(lua_State* L);
			int pause(lua_State* L);
			int stop(lua_State* L);
			int seek(lua_State* L);

			int setPosition(lua_State* L);
			int setVelocity(lua_State* L);
			int setDirection(lua_State* L);
			int setPitch(lua_State* L);
			int setVolume(lua_State* L);
			int setStrength(lua_State* L);
			int setLooping(lua_State* L);
			int setDopplerStrength(lua_State* L);
			int setDopplerVelocity(lua_State* L);

			int isPlaying(lua_State* L);

			int destroy(lua_State* L);

			void setRoketDevice(core::RDevice* rd);

			static const char *className;
			static const Luna < RAudioSound >::FunctionType Functions[];
			static const Luna < RAudioSound >::PropertyType Properties[];
	};
}
#endif
