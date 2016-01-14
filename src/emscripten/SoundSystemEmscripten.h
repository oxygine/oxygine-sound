#pragma once
#include "SoundSystem.h"
#include "ChannelEmscripten.h"
#include "Channels.h"

namespace oxygine
{
    class Sound;
    class Channel;

    class SoundSystemEmscripten : public SoundSystem
    {
    public:
        SoundSystemEmscripten();
        ~SoundSystemEmscripten();

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

    private:
        Channels<ChannelEmscripten> _channels;
    };
}