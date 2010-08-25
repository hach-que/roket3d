#ifndef COGGAUDIODECODERFACTORY_H
#define COGGAUDIODECODERFACTORY_H

#include "../include/IAudioDecoderFactory.h"
#include "cOggDecoder.h"
#include "../export.h"

namespace cAudio
{

class cOggAudioDecoderFactory : public IAudioDecoderFactory
{
    public:
        EXPORTDLL cOggAudioDecoderFactory() {}
        EXPORTDLL ~cOggAudioDecoderFactory() {}

        EXPORTDLL IAudioDecoder* CreateAudioDecoder(IDataSource* stream)
        {
            return new cOggDecoder(stream);
        }
    protected:
    private:
};

};

#endif //! COGGAUDIODECODERFACTORY_H
