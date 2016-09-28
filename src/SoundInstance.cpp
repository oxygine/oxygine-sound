#include "SoundInstance.h"
#include "SoundPlayer.h"
#include "Sound.h"
#include "Channel.h"

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
        OX_ASSERT(_finished == false);
        _finished = true;
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

		_state = Normal;
        _handle->play();
        _player->addSoundInstance(this);
    }

    void SoundInstance::pause()
    {
        EXIT_IF_EMPTY();

        _handle->pause();
        _player->removeSoundInstance(this);
    }

    void SoundInstance::resume()
    {
        EXIT_IF_EMPTY();

        _handle->resume();
        _player->addSoundInstance(this);
        /*

        if (!_channel)
            return;
        if (_state != Paused)
            return;
        _state = Normal;
        _channel->resume();
        */
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

    void SoundInstance::fadeOut(int fadeOutMS)
    {
        if (_state == Paused)
        {
            //we don't need sound, emulate fade out for silent sound
			_handle->setVolume(0);
        }

        if (_state == Normal || _state == FadingIn || _state == FadingOutStop || _state == Paused)
        {
            _startFadeOut = _player->getTime();
            _state = FadingOutStop;
            _fadeOutMS = fadeOutMS;
        }
    }

    void SoundInstance::fadeOutPause(int fadeOutMS)
    {
        if (_state == Normal || _state == FadingIn || _state == FadingOutStop)
        {
            _startFadeOut = _player->getTime();
            _state = FadingOutPause;
            _fadeOutMS = fadeOutMS;
        }
    }

    void SoundInstance::fadeIn(int fadeInMS)
    {
		switch (_state)
		{
		case oxygine::SoundInstance::FadingIn:
			break;
		case oxygine::SoundInstance::FadingOutStop:
		case oxygine::SoundInstance::FadingOutPause:
		case oxygine::SoundInstance::Paused:
		case oxygine::SoundInstance::Stopped:
			_fadeIn(fadeInMS);
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
		_startFadeIn = _player->getTime();
		_fadeInMS = dur;
		_state = FadingIn;
		_handle->setVolume(0);

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
        if (_channel)
            _channel->setCoord(pos.x, pos.y, z);
    }

    void SoundInstance::setPitch(float v)
    {
        _handle->setPitch(v);

        if (_channel)
            _channel->setPitch(_volume);
    }

    void SoundInstance::setLoop(bool loop)
    {
        _handle->setLoop(loop);
    }

    void SoundInstance::seek(int tm)
    {
        if (_channel)
            _channel->seek(tm);
    }

    void SoundInstance::update()
    {
        _handle->update();
        /*
        switch (_handle->getState())
        {
            case Sou
        default:
            break;
        }
        */
        if (_handle->getState() == SoundHandle::ended)
        {
            finished();
			return;
        }

		timeMS dur = _handle->getDuration();

        //OX_ASSERT(_channel);
        if (!_channel)
            return;

		int duration = _handle->getDuration();
        int globalTime = _player->getTime();

        //int soundTime = globalTime - _startTime;

        switch (_state)
        {
            case Paused:
            {

            }
            break;

            case FadingIn:
            {
                float p = (globalTime - _startFadeIn) / (float)_fadeInMS;

                if (p >= 1.0f)
                {
                    _state = Normal;
                    p = 1.0;
                }

                //printf("fade in\n");
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
                float p = (globalTime - _startFadeOut) / (float)_fadeOutMS;

                if (p > 1.0)
                {
                    if (_state == FadingOutPause)
                    {
						_handle->setVolume(0);
						_handle->pause();
                        _state = Paused;
                        _startFadeOut = duration;
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
        OX_ASSERT(0);
        return 0;
    }
}