
#ifndef IAUDIOMANAGER_H
#define IAUDIOMANAGER_H
#include "export.h"
#include <string>

#ifdef COMPILE_FOR_WINDOWS
#ifndef CAUDIO_STATIC_LIB
#ifdef CAUDIO_EXPORTS
#define CAUDIO_API __declspec(dllexport)
#else
#define CAUDIO_API __declspec(dllimport)
#endif //! CAUDIO_API_EXPORT
#else
#define CAUDIO_API
#endif  _CAUDIO_API_STATIC_LIB_
#else
#define CAUDIO_API
#endif
namespace cAudio
{
	class IAudio;
	class IAudioDecoderFactory;

    class IAudioManager
    {
    public:
		virtual void init(int argc,char* argv[]) = 0; //!Inits the audio manager calling the alut/etc start ups
        virtual void shutDown() = 0;//!Shuts everything down
        virtual IAudio* createFromFile(const std::string& identifier,const std::string& file,bool stream = false) = 0;//!Creates the cAudio object
        virtual IAudio* createFromMemory(const std::string& identifier,const char* data, size_t length, std::string ext) = 0;
        virtual void registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension) = 0;
        virtual void setListenerPos(float x,float y,float z) = 0;//!Allows you to set the listener position
        virtual void setListenerOrientation(float ux,float uy,float uz) = 0;//!set the listeners orientation
        virtual void update() = 0;//!Updates the cAudio playback
        virtual IAudio *getSound(std::string identifier) = 0;//!Gets you the cAudio object you wnat
        virtual void release() = 0;
        virtual ~IAudioManager() {}
    protected:
    private:
    };

    CAUDIO_API IAudioManager* getAudioManager(void);
}
#endif //! IAUDIOMANAGER_H
