#pragma once
#include "../SoundSystem.h"
#include "Channel_oal.h"
#include "Channels.h"

namespace oxygine
{


    class SoundSystemOAL : public SoundSystem
    {
    public:
        SoundSystemOAL();
        ~SoundSystemOAL();

        void init(int channels_num);
        void release();

        void pause();
        void resume();
        void stop();

        SoundOAL* createSound(std::vector<unsigned char>& buffer, bool swap);
        SoundOAL* createSound(const char* path);

        bool        isAvailable() const {return _context != 0;}
        float       getVolume() const {return _volume;}
        int         getFreeChannelsNum();
        ChannelOAL* getFreeChannel();

        void setVolume(float v);
        void setContext();

        void update();

    private:
        //typedef std::vector<ChannelOAL> channels;
        //channels _channels;

        Channels<ChannelOAL> _channels;

        //channels _pausedChannels;

        float _volume;

        ALCdevice* _device;
        ALCcontext* _context;
    };
}