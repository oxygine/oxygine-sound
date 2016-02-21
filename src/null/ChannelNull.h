#pragma once
#include "Channel.h"
namespace oxygine
{
    class ChannelNull : public Channel
    {
    public:
        ChannelNull();
        ~ChannelNull();

        void play(const sound_desc&);
        void continuePlay(const sound_desc&);
        void pause();
        void resume();
        void stop();

        Sound*  getSound();
        float               getVolume();
        const sound_desc&   getSoundDesc();
        int                 getPosition();

        void setVolume(float v);
        void setCoord(float x, float y, float z);

    private:
    };
}