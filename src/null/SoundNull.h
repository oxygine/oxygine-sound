#pragma once
#include "Sound.h"

namespace oxygine
{
    class SoundNull: public Sound
    {
    public:
        SoundNull();
        ~SoundNull();

        int getDuration() const;
    private:
    };
}