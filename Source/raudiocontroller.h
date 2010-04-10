#ifndef HEADER_RAUDIOCONTROLLER
#define HEADER_RAUDIOCONTROLLER
namespace audio
{
	class RAudioController : public RObject
	{
#if ENABLE_AUDIO
		cAudio::IAudioManager* aud;
#endif
		irr::IrrlichtDevice* irrdevice;
		core::RDevice* roketdevice;
		
		public:
			RAudioController(lua_State* L, bool instantedByUser);
#if ENABLE_AUDIO
			int setAudioController(cAudio::IAudioManager* audio);
			cAudio::IAudioManager* getAudioController();
#else
			int setAudioController();
			void getAudioController();
#endif
			int setListenerPosition(lua_State* L);
			int setListenerOrientation(lua_State* L);
			int createFromFile(lua_State* L);
			int update(lua_State* L);

			void setCIrrlichtDevice(irr::IrrlichtDevice* idevice);
			void setCRoketDevice(core::RDevice* rd);

			static const char *className;
			static const Luna < RAudioController >::FunctionType Functions[];
			static const Luna < RAudioController >::PropertyType Properties[];
			bool isAudioSet;
	};

}
#endif
