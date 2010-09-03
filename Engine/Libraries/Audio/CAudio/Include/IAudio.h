#ifndef IAUDIO_H
#define IAUDIO_H

#include "CAudioExport.h"
#include "IAudioDecoder.h"

namespace cAudio
{
    class IAudio
    {
    public:
        IAudio() {}
        virtual ~IAudio() {}

        virtual void play() = 0; //! play with defualts / the last set values
        virtual void play2d(bool loop = false) = 0;//!plays the audio file 2d no distance.
        virtual void play3d(bool loop = false, float x = 0.0, float y = 0.0, float z = 0.0, float soundstr = 1.0) = 0;//!plays the audio file and sets it to 3d
        virtual void setPosition(float posx,float posy,float posz) = 0;//!allows us to set the position or reset the position
        virtual void setVelocity(float velx,float vely,float velz) = 0;//!allows you to set the audio objects velocity
        virtual void setDirection(float dirx,float diry,float dirz) = 0;//!allows us to set the direction the audio should play in / move
        virtual void setPitch(float pitch) = 0; //! Sets the audios pitch level
        virtual void setStrength(float soundstrength) = 0;//!allows us to set and reset the sound strenght
        virtual void setVolume(float volume) = 0; //! Set the volume
        virtual void setDopplerStrength(float doop) = 0; //!Set the doppler strength
        virtual void setDopplerVelocity(float doopx,float doopy,float doopz) = 0;//!Set the doppler velocity
        virtual void seek(float secs) = 0; //!Seek through the audio stream

        virtual void release() = 0;//!release the file handle
        virtual void pause() = 0;//!pauses the audio file
        virtual void loop(bool loop) = 0;//!controls altering of the looping to make it loop or not to.
        virtual void stop() = 0;//!stops the audio file from playing

        virtual bool playback() = 0;//!play file
        virtual bool playing() = 0;//!check if source is playing
        virtual bool update() = 0;//!update the stream
        virtual bool isvalid() = 0;//!checks to make sure everything is ready to go
    protected:
    private:
    };
}
#endif //! IAUDIO_H
