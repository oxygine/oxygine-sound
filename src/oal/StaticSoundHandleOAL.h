#pragma once
#include "Sound_oal.h"

namespace oxygine
{
    class StaticSoundHandleOAL: public SoundHandleOAL
    {
    public:
        StaticSoundHandleOAL(ALuint buffer);

    protected:
        void _xplay() override;
        void _xpause() override;
        void _xresume() override;
        void _xupdate() override;

        void _xupdateLoop() override;

        ALuint _buffer;
    };

}
