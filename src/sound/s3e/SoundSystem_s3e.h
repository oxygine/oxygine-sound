#pragma once
#include "SoundSystem.h"
#include "Channel_s3e.h"
namespace oxygine
{
    typedef std::vector<ChannelS3E*> channels;
    class SoundSystemS3E : public SoundSystem
    {
    public:
        SoundSystemS3E();
        ~SoundSystemS3E();

        void init(int channels_num);
        void release();

        void pause();
        void resume();

        SoundS3E* createSound(std::vector<unsigned char>& buffer, bool swap);
        SoundS3E* createSound(const char* path);

        bool        isAvailable() const {return true;}
        float       getVolume() const {return _volume;}
        int         getFreeChannelsNum();
        ChannelS3E* getFreeChannel();

        void setVolume(float v);

        void update();

        const channels& getChannels() { return _channels; };

    private:

        channels _channels;

        channels _pausedChannels;

        float _volume;
    };
}