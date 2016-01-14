#include "SoundEmscripten.h"
#include <emscripten.h>
#include "utils/stringUtils.h"
namespace oxygine
{
    SoundEmscripten::SoundEmscripten(const char* file): _file(file)
    {
    }

    SoundEmscripten::~SoundEmscripten()
    {

    }

    int SoundEmscripten::getDuration() const
    {
        return 10000;
    }
}