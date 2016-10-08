#include "SoundSystem_oal.h"
#include "DebugActor.h"
#include "utils/stringUtils.h"
#include "pthread.h"
#include "../oal.h"
#include "../null/SoundSystemNull.h"
#include "StreamingSoundHandleOAL.h"


namespace oxygine
{
	void oalCheck()
	{
#ifdef CHANNEL_DEBUG
		int error = alGetError();

		if (error != AL_NO_ERROR)
		{
#ifdef CHANNEL_DEBUG
			log::messageln("AL error: %d\n", error);
#endif
			OX_ASSERT(error == AL_NO_ERROR);
			//throw std::string("OpenAL error was raised.");
		}
#endif
	}


    SoundSystem* SoundSystem::create()
    {
        if (!SoundSystem::instance)
            SoundSystem::instance = new SoundSystemOAL;

        return SoundSystem::instance;
    }

    SoundSystemOAL::SoundSystemOAL(): _device(0), _context(0), _volume(1.0), _totalSources(0)
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
        oalCheck();
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
		StreamingSoundHandleOAL::stopThread();
    }

    void SoundSystemOAL::setVolume(float v)
    {
        _volume = v;

        oalCheck();
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

		StreamingSoundHandleOAL::runThread();
        oalCheck();
    }

    void SoundSystemOAL::release()
    {
        stop();

		if (!_freeSources.empty())
			alDeleteSources(_freeSources.size(), &_freeSources[0]);
		_freeSources.clear();
		oalCheck();

		if (!_freeBuffers.empty())
			alDeleteBuffers(_freeBuffers.size(), &_freeBuffers[0]);
		_freeBuffers.clear();
		oalCheck();
		


        alcMakeContextCurrent(0);
        alcDestroyContext(_context);
		oalCheck();
        _context = 0;

        alcCloseDevice(_device);
        _device = 0;
		oalCheck();
    }

    SoundOAL* SoundSystemOAL::createSound(std::vector<unsigned char>& buffer, bool swap)
    {
        if (!_context)
            return 0;

        SoundOAL* sound = 0;
        sound = new SoundOAL();
        sound->init(buffer, swap);
        oalCheck();

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
        oalCheck();

        return sound;
    }

    void SoundSystemOAL::update()
    {
        if (!_device)
            return;


        oalCheck();

        if (DebugActor::instance)
        {
            char str[255];
            safe_sprintf(str, "channels: %d/%d", _totalSources - (int)_freeSources.size(), _totalSources);
            DebugActor::instance->addDebugString(str);
        }
    }

    ALuint SoundSystemOAL::getSource()
    {
        ALuint source;
        if (_freeSources.empty())
        {
            _totalSources++;
            alGenSources(1, &source);
            oalCheck();
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
            //_totalSources++;
            alGenBuffers(1, &buffer);
            oalCheck();
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
