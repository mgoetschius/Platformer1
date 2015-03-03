#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
    device = audiere::OpenDevice();
    AddSound("./res/audio/drop.wav", false, false);
    AddSound("./res/audio/ding.wav", false, false);
}

void AudioPlayer::AddSound(const char* filename, bool streaming, bool repeating)
{
    sound = audiere::OpenSound(device, filename, streaming);
    sound->setRepeat(repeating);
    sound->setVolume(0.5);
    sounds.push_back(sound);
}

void AudioPlayer::play(int index)
{
    sounds[index]->play();
}


AudioPlayer::~AudioPlayer()
{
    //dtor
}
