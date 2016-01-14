#include "ResSound.h"
#include "core/file.h"
#include "Sound.h"
#include "SoundSystem.h"
#include "res/CreateResourceContext.h"
#include "res/Resources.h"
#include "utils/stringUtils.h"

namespace oxygine
{
    Resource* ResSound::createResSound(CreateResourceContext& context)
    {
        ResSound* rs = new ResSound;
        rs->init(context, SoundSystem::instance);
        setNode(rs, context.walker.getNode());
        context.resources->add(rs);

        return rs;
    }

    ResSound::ResSound(): _sound(0), _soundSystem(0), _streaming(false)
    {

    }

    ResSound::~ResSound()
    {
        delete _sound;
    }

    bool ResSound::init(CreateResourceContext& context, SoundSystem* soundSystem)
    {
        _soundSystem = soundSystem;

        std::string file = context.walker.getNode().attribute("file").as_string();
        _streaming = context.walker.getNode().attribute("streaming").as_bool(_streaming);

#if EMSCRIPTEN
        _streaming = true;
#endif

        setName(Resource::extractID(context.walker.getNode(), file, ""));
        _file = context.walker.getCurrentFolder() + file;

        char nfile[512];
        path::normalize(_file.c_str(), nfile);
        _file = nfile;

        return true;
    }

    const std::string&  ResSound::getPath() const
    {
        return _file;
    }

    Sound* ResSound::getSound()
    {
        if (!_sound)
        {
            if (!_soundSystem)
                return 0;
            //printf("loading sound: %S\n", _file.c_str());

            if (_streaming)
            {
                _sound = SoundSystem::instance->createSound(_file.c_str());
            }
            else
            {
                file::buffer fb;
                file::read(_file.c_str(), fb);

                if (fb.getSize())
                    _sound = SoundSystem::instance->createSound(fb.data, true);
            }
            if (_sound)
                _sound->setRes(this);
        }

        return _sound;
    }

    void ResSound::_load(LoadResourcesContext* context)
    {

    }

    void ResSound::_unload()
    {
        if (!_sound)
            return;

    }
}