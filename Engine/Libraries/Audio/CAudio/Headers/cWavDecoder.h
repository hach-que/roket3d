#ifndef CWAVDECODER_H_INCLUDED
#define CWAVDECODER_H_INCLUDED

#include "../include/IAudioDecoder.h"
#include <iostream>
#include "CAudioExport.h"

namespace cAudio
{

    class cWavDecoder : public IAudioDecoder
    {
        public:

            EXPORTDLL cWavDecoder(IDataSource* stream);
            EXPORTDLL ~cWavDecoder();

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

        private:
            int mChunkSize;
            int mSubChunk1Size;
            short mFormat;
            short mChannels;
            int mSampleRate;
            int mByteRate;
            short mBlockAlign;
            short mBitsPerSample;
            int mDataSize;


    };

}

#endif //! CFLACDECODER_H_INCLUDED

