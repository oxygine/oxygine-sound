#pragma once
#include "Channel.h"

namespace oxygine
{
    class SoundSystemEmscripten;

    class ChannelEmscripten : public Channel
    {
    public:
        ChannelEmscripten();
        ~ChannelEmscripten();

        void play(const sound_desc&);
        void continuePlay(const sound_desc&);
        void pause();
        void resume();
        void stop();
        bool update();

        void seek(int tm) {}

        Sound*  getSound() const;
        float               getVolume() const;
        float               getPitch() const;
        const sound_desc&   getSoundDesc() const;
        int                 getPosition() const;

        void setVolume(float v);
        void setPitch(float v);
        void setCoord(float x, float y, float z) {}

        bool isFree() const;
        void _updateMasterVolume(float) {}

    protected:
        friend class SoundSystemEmscripten;
        void init(SoundSystemEmscripten*, int index);

    private:
        SoundSystemEmscripten* _soundSystem;

        sound_desc _desc;
        int _handle;
        int _index;
    };
}