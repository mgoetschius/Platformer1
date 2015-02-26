#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>

#include "audiere.h"

class AudioPlayer
{
    public:
        AudioPlayer();
        void play(int index);
        virtual ~AudioPlayer();
        void AddSound(const char* filename, bool streaming, bool repeating);
    protected:
    private:
        audiere::AudioDevicePtr device;
        audiere::OutputStreamPtr sound;
        std::vector<audiere::OutputStreamPtr> sounds;
};

#endif // AUDIOPLAYER_H
