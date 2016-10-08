#include "ChannelNull.h"
namespace oxygine
{
    ChannelNull::ChannelNull()
    {

    }

    ChannelNull::~ChannelNull()
    {
    }

    void ChannelNull::play(const sound_desc&)
    {

    }

    void ChannelNull::continuePlay(const sound_desc&)
    {
    }

    void ChannelNull::pause()
    {
    }

    void ChannelNull::resume()
    {
    }

    void ChannelNull::stop()
    {
    }

    Sound* ChannelNull::getSound()
    {
        return 0;
    }

    float ChannelNull::getVolume()
    {
        return 0.0f;
    }

    const sound_desc& ChannelNull::getSoundDesc()
    {
        static sound_desc desc;
        return desc;
    }

    int ChannelNull::getPosition()
    {
        return 0;
    }

    void ChannelNull::setVolume(float v)
    {

    }

    void ChannelNull::setCoord(float x, float y, float z)
    {

    }
}