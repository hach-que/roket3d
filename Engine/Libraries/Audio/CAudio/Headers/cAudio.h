#ifndef CAUDIO_H_INCLUDED
#define CAUDIO_H_INCLUDED
#include <string>
#include <iostream>
#include "AL/al.h"
#include "AL/alut.h"
#include <vector>

#define BUFFER_SIZE ( 1024 * 32 )
#include "../include/IAudio.h"
#include "../export.h"
namespace cAudio
{
    class cAudio : public IAudio
    {
    public:
        EXPORTDLL void play(); //! play with defualts / the last set values
        EXPORTDLL void play2d(bool loop = false);//!plays the audio file 2d no distance.
        EXPORTDLL void play3d(bool loop = false, float x = 0.0, float y = 0.0, float z = 0.0, float soundstr = 1.0);//!plays the audio file and sets it to 3d
        EXPORTDLL void setPosition(float posx,float posy,float posz);//!allows us to set the position or reset the position
        EXPORTDLL void setVelocity(float velx,float vely,float velz);//!allows you to set the audio objects velocity
        EXPORTDLL void setDirection(float dirx,float diry,float dirz);//!allows us to set the direction the audio should play in / move
        EXPORTDLL void setPitch(float pitch); //! Sets the audios pitch level
        EXPORTDLL void setStrength(float soundstrength);//!allows us to set and reset the sound strenght
        EXPORTDLL void setVolume(float volume);//!Set the volume
        EXPORTDLL void setDopplerStrength(float doop);//!Set the doppler strength
        EXPORTDLL void setDopplerVelocity(float doopx,float doopy,float doopz);//!Set doppler velocity
        EXPORTDLL void seek(float secs);//!Seek the audio stream

        EXPORTDLL void release();//!release the file handle
        EXPORTDLL void pause();//!pauses the audio file
        EXPORTDLL void loop(bool loop);//!controls altering of the looping to make it loop or not to.
        EXPORTDLL void stop();//!stops the audio file from playing

        EXPORTDLL bool playback();//!play file
        EXPORTDLL bool playing();//!check if source is playing
        EXPORTDLL bool update();//!update the stream
        EXPORTDLL bool isvalid();//!checks to make sure everything is ready to go

        EXPORTDLL cAudio(IAudioDecoder* decoder);//!the cAudio constructor
        EXPORTDLL ~cAudio();

    protected:

        EXPORTDLL void empty();//!empties the queue
        EXPORTDLL void check();//!checks openal error state

        EXPORTDLL bool stream(ALuint buffer);//!reloads a buffer
        EXPORTDLL std::string errorString(int code);//!stringify an error code

    private:
        ALuint buffers[3]; //! front and back buffers
        ALuint source; //! audio source
        IAudioDecoder* Decoder;

        bool streaming; //! if the file to be played is going to be streamed
        bool toloop;//!if the file is to loop
        bool playaudio;//!if the file should play
        bool pauseaudio;//!if to pause audio
        bool paused();
    };
}
#endif //! CAUDIO_H_INCLUDED
