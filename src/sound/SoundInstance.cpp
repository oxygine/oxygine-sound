#include "SoundInstance.h"
#include "SoundPlayer.h"
#include "Sound.h"
#include "SoundHandle.h"
namespace oxygine
{
#define EXIT_IF_EMPTY() if (!_handle) return

    SoundInstance::SoundInstance(SoundPlayer* player, SoundHandle* h):
        _player(player),
        _handle(h),
        _startTime(0),
        _fadeInMS(0),
        _fadeOutMS(0),
        _state(Paused),
        _volume(1.0f),
        _startFadeIn(0),
        _finished(false),
        _startFadeOut(0)
    {

    }

    SoundInstance::~SoundInstance()
    {
        if (_handle)
            delete _handle;
        _handle = 0;
    }

    void SoundInstance::finished()
    {
        //OX_ASSERT(_finished == false);
        //_finished = true;
        //_player->removeSoundInstance(this);

        _state = Ended;

        //callback would called on fadeout
        if (_fadeOutMS)
            return;

        if (_cbDone)
        {
            SoundEvent ev(SoundEvent::EVENT_DONE, this);
            _cbDone(&ev);
        }
    }

    void SoundInstance::aboutDone()
    {
        if (_cbAboutDone)
        {
            SoundEvent ev(SoundEvent::EVENT_ABOUT_DONE, this);
            _cbAboutDone(&ev);
        }
    }

    void SoundInstance::play()
    {
        EXIT_IF_EMPTY();

        //if (!(_state == Paused || _state == Stopped))
        //  return;
        _handle->setVolume(_volume);
        _state = Normal;
        _handle->play();
        _player->addSoundInstance(this);
    }

    void SoundInstance::pause()
    {
        EXIT_IF_EMPTY();

        _state = Paused;
        _handle->pause();
        _player->removeSoundInstance(this);
    }

    void SoundInstance::resume()
    {
        EXIT_IF_EMPTY();

        _state = Normal;
        _handle->resume();
        _player->addSoundInstance(this);
    }

    void SoundInstance::stop()
    {
        EXIT_IF_EMPTY();
        //if (_state == Stopped)
        //  return;

        _handle->stop();
        _player->removeSoundInstance(this);
        _state = Stopped;
        //delete _handle;
        //_handle = 0;
    }

    void SoundInstance::fadeOut(int ms)
    {
        _fadeOut(ms, true);
    }

    void SoundInstance::fadeOutPause(int ms)
    {
        _fadeOut(ms, false);
    }

    void SoundInstance::_fadeOut(int ms, bool stop)
    {
        State nxt = stop ? FadingOutStop : FadingOutPause;

        timeMS ct = _player->getTime();
        float p;
        switch (_state)
        {
            case oxygine::SoundInstance::FadingIn:
                _state = stop ? FadingOutStop : FadingOutPause;
                _fadeOutMS = ms;
                p = (ct - _startFadeIn) / (float)_fadeInMS;
                _startFadeOut = (int)(ct - (1.0f - p) * (float)_fadeOutMS);
                break;

            case oxygine::SoundInstance::Paused:
            case oxygine::SoundInstance::Stopped:
                _handle->setVolume(0);
                break;

            case oxygine::SoundInstance::Normal:
                _startFadeOut = ct;
                _state = nxt;
                _fadeOutMS = ms;
                break;

            case oxygine::SoundInstance::FadingOutStop:
            case oxygine::SoundInstance::FadingOutPause:
                _state = nxt;
                p = (ct - _startFadeOut) / (float)_fadeOutMS;
                _fadeOutMS = ms;
                _startFadeOut = int(ct - p * (float)_fadeOutMS);
                break;

            case oxygine::SoundInstance::Ended:
                break;

            default:
                break;
        }
    }

    void SoundInstance::fadeIn(int ms)
    {
        timeMS ct = _player->getTime();
        float p;

        switch (_state)
        {
            case oxygine::SoundInstance::FadingIn:
                p = (ct - _startFadeIn) / (float)_fadeInMS;
                _fadeInMS = ms;
                _startFadeIn = int(ct - p * (float)_fadeInMS);
                break;

            case oxygine::SoundInstance::Paused:
            case oxygine::SoundInstance::Stopped:
                _handle->setVolume(0);
                _fadeIn(ms);
                _startFadeIn = _player->getTime();
                break;

            case oxygine::SoundInstance::FadingOutStop:
            case oxygine::SoundInstance::FadingOutPause:
                _fadeIn(ms);
                ct = _player->getTime();
                p = (ct - _startFadeOut) / (float)_fadeOutMS;
                _startFadeIn = int(ct - (1.0f - p) * (float)_fadeInMS);
                break;

            case oxygine::SoundInstance::Normal:
                break;

            case oxygine::SoundInstance::Ended:
                break;

            default:
                break;
        }
    }

    void SoundInstance::_fadeIn(int dur)
    {
        _fadeInMS = dur;
        _state = FadingIn;
        _handle->play();
        _player->addSoundInstance(this);
    }

    bool SoundInstance::isPlaying() const
    {
        return _handle->getState() == SoundHandle::playing;
    }

    bool SoundInstance::isPaused() const
    {
        return _state == Paused;
    }

    bool SoundInstance::isFadingOut() const
    {
        return _state == FadingOutPause || _state == FadingOutStop;
    }

    float SoundInstance::getVolume() const
    {
        return _volume;
    }

    float SoundInstance::getCurrentVolume() const
    {
        return _handle->getVolume();
    }

    float SoundInstance::getPitch() const
    {
        return _handle->getPitch();
    }

    void SoundInstance::setVolume(float v)
    {
        OX_ASSERT(v >= 0 && v <= 1.0f);
        _volume = v;
        _handle->setVolume(v);
    }

    void SoundInstance::setCoord(const Vector2& pos, float z)
    {
		OX_ASSERT(0);
    }

    void SoundInstance::setPitch(float v)
    {
        _handle->setPitch(v);
    }

    void SoundInstance::setLoop(bool loop)
    {
        _handle->setLoop(loop);
    }

    void SoundInstance::seek(int tm)
    {
        _handle->setPosition(tm);
    }

    void SoundInstance::update()
    {
        _handle->update();
        if (_handle->getState() == SoundHandle::ended)
        {
            finished();
            return;
        }

        //timeMS dur = _handle->getDuration();
        //int duration = _handle->getDuration();
        int tm = _player->getTime();

        //int soundTime = globalTime - _startTime;

        switch (_state)
        {
            case Paused:
            {

            }
            break;

            case FadingIn:
            {
                float p = (tm - _startFadeIn) / (float)_fadeInMS;

                if (p >= 1.0f)
                {
                    _state = Normal;
                    p = 1.0;
                }

                float volume = p * _volume;
                _handle->setVolume(volume);
            }
            break;

            case Normal:
            {
                /*
                if (!_desc.looping && soundTime >= _startFadeOut && duration != _startFadeOut)
                {
                    _state = FadingOutStop;
                    if (_cbDone)
                    {
                        Event ev(Event::COMPLETE);
                        _cbDone(&ev);
                    }
                }
                */
            }
            break;

            case FadingOutPause:
            case FadingOutStop:
            {
                float p = (tm - _startFadeOut) / (float)_fadeOutMS;

                if (p > 1.0)
                {
                    _handle->setVolume(0);

                    if (_state == FadingOutPause)
                    {
                        _handle->pause();
                        _state = Paused;
                    }
                    else
                    {
                        _handle->stop();
                        finished();
                    }
                }
                else
                {
                    float volume = (1.0f - p) * _volume;
                    _handle->setVolume(volume);
                }
            }
            break;
        }

    }

    int SoundInstance::getDuration()const
    {
        return _handle->getDuration();
    }

    int SoundInstance::getPosition() const
    {
        return _handle->getPosition();
    }
}