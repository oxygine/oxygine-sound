#include "SoundSystem_oal.h"
#include "Channel_oal.h"
#include "DebugActor.h"
#include "utils/stringUtils.h"
#include "pthread.h"
#include "../oal.h"
#include "../null/SoundSystemNull.h"

//#ifdef EMSCRIPTEN
//#include "../emscripten/SoundSystemEmscripten.h
//#endif

namespace oxygine
{
    void check();

    SoundSystem* SoundSystem::create()
    {
        if (!SoundSystem::instance)
            SoundSystem::instance = new SoundSystemOAL;

        return SoundSystem::instance;
    }

    SoundSystemOAL::SoundSystemOAL(): _device(0), _context(0), _volume(1.0)
    {
#ifdef __S3E__
//      alcInit();
#endif
    }

    SoundSystemOAL::~SoundSystemOAL()
    {
    }


    void SoundSystemOAL::setContext()
    {
        alcMakeContextCurrent(_context);
        check();
    }

    void SoundSystemOAL::pause()
    {
        log::messageln("SoundSystemOAL::pause");
        /*
        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelOAL& channel = *i;
            channel.pause();
        }
        */

#if defined(__ANDROID__)
        //android needs special workaround
        alcSuspend();
#endif

#if defined(__S3E__)
        alcSuspendContext(_context);
        alcMakeContextCurrent(0);
#endif
    }

    void SoundSystemOAL::resume()
    {
        log::messageln("SoundSystemOAL::resume");

#if defined(__ANDROID__)
        alcResume();
#endif

#if defined(__S3E__)
        alcMakeContextCurrent(_context);
        alcProcessContext(_context);
#endif



        /*
        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelOAL& channel = *i;
            channel.resume();
        }
        */
    }

    void SoundSystemOAL::stop()
    {
        _channels.stop();
    }

    void SoundSystemOAL::setVolume(float v)
    {
        _volume = v;
        _channels.setVolume(v);
        check();
    }

    void SoundSystemOAL::init(int channels_num)
    {
        log::messageln("SoundSystemOAL init");
#ifdef __ANDROID__
        _device = alcOpenDevice("opensles");
#else
        _device = alcOpenDevice(0);
#endif
        if (!_device)
        {
            log::messageln("can't create alc device");
            return;
        }

        _context = alcCreateContext(_device, 0);

        if (!_context)
        {
            log::messageln("can't create alc context");
            return;
        }

        alcMakeContextCurrent(_context);


        /*
        ALCint nummono, numstereo;
        alcGetIntegerv(_device, ALC_MONO_SOURCES, 1, &nummono);
        alcGetIntegerv(_device, ALC_STEREO_SOURCES, 1, &numstereo);
        */

        _channels._channels.resize(channels_num);

        for (int i = 0; i < channels_num; ++i)
        {
            ALuint source = 0;
            alGenSources(1, &source);
            check();

            _channels._channels[i].init(this, i, source);
        }


        ChannelOAL::runThread();
        check();
    }

    void SoundSystemOAL::release()
    {
        stop();

        ChannelOAL::stopThread();
        _channels._channels.clear();
        alcMakeContextCurrent(0);
        alcDestroyContext(_context);
        _context = 0;

        alcCloseDevice(_device);
        _device = 0;
    }

    SoundOAL* SoundSystemOAL::createSound(std::vector<unsigned char>& buffer, bool swap)
    {
        if (!_context)
            return 0;

        SoundOAL* sound = 0;
        sound = new SoundOAL();
        sound->init(buffer, swap);
        check();

        return sound;
    }

    SoundOAL* SoundSystemOAL::createSound(const char* path)
    {
        if (!_context)
            return 0;

        SoundOAL* sound = 0;
        sound = new SoundOAL();
        bool res = sound->init(path);
        if (!res)
        {
            delete sound;
            return 0;
        }
        check();

        return sound;
    }

    void SoundSystemOAL::update()
    {
        if (!_device)
            return;

        _channels.update();
        check();

        if (DebugActor::instance)
        {
            char str[255];
            safe_sprintf(str, "channels: %d", _channels._channels.size() - getFreeChannelsNum());
            DebugActor::instance->addDebugString(str);
        }
    }

    int SoundSystemOAL::getFreeChannelsNum()
    {
        return _channels.getFreeNum();
    }

    ChannelOAL* SoundSystemOAL::getFreeChannel()
    {
        ChannelOAL* channel = _channels.getFree();
        check();
        return channel;
    }
}
