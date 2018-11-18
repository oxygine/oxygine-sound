#pragma once
#include "oxygine/core/Object.h"
#include "oxygine/math/Vector3.h"

namespace oxygine
{

    DECLARE_SMART(SoundHandle, spSoundHandle);

    class SoundHandle : public Object
    {
    public:
        enum sound_state
        {
            initial,
            playing,
            paused,
            stopped,
            ended,
        };
        SoundHandle() : _volume(1.0f), _pitch(1.0f), _looping(false), _state(initial), _pos3d(0, 0, 0) {}

        void setVolume(float v) { _volume = v; _updateVolume(); }
        void setPitch(float pitch) { _pitch = pitch; _updatePitch(); }
        void setLoop(bool loop) { _looping = loop; _updateLoop(); }
        void setPosition(timeMS pos) { _setPosition(pos); }
        void setPosition3D(const Vector3& pos) { _pos3d = pos; _updatePosition3D(); }

        float           getVolume() const { return _volume; }
        float           getPitch() const { return _pitch; }
        const Vector3&  getPosition3D() const { return _pos3d; }
        bool            getLoop() const { return _looping; }
        sound_state     getState() const { return _state; }
        timeMS          getDuration() const { return _duration; }
        timeMS          getPosition() const { return _getPosition(); }

        /**play from begin*/
        void play() { _state = playing; _play(); }
        void pause() { _state = paused; _pause(); }
        void resume() { _state = playing; _resume(); }
        void stop() { _state = stopped; _stop(); }

        void update() { _update(); }

    protected:

        virtual void _updateVolume() {}
        virtual void _updatePitch() {}
        virtual void _updateLoop() {}
        virtual void _update() {}
        virtual void _updatePosition3D() {}

        virtual void _init() {}
        virtual void _play() {}
        virtual void _pause() {}
        virtual void _resume() {}
        virtual void _stop() {}
        virtual void _setPosition(int tm) {}

        virtual timeMS _getPosition() const = 0;

        float _volume;
        float _pitch;
        bool _looping;
        timeMS _duration;

        Vector3 _pos3d;

        sound_state _state;
    };
}