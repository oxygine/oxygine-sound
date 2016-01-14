#pragma once
#include "SoundSystem.h"

namespace oxygine
{
    class Sound;
    class Channel;

    class SoundSystemNull : public SoundSystem
    {
    public:
        void init(int channels);
        void release();
        void pause();
        void resume();
        void stop();


        Sound* createSound(std::vector<unsigned char>& data, bool swap);
        Sound* createSound(const char* file);


        Channel*    getFreeChannel();
        float       getVolume() const;

        void setVolume(float);
        void update();
    };
}