#ifndef IAUDIODECODERFACTORY_H
#define IAUDIODECODERFACTORY_H

#include "CAudioExport.h"
#include "IAudioDecoder.h"

namespace cAudio
{

class IAudioDecoderFactory
{
    public:
        IAudioDecoderFactory() {}
        virtual ~IAudioDecoderFactory() {}

        virtual IAudioDecoder* CreateAudioDecoder(IDataSource* stream) = 0;
    protected:
    private:
};

};

#endif //! IAUDIODECODERFACTORY_H
