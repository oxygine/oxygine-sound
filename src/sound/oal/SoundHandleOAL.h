#pragma once
#include "../SoundHandle.h"
#include "../oal.h"

namespace oxygine
{
    class Sound;

    class SoundHandleOAL : public SoundHandle
    {
    public:
        static SoundHandleOAL* create(Sound*);

        SoundHandleOAL();
        bool isPlaying() const { return _alSource != 0; }

    protected:
        friend class StaticStreamOAL;
        friend class OggStreamOAL;

        class SoundSystemOAL* ss();

        void _init();
        void _play();
        void _pause();
        void _resume();
        void _stop();

        void _update();
        void _updateVolume();
        void _updatePitch();
        void _updateLoop() override;
        void _setPosition(int tm) override;

        void _ended();
        void _restore();

        virtual void _xplay() {}
        virtual void _xpause() {}
        virtual void _xupdate() {}
        virtual void _xresume() {}
        virtual void _xstop() {}
        virtual void _xupdateLoop() {}
        virtual void _xsetPosition(int tm) {}

        ALuint _alSource;
        ALint _pos;
    };
}