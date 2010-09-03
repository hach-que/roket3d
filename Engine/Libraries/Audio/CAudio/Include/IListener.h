
#ifndef ILISTENER_H
#define ILISTENER_H
#include "CAudioExport.h"

namespace cAudio
{
    class IListener
    {
    public:
        virtual ~IListener() {}

        virtual void setPosition(ALfloat x,ALfloat y, ALfloat z) = 0;

        virtual void setOrientation(ALfloat x,ALfloat y,ALfloat z,ALfloat upx,ALfloat upy,ALfloat upz) = 0;
    protected:
        IListener() {}
    private:
    };
}
#endif //! ILISTENER_H
