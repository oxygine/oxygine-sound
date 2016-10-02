#include "Sound_oal.h"
#include "../OggStream.h"
#include "../oal.h"
#include "SoundSystem_oal.h"
#include "core/ThreadDispatcher.h"
#include "StaticSoundHandleOAL.h"
#include "StreamingSoundHandleOAL.h"

namespace oxygine
{
    using namespace std;

    void check();

    SoundOAL::SoundOAL(): _alBuffer(0), _format(AL_FORMAT_MONO16), _timeMS(0)
    {

    }

    SoundOAL::~SoundOAL()
    {
        check();
        if (_alBuffer)
        {
            alDeleteBuffers(1, &_alBuffer);
            _alBuffer = 0;
        }
        check();
    }

    bool SoundOAL::_init(OggStream& stream)
    {
        if (stream.isEmpty())
            return false;
        int channels = 0, rate = 0;
        unsigned int pcm = 0;
        stream.getDetails(&pcm, &channels, &rate, &_timeMS);

        if (channels == 1)
            _format = AL_FORMAT_MONO16;
        if (channels == 2)
            _format = AL_FORMAT_STEREO16;

        check();

        int len = pcm / rate;//len in seconds
        //todo, fix, use only pcm?

        //return;
        //return true;


        if (len > 3)//this sound would be streamed
            return true;

        vector<char> buff;
        buff.resize(pcm * 2 * channels);

        void* data = &buff.front();
        size_t size = buff.size();
        stream.decodeAll(data, (int)size);

        //const std::vector<char> &decoded = stream.getDecodedBuffer();

        alGenBuffers(1, &_alBuffer);
        check();

        alBufferData(_alBuffer, _format, data, (ALsizei)size, rate);
        check();

        return false;
    }

    void SoundOAL::init(std::vector<unsigned char>& buffer, bool swap)
    {
        OggStream stream;
        stream.init(&buffer.front(), (unsigned int)buffer.size());

        bool streaming = _init(stream);
        if (streaming)
        {
            if (swap)
                buffer.swap(_fileBuffer);
            else
                _fileBuffer = buffer;
        }
    }

    bool SoundOAL::init(const char* path)
    {
        _fileName = path;

        OggStream stream;
        bool res = stream.init(path);

        _init(stream);
        return res;
    }

    int SoundOAL::getDuration() const
    {
        return _timeMS;
    }

    void SoundOAL::initStream(OggStream& stream)
    {
        if (_fileName.empty())
            stream.init(&_fileBuffer.front(), (unsigned int)_fileBuffer.size());
        else
            stream.init(_fileName.c_str());
    }




    SoundSystemOAL* SoundHandleOAL::ss()
    {
        return (SoundSystemOAL*)SoundSystem::get();
    }

    SoundHandleOAL::SoundHandleOAL(): _alSource(0), _pos(0)//, _stream(0)
    {
    }

    void SoundHandleOAL::_init()
    {

    }

    void SoundHandleOAL::_restore()
    {
        alSourcef(_alSource, AL_GAIN, _volume);
        check();

        alSourcef(_alSource, AL_PITCH, _pitch);
        check();
    }

    void SoundHandleOAL::_play()
    {
        if (_alSource)
            _stop();

        _alSource = ss()->getSource();
        _restore();


        _xplay();

        //alSourcei(_alSource, AL_BYTE_OFFSET, 0);
        check();

        alSourcePlay(_alSource);
        check();
    }

    void SoundHandleOAL::_resume()
    {
        if (_alSource)
            return;

        _alSource = ss()->getSource();

        _restore();

        _xresume();

        alSourcei(_alSource, AL_BYTE_OFFSET, _pos);
        check();

        alSourcePlay(_alSource);
        check();
    }

    void SoundHandleOAL::_pause()
    {
        if (!_alSource)
            return;

        alGetSourcei(_alSource, AL_BYTE_OFFSET, &_pos);
        check();

        alSourceStop(_alSource);
        check();

        _xpause();

        //_stream->pause(this);


        alSourcei(_alSource, AL_LOOPING, AL_FALSE);

        ss()->freeSource(_alSource);
        _alSource = 0;
        _state = paused;
    }

    void SoundHandleOAL::_stop()
    {
        if (_alSource)
        {
            alSourceStop(_alSource);
            check();
            ss()->freeSource(_alSource);
        }

        _xstop();

        _alSource = 0;
        _state = stopped;
        _pos = 0;
    }

    void SoundHandleOAL::_update()
    {
        if (!_alSource)
            return;

        _xupdate();

        //_stream->update(this);
    }

    void SoundHandleOAL::_updateVolume()
    {
        if (!_alSource)
            return;
        alSourcef(_alSource, AL_GAIN, _volume);
    }

    void SoundHandleOAL::_updatePitch()
    {
        if (!_alSource)
            return;
        alSourcef(_alSource, AL_PITCH, _pitch);
    }

    void SoundHandleOAL::_updateLoop()
    {
        if (!_alSource)
            return;
        _xupdateLoop();
    }

    void SoundHandleOAL::_ended()
    {
        _state = ended;
        ss()->freeSource(_alSource);
        _alSource = 0;
    }


    SoundHandleOAL* SoundHandleOAL::create(Sound* snd_)
    {
        SoundOAL* snd = (SoundOAL*)snd_;

        SoundHandleOAL* s;
        if (snd->getAlBuffer())
            s = new StaticSoundHandleOAL(snd->getAlBuffer());
        else
            s = new StreamingOggSoundHandleOAL(snd);

        s->_duration = snd->getDuration();

        return s;
    }
}