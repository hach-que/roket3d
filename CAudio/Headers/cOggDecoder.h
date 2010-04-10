#ifndef COGGDECODER_H_INCLUDED
#define COGGDECODER_H_INCLUDED

#include "../include/IAudioDecoder.h"
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <iostream>
#include "../export.h"

namespace cAudio
{

    class cOggDecoder : public IAudioDecoder
    {
        public:

            EXPORTDLL cOggDecoder(IDataSource* stream);
            EXPORTDLL ~cOggDecoder();

            //!Retruns the format of the audio data
            EXPORTDLL virtual AudioFormats getFormat();

            //!Returns the frequency of the audio data
            EXPORTDLL virtual int getFrequency();

            //!Returns whether seeking is supported
            EXPORTDLL virtual bool isSeekingSupported();

            //!Reads a section of data out of the audio stream
            EXPORTDLL virtual int readAudioData(void* output, int amount);

            //!Sets the position to read data out of
            EXPORTDLL virtual bool setPosition(int position, bool relative);

            //!If seeking is supported, will seek the stream to seconds
            EXPORTDLL virtual bool seek(int seconds,bool relative);

        protected:
            ov_callbacks vorbisCallbacks; //! callbacks used for read memory
            vorbis_info*    vorbisInfo;//!some formatting data
            vorbis_comment* vorbisComment;//!user comments
            OggVorbis_File oggStream; //! stream handle
            bool seekable;
    };

}

#endif //! COGGDECODER_H_INCLUDED
