#ifndef IAUDIODECODER_H
#define IAUDIODECODER_H

#include "IDataSource.h"

namespace cAudio
{

enum AudioFormats
{
    EAF_8BIT_MONO = 0x1100,
    EAF_8BIT_STEREO = 0x1102,
    EAF_16BIT_MONO = 0x1101,
    EAF_16BIT_STEREO = 0x1103,
    EAF_COUNT
};

class IAudioDecoder
{
    public:
        IAudioDecoder(IDataSource* stream) : Stream(stream) {}
        virtual ~IAudioDecoder() { delete Stream; }

        //!Returns the format of the audio data
        virtual AudioFormats getFormat() = 0;

        //!Returns the frequency of the audio data
        virtual int getFrequency() = 0;

        //!Returns whether seeking is supported
        virtual bool isSeekingSupported() = 0;

        //!Reads a section of data out of the audio stream
        virtual int readAudioData(void* output, int amount) = 0;

        //!Sets the position to read data out of
        virtual bool setPosition(int position, bool relative) = 0;

        //!If seeking is supported, will seek the stream to seconds
        virtual bool seek(int seconds, bool relative) = 0;
    protected:
        IDataSource* Stream;
};

};

#endif //! IAUDIODECODER_H
