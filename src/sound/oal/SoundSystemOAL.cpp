#include "SoundSystemOAL.h"
#include "DebugActor.h"
#include "utils/stringUtils.h"
#include "pthread.h"
#include "../oal.h"
#include "../null/SoundSystemNull.h"
#include "StreamingSoundHandleOAL.h"


namespace oxygine
{
	void OAL_CHECK()
	{
#ifdef OX_DEBUG
		int error = alGetError();

		if (error != AL_NO_ERROR)
		{
			OX_ASSERT(!"open al error");

			log::messageln("AL error: %d\n", error);
		}
#endif
	}


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
        OAL_CHECK();
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

#if TARGET_OS_IPHONE
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

#if TARGET_OS_IPHONE
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
		StreamingSoundHandleOAL::stopThread();
		for (size_t i = 0; i < _sources.size(); ++i)
		{
			alSourceStop(_sources[i]);
		}
    }

    void SoundSystemOAL::setVolume(float v)
    {
        _volume = v;

        OAL_CHECK();
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
            OX_ASSERT(0);
            log::error("can't create alc device");
            return;
        }

        _context = alcCreateContext(_device, 0);

        if (!_context)
        {
            OX_ASSERT(0);
            log::error("can't create alc context");
            return;
        }

        alcMakeContextCurrent(_context);
		OAL_CHECK();

        /*
        ALCint nummono, numstereo;
        alcGetIntegerv(_device, ALC_MONO_SOURCES, 1, &nummono);
        alcGetIntegerv(_device, ALC_STEREO_SOURCES, 1, &numstereo);
        */

		StreamingSoundHandleOAL::runThread();
        OAL_CHECK();
    }

    void SoundSystemOAL::release()
    {
        stop();

		if (!_sources.empty())
			alDeleteSources(_sources.size(), &_sources[0]);
		_sources.clear();
		_freeSources.clear();
		OAL_CHECK();

		if (!_buffers.empty())
			alDeleteBuffers(_buffers.size(), &_buffers[0]);
		_freeBuffers.clear();
		_buffers.clear();
		OAL_CHECK();
		

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
        OAL_CHECK();

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
        OAL_CHECK();

        return sound;
    }

    void SoundSystemOAL::update()
    {
        if (!_device)
            return;


        OAL_CHECK();

        if (DebugActor::instance)
        {
            char str[255];
            safe_sprintf(str, "channels: %d/%d", (int)(_sources.size() - _freeSources.size()), int(_sources.size()));
            DebugActor::instance->addDebugString(str);
        }
    }

    ALuint SoundSystemOAL::getSource()
    {
        ALuint source;
        if (_freeSources.empty())
        {
            alGenSources(1, &source);
			_sources.push_back(source);
            OAL_CHECK();
            return source;
        }

        source = _freeSources.back();
        _freeSources.pop_back();
        return source;
    }

    void SoundSystemOAL::freeSource(ALuint s)
    {
        _freeSources.push_back(s);
    }


    ALuint SoundSystemOAL::getBuffer()
    {
        ALuint buffer;
        if (_freeBuffers.empty())
        {
            alGenBuffers(1, &buffer);
			_buffers.push_back(buffer);
            OAL_CHECK();
            return buffer;
        }

        buffer = _freeBuffers.back();
        _freeBuffers.pop_back();
        return buffer;
    }

    void   SoundSystemOAL::freeBuffer(ALuint buffer)
    {
        _freeBuffers.push_back(buffer);
    }
}
