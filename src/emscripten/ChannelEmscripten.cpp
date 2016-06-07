#include "ChannelEmscripten.h"

#include <emscripten.h>
#include "core/log.h"
#include "Sound.h"
#include "ResSound.h"
#include "SoundSystemEmscripten.h"
//#include <emscripten/val.h>
//using namespace emscripten;

namespace oxygine
{
    ChannelEmscripten::ChannelEmscripten(): _handle(0), _soundSystem(0), _index(0)
    {

    }

    ChannelEmscripten::~ChannelEmscripten()
    {

    }

    void ChannelEmscripten::init(SoundSystemEmscripten* ss, int index)
    {
        _soundSystem = ss;
        _index = index;
    }

    void ChannelEmscripten::play(const sound_desc& desc)
    {
        //log::messageln("%s %s", __func__, desc.id.c_str());
        _desc = desc;
        float volume = _soundSystem->getVolume() * _desc.volume;
        _handle = EM_ASM_INT(
        {
            return sound.play(Pointer_stringify($0), $1, $2);
        }, desc.sound->getRes()->getPath().c_str(), volume, desc.looping);
        //log::messageln("handle %d", _handle);
    }

    bool ChannelEmscripten::update()
    {
        if (_handle == 0)
            return false;
        //if (_free)
        //  return false;

        //if (_paused)
        //  return false;

        int r = EM_ASM_INT(
        {
            return sound.update($0);
        }, _handle);
        if (r == 1 && !_desc.looping)
        {
            log::messageln("sound done");
            if (_desc.cbDone)
            {
                _desc.cbDone(_desc.cbUserData, this, _desc);
                _desc.cbDone = 0;
            }

            if (_desc.cbAboutEnd)
            {
                sound_desc::callback cb = _desc.cbAboutEnd;
                _desc.cbAboutEnd = 0;
                cb(_desc.cbUserData, this, _desc);
            }

            EM_ASM_ARGS(
            {
                sound.free($0);
            }, _handle);
            _handle = 0;
        }

        return true;
    }

    void ChannelEmscripten::continuePlay(const sound_desc&)
    {
    }

    void ChannelEmscripten::pause()
    {
        EM_ASM_ARGS(
        {
            sound.pause($0);
        }, _handle);
    }

    void ChannelEmscripten::resume()
    {
        EM_ASM_ARGS(
        {
            sound.resume($0);
        }, _handle);
    }

    void ChannelEmscripten::stop()
    {
        EM_ASM_ARGS(
        {
            sound.stop($0);
            sound.free($0);
        }, _handle);
        _handle = 0;
    }

    Sound* ChannelEmscripten::getSound() const
    {
        return _desc.sound;
    }

    float ChannelEmscripten::getVolume() const
    {
        return _desc.volume;
    }


    float ChannelEmscripten::getPitch() const
    {
        return _desc.pitch;
    }

    const sound_desc& ChannelEmscripten::getSoundDesc() const
    {
        return _desc;
    }

    int ChannelEmscripten::getPosition() const
    {
        return 0;
    }

    void ChannelEmscripten::setVolume(float v)
    {
        _desc.volume = v;
        EM_ASM_ARGS(
        {
            sound.setVolume($0, $1);
        }, _handle, v);
    }

    bool ChannelEmscripten::isFree() const
    {
        return _handle == 0;
    }

    void ChannelEmscripten::setPitch(float v)
    {
        _desc.pitch = v;
    }
}