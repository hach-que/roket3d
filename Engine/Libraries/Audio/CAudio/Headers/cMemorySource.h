#ifndef CMEMORYSOURCE_H
#define CMEMORYSOURCE_H

#include "../include/IDataSource.h"
#include "export.h"

namespace cAudio
{

//!Class used to read from a memory source.
//!If copy is true, then cMemorySource will make a copy of the data.
//!Otherwise, cMemorySource will take care of deleting the data array for you.
class cMemorySource : public IDataSource
{
    public:
        EXPORTDLL cMemorySource(const void* data, int size, bool copy);
        EXPORTDLL ~cMemorySource();

        //!Returns whether the source is valid (in case of an error, like the file couldn't be found)
        EXPORTDLL virtual bool isValid();

        //!Get the current location in the data stream
        EXPORTDLL virtual int getCurrentPos();

        //!Get the total size of the data stream
        EXPORTDLL virtual int getSize();

        //!Read out a section of the data stream
        EXPORTDLL virtual int read(void* output, int size);

        //!Seek to a position in the data stream
        EXPORTDLL virtual bool seek(int amount, bool relative);
    protected:
        char* Data;
        int Size;
        bool Valid;
        int Pos;
    private:
};

};

#endif //! CMEMORYSOURCE_H
