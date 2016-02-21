#include "core/file.h"
#include "core/oxygine.h"
#include "res/Resources.h"
#include "res/CreateResourceContext.h"

#include "SoundPlayer.h"
#include "Sound.h"
#include "SoundInstance.h"
#include "SoundSystem.h"
#include "Channel.h"
#include "ResSound.h"

#include <algorithm>

namespace oxygine
{
    void SoundPlayer::initialize()
    {
        Resources::registerResourceType(&ResSound::createResSound, "sound");
    }

    void SoundPlayer::free()
    {
        Resources::unregisterResourceType("sound");

    }

    SoundPlayer::SoundPlayer(): _resources(0), _volume(1.0f), _time(0), _lastUpdateTime(0), _paused(false)
    {
        _time = getTimeMS();
        _lastUpdateTime = _time;
    }

    SoundPlayer::~SoundPlayer()
    {

    }

    void SoundPlayer::setVolume(float v)
    {
        _volume = v;
        for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
        {
            SoundInstance* s = (*i).get();
            s->setVolume(v);
        }

    }

    void SoundPlayer::removeSoundInstance(SoundInstance* soundInstance)
    {
        playingSounds::iterator i = std::find(_sounds.begin(), _sounds.end(), soundInstance);
        OX_ASSERT(i != _sounds.end());

        _sounds.erase(i);
    }

    void SoundPlayer::_onSoundDone(void* sound_instance, Channel* channel, const sound_desc& desc)
    {
        SoundInstance* t = (SoundInstance*)sound_instance;
        t->finished();
    }

    void SoundPlayer::_onSoundAboutDone(void* sound_instance, Channel* channel, const sound_desc& desc)
    {
        SoundInstance* t = (SoundInstance*)sound_instance;
        t->aboutDone();
    }

    void SoundPlayer::onSoundAboutDone(SoundInstance* soundInstance, Channel* channel, const sound_desc& desc)
    {
        /*
        soundInstance->_channel = 0;
        removeSoundInstance(soundInstance);
        */
    }

    spSoundInstance SoundPlayer::getSoundByIndex(int index)
    {
        return _sounds[index];
    }

    void SoundPlayer::setResources(Resources* res)
    {
        _resources = res;
    }



    spSoundInstance SoundPlayer::prepareSound(Resource* ressound_, Channel* channel, const PlayOptions& opt)
    {
        ResSound* ressound = safeCast<ResSound*>(ressound_);
        if (!ressound || !ressound->getSound())
            return 0;

        if (!channel)
            return 0;

        spSoundInstance s = new SoundInstance;
        sound_desc desc;

        float volume = opt._volume;


        if (volume < 0.f)
            volume = _volume;

        desc.sound = ressound->getSound();
        desc.cbDone = _onSoundDone;
        desc.cbAboutEnd = _onSoundAboutDone;
        desc.cbUserData = s.get();
        desc.looping = opt._looped;
        desc.id = ressound->getName();
        desc.volume = volume;
        desc.paused = opt._paused;
        desc.pitch = opt._pitch;
        desc.seek = opt._seek;

        s->_player = this;
        s->_desc = desc;
        s->_channel = channel;
        s->_startTime = getTime();

        s->_startFadeIn = 0;
        s->_fadeInMS = opt._fadeIn;

        if (opt._looped)
            s->_startFadeOut = 0;
        else
            s->_startFadeOut = desc.sound->getDuration() - opt._fadeOut;

        s->_fadeOutMS = opt._fadeOut;

        s->_volume = volume;// *_volume;
        s->_state = SoundInstance::Normal;

        if (opt._fadeIn)
        {
            s->_state = SoundInstance::FadingIn;
            desc.volume = 0.0f;
        }

        return s;
    }

    spSoundInstance SoundPlayer::play(Resource* res, const PlayOptions& opt)
    {
        Channel* ch = SoundSystem::get()->getFreeChannel();
        spSoundInstance s = prepareSound(res, ch, opt);
        if (!s)
            return 0;

        _sounds.push_back(s);
        ch->play(s->_desc);

        return s;
    }

    spSoundInstance SoundPlayer::play(const std::string& id, const PlayOptions& opt)
    {
        if (!_resources)
            return 0;

        ResSound* res = _resources->getT<ResSound>(id);
        if (!res)
            return 0;

        Channel* ch = SoundSystem::get()->getFreeChannel();
        spSoundInstance s = prepareSound(res, ch, opt);
        if (!s)
            return 0;

        _sounds.push_back(s);
        ch->play(s->_desc);

        return s;
    }

    spSoundInstance SoundPlayer::continuePlay(Resource* res, Channel* ch, const PlayOptions& opt)
    {
        spSoundInstance s = prepareSound(res, ch, opt);
        if (!s)
            return 0;

        _sounds.push_back(s);
        ch->continuePlay(s->_desc);

        return s;
    }

    void SoundPlayer::pause()
    {
        for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
        {
            SoundInstance* s = (*i).get();
            if (s->_channel)
            {
                s->_channel->pause();
            }
        }
        _paused = true;
    }

    void SoundPlayer::resume()
    {
        for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
        {
            SoundInstance* s = (*i).get();
            if (s->_channel)
            {
                s->_channel->resume();
            }
        }
        _paused = false;
    }

    void SoundPlayer::stopByID(const string& id)
    {
        bool try_again = true;
        while (try_again)
        {
            try_again = false;

            for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
            {
                spSoundInstance s = *i;
                if (!s->_channel)
                    continue;

                if (s->_desc.id == id)
                {
                    s->_channel->stop();
                }
            }
        }
    }

    void SoundPlayer::stop()
    {
        for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
        {
            spSoundInstance sound = *i;
            sound->stop();
        }

        _sounds.resize(0);
    }

    void SoundPlayer::fadeOut(int ms)
    {
        for (playingSounds::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
        {
            spSoundInstance sound = *i;
            sound->fadeOut(ms);
        }
    }

    unsigned int SoundPlayer::getTime()const
    {
        return _time;
    }

    void SoundPlayer::update()
    {
        timeMS t = getTimeMS();
        timeMS d = t - _lastUpdateTime;
        if (_paused)
            d = 0;
        _time += d;

        for (size_t i = 0; i < _sounds.size();)
        {
            spSoundInstance s = _sounds[i];
            s->update();

            if (s->getChannel())
                ++i;
            else
                _sounds.erase(_sounds.begin() + i);
        }

        //log::messageln("sounds %d", _sounds.size());

        _lastUpdateTime = t;
    }
}
