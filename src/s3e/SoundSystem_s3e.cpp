#include "SoundSystem_s3e.h"
#include "Channel_s3e.h"
#include "core/log.h"
#include "DebugActor.h"

namespace oxygine
{
    SoundSystem* SoundSystem::instance = 0;
    void check();

    SoundSystemS3E::SoundSystemS3E(): _volume(1.0)
    {

    }

    SoundSystemS3E::~SoundSystemS3E()
    {

    }


    void SoundSystemS3E::pause()
    {
        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelS3E* channel = *i;
            //channel->G
            channel->pause();
        }
        check();
    }

    void SoundSystemS3E::resume()
    {
        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelS3E* channel = *i;
            channel->resume();
        }
        check();
    }

    void SoundSystemS3E::setVolume(float v)
    {
        _volume = v;

        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelS3E* channel = *i;
            channel->_updateMasterVolume(v);
        }
        check();
    }

    void SoundSystemS3E::init(int channels_num)
    {
        _channels.reserve(channels_num);
        for (int i = 0; i < channels_num; ++i)
        {
            ChannelS3E* channel = new ChannelS3E(this, i);
            _channels.push_back(channel);
        }
        check();

        log::messageln("S3E_SOUND_OUTPUT_FREQ: %d", s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ));
    }

    void SoundSystemS3E::release()
    {
    }

    SoundS3E* SoundSystemS3E::createSound(std::vector<unsigned char>& buffer, bool swap)
    {
        SoundS3E* sound = 0;
        sound = new SoundS3E();
        sound->init(buffer, swap);
        check();

        return sound;
    }

    SoundS3E* SoundSystemS3E::createSound(const char* path)
    {
        SoundS3E* sound = 0;
        sound = new SoundS3E();
        sound->init(path);
        check();

        return sound;
    }

    void SoundSystemS3E::update()
    {
        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelS3E* channel = *i;
            channel->update();
        }

        if (DebugActor::instance)
            DebugActor::instance->addDebugString("free channels: %d/%d", getFreeChannelsNum(), (int)_channels.size());
        check();
    }

    int SoundSystemS3E::getFreeChannelsNum()
    {
        int freeNum = 0;
        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelS3E* channel = *i;

            if (channel->_free)
                ++freeNum;
        }

        return freeNum;
    }

    ChannelS3E* SoundSystemS3E::getFreeChannel()
    {
        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelS3E* channel = *i;

            if (channel->_free)
            {
                check();
                return channel;
            }
        }

        check();
        return 0;
    }
}