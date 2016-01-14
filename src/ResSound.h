#pragma once
#include "res/Resource.h"

namespace oxygine
{
    class Sound;
    class SoundSystem;
    class CreateResourceContext;

    class ResSound: public Resource
    {
    public:
        static Resource* createResSound(CreateResourceContext& context);

        ResSound();
        ~ResSound();

        bool init(CreateResourceContext& context, SoundSystem* soundSystem);

        Sound*              getSound();
        const std::string&  getPath() const;

    private:
        void _load(LoadResourcesContext* context);
        void _unload();

        SoundSystem* _soundSystem;

        std::string _file;
        bool _streaming;

        Sound* _sound;
    };
}