#ifndef CFILESOURCE_H
#define CFILESOURCE_H

#include "../include/IDataSource.h"
#include <string>
#include <iostream>
#include <fstream>
#include "CAudioExport.h"

namespace cAudio
{

class cFileSource : public IDataSource
{
    public:
        EXPORTDLL cFileSource(const std::string& filename);
        EXPORTDLL ~cFileSource();

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
        std::ifstream File;
        bool Valid;
        int Filesize;
    private:
         FILE*   pFile;
};

};

#endif //! CFILESOURCE_H
