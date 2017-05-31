#include "SoundEmscripten.h"
#include "SoundHandleEmscripten.h"

namespace oxygine
{
    SoundEmscripten::SoundEmscripten(const char *file):_path(file)
    {

    }

    SoundEmscripten::~SoundEmscripten()
    {

    }

    int SoundEmscripten::getDuration() const
    {
        return 1000;
    }

    SoundHandle* SoundEmscripten::createSH()
    {
        SoundHandleEmscripten *handle = new SoundHandleEmscripten(_path);

        return handle;
    }
}