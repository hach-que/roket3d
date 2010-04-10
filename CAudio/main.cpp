#include <iostream>
#include <fstream>
#include "include/IAudioManager.h"
#include "include/IAudio.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout <<"Hello motto\n";

    cAudio::IAudioManager* manager = cAudio::getAudioManager();
    manager->init(argc,argv);

    cAudio::IAudio* mysound = manager->createFromFile("test","1.ogg",true);

    mysound->play2d(true);

    while(mysound->playing()){
        manager->update();
    }

    manager->release();
    manager->shutDown();
    return 0;
}
