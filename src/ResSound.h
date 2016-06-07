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
        static ResSound* create(const std::string& file, bool streaming);

        ResSound();
        ~ResSound();

        bool init(const std::string& file, bool streaming);
        bool init(CreateResourceContext& context);

        Sound*              getSound();
        const std::string&  getPath() const;

    private:
        void _load(LoadResourcesContext* context);
        void _unload();

        std::string _file;
        bool _streaming;

        Sound* _sound;
    };


    void emscStartSoundsPreloading(Resources& res);
}