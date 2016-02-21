#include "SoundSystemEmscripten.h"
#include "SoundEmscripten.h"
#include "DebugActor.h"
#include "utils/stringUtils.h"

namespace oxygine
{
    SoundSystem*    SoundSystem::create()
    {
        if (!SoundSystem::instance)
            SoundSystem::instance = new SoundSystemEmscripten;

        return SoundSystem::instance;
    }

    SoundSystemEmscripten::SoundSystemEmscripten()
    {
    }

    SoundSystemEmscripten::~SoundSystemEmscripten()
    {

    }

    void SoundSystemEmscripten::init(int channels_num)
    {
        _channels._channels.resize(channels_num);

        for (int i = 0; i < channels_num; ++i)
        {
            _channels._channels[i].init(this, i);
        }
    }

    void SoundSystemEmscripten::release()
    {
        _channels._channels.clear();
    }

    void SoundSystemEmscripten::pause()
    {

    }

    void SoundSystemEmscripten::resume()
    {

    }

    void SoundSystemEmscripten::stop()
    {
        _channels.stop();

    }


    Sound* SoundSystemEmscripten::createSound(std::vector<unsigned char>& data, bool swap)
    {
        return 0;
    }

    Sound* SoundSystemEmscripten::createSound(const char* file)
    {
        return new SoundEmscripten(file);
    }


    Channel*    SoundSystemEmscripten::getFreeChannel()
    {
        ChannelEmscripten* channel = _channels.getFree();
        return channel;
    }

    float       SoundSystemEmscripten::getVolume() const
    {
        return 1.0f;
    }

    void SoundSystemEmscripten::setVolume(float v)
    {
        _channels.setVolume(v);
    }

    void SoundSystemEmscripten::update()
    {
        _channels.update();
        if (DebugActor::instance)
        {
            char str[255];
            safe_sprintf(str, "channels: %d", _channels._channels.size() - _channels.getFreeNum());
            DebugActor::instance->addDebugString(str);
        }
    }
}