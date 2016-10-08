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

    Sound* SoundSystemNull::createSound(const char* file)
    {
        return new SoundNull;
    }


    Channel*    SoundSystemNull::getFreeChannel()
    {
        return 0;
    }

    float       SoundSystemNull::getVolume() const
    {
        return 1.0f;
    }

    void SoundSystemNull::setVolume(float)
    {

    }

    void SoundSystemNull::update()
    {

    }
}