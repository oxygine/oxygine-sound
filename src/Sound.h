#pragma once
#include "oxygine_include.h"
#include "core/Object.h"
#include <list>

namespace oxygine
{
    class ResSound;

    class Sound
    {
    public:
        Sound() : _parent(0) {}
        virtual ~Sound() {}

        ResSound*   getRes() { return _parent;}
        void        setRes(ResSound* rs) { _parent = rs; }
        virtual int getDuration() const = 0;
    private:
        ResSound* _parent;
    };

	DECLARE_SMART(SoundHandle, spSoundHandle);

    class SoundHandle: public Object
    {
    public:
        enum sound_state
        {
            initial,
            playing,
            paused,
            stopped,
        };
        SoundHandle(): _volume(1.0f), _pitch(1.0f), _looping(false), _state(initial) {}
        void add(Sound* snd) {_sounds.push_back(snd) ;  _init(); }

        void setVolume(float v) { _volume = v; _updateVolume(); }
        void setPitch(float pitch) { _pitch = pitch; _updatePitch(); }
        void setLoop(bool loop) { _looping = loop; _updateLoop(); }

        sound_state getState() const { return _state; }

        void play() { _state = playing; _play(); }
        void pause() { _state = paused; _pause(); }

        void update() { _update(); }

    protected:
        virtual void _updateVolume() {}
        virtual void _updatePitch() {}
        virtual void _updateLoop() {}
        virtual void _update() {}
        virtual void _init() {}
        virtual void _play() {}
        virtual void _pause() {}

        float _volume;
        float _pitch;
        bool _looping;

        std::list<Sound*> _sounds;
        sound_state _state;
    };
}