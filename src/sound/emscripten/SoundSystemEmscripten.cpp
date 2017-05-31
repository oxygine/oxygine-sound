#include "SoundSystemEmscripten.h"
#include "SoundEmscripten.h"
#include "DebugActor.h"
#include "utils/stringUtils.h"
#include <emscripten.h>

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
        EM_ASM_ARGS(
        {
            sound.init();
        }, 0);
    }

    void SoundSystemEmscripten::release()
    {
        //_channels._channels.clear();
    }

    void SoundSystemEmscripten::pause()
    {

    }

    void SoundSystemEmscripten::resume()
    {

    }

    void SoundSystemEmscripten::stop()
    {
        //_channels.stop();
    }


    Sound* SoundSystemEmscripten::createSound(std::vector<unsigned char>& data, bool swap)
    {
        return 0;
    }

    Sound* SoundSystemEmscripten::createSound(const char* file, bool streaming)
    {
        return new SoundEmscripten(file);
    }



    void SoundSystemEmscripten::update()
    {
        //_channels.update();
        if (DebugActor::instance)
        {

            int num = EM_ASM_INT(
            {
                return sound.stats();
            }, 0);

            char str[255];
            safe_sprintf(str, "handles: %d", num);
            DebugActor::instance->addDebugString(str);
        }
    }
}