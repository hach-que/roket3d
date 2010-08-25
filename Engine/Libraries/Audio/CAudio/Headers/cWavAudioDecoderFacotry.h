#ifndef CWAVAUDIODECODERFACOTRY_H_INCLUDED
#define CWAVAUDIODECODERFACOTRY_H_INCLUDED

#include "../include/IAudioDecoderFactory.h"
#include "cWavDecoder.h"
#include "export.h"


namespace cAudio
{

class cWavAudioDecoderFactory : public IAudioDecoderFactory
{
    public:
        EXPORTDLL cWavAudioDecoderFactory() {}
        EXPORTDLL ~cWavAudioDecoderFactory() {}

        EXPORTDLL IAudioDecoder* CreateAudioDecoder(IDataSource* stream)
        {
            return new cWavDecoder(stream);
        }
    protected:
    private:
};

};

#endif //! CWAVAUDIODECODERFACOTRY_H_INCLUDED
