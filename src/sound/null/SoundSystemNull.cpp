#include "SoundSystemNull.h"
#include "SoundNull.h"

namespace oxygine
{
    SoundSystem* SoundSystem::instance = 0;

    void SoundSystemNull::init(int channels)
    {

    }

    void SoundSystemNull::release()
    {

    }

    void SoundSystemNull::pause()
    {

    }

    void SoundSystemNull::resume()
    {

    }

    void SoundSystemNull::stop()
    {

    }


    Sound* SoundSystemNull::createSound(std::vector<unsigned char>& data, bool swap)
    {
        return new SoundNull;
    }

    Sound* SoundSystemNull::createSound(const char* file, bool)
    {
        return new SoundNull;
    }



    void SoundSystemNull::update()
    {

    }
}