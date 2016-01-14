#pragma once
#include "Sound.h"
#include <string>

namespace oxygine
{
    using namespace std;
    class SoundEmscripten: public Sound
    {
    public:
        SoundEmscripten(const char* file);
        ~SoundEmscripten();

        int getDuration() const;

    private:
        string _file;
    };
}