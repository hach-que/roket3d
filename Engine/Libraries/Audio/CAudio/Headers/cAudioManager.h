#ifndef CAUDIOMANAGER_H_INCLUDED
#define CAUDIOMANAGER_H_INCLUDED
#include <map>
#include <string>
#include <vector>
#include "cAudio.h"
#include "../include/IAudioDecoderFactory.h"
#include "cListener.h"
#include "../include/IAudioManager.h"
#include "CAudioExport.h"

namespace cAudio
{
	class IAudio;

    class cAudioManager : public IAudioManager
    {
    public:
        EXPORTDLL void init(int argc,char* argv[]); //!Inits the audio manager calling the alut/etc start ups
        EXPORTDLL void shutDown();//!Shuts everything down
        EXPORTDLL IAudio* createFromFile(const std::string& identifier,const std::string& file,bool stream = false);//!Creates the cAudio object
        EXPORTDLL IAudio* createFromMemory(const std::string& identifier, const char* data, size_t length, std::string ext);//!Loads ogg from memory or virtual file system
        EXPORTDLL void registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension);
        EXPORTDLL void setListenerPos(float x,float y,float z);//!Allows you to set the listener position
        EXPORTDLL void setListenerOrientation(float ux,float uy,float uz);
        EXPORTDLL void update();//!Updates the cAudio playback
        EXPORTDLL IAudio *getSound(std::string identifier);//!Gets you the cAudio object you wnat
        EXPORTDLL void release();//!Releases "ALL" cAudio objects

        EXPORTDLL static cAudioManager* Instance()
        {
            return &m_cAudioManager;
        }

    protected:
        EXPORTDLL cAudioManager(){ }

    private:
        static cAudioManager m_cAudioManager;
        std::map<std::string, IAudio*> audiomap; //!The map that holds the cAudio objects
        std::map<std::string, IAudioDecoderFactory*> decodermap; //!Decoder map that holds all decoders by file extension
        cListener initlistener;//!The listener object
        float listenerX;
        float listenerY;
        float listenerZ;
    };
}

#endif //! CAUDIOMANAGER_H_INCLUDED
