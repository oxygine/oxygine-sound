#pragma once
#include <vector>

namespace oxygine
{

    class Channel;
    class Sound;

    class SoundSystem
    {
    public:
        static SoundSystem* instance;

    public:
        virtual ~SoundSystem() {}

        static SoundSystem* create();
        static SoundSystem* get() {return instance;}
        static void free()
        {
            instance->release();
            delete instance;
            instance = 0;
        }


        virtual void init(int channels) = 0;
        virtual void release() = 0;
        virtual void pause() = 0;
        virtual void resume() = 0;
        virtual void stop() = 0;


        virtual Sound* createSound(std::vector<unsigned char>& data, bool swap) = 0;
        virtual Sound* createSound(const char* file) = 0;


        virtual Channel*    getFreeChannel() = 0;
        virtual float       getVolume() const = 0;

        virtual void setVolume(float) = 0;
        virtual void update() = 0;

    private:
    };
}
