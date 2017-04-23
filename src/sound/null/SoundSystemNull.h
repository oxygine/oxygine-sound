#pragma once
#include "../SoundSystem.h"
namespace oxygine
{
    class Sound;
    class Channel;

    class SoundSystemNull : public SoundSystem
    {
    public:
        void init(int channels) override;
        void release() override;
        void pause() override;
        void resume() override;
        void stop() override;


        Sound* createSound(std::vector<unsigned char>& data, bool swap) override;
        Sound* createSound(const char* file, bool streaming) override;

        void update()  override;
    };
}