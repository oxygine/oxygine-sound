#include "Sound_oal.h"
#include "../OggStream.h"
#include "../oal.h"
#include "SoundSystem_oal.h"
#include "core/ThreadDispatcher.h"

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




    SoundSystemOAL* ss()
    {
        return (SoundSystemOAL*)SoundSystem::get();
    }

    SoundHandleOAL::SoundHandleOAL(): _alSource(0), _pos(0), _stream(0)
    {
    }

    void SoundHandleOAL::_init()
    {

    }

    void SoundHandleOAL::_play()
    {

        _alSource = ss()->getSource();

        alSourcef(_alSource, AL_GAIN, _volume);
        check();

        alSourcef(_alSource, AL_PITCH, _pitch);
        check();


		if (_stream)
		{
			_stream->resume(this);

		}
		else
		{
			SoundOAL *snd = (SoundOAL*)_sounds.front();
			if (snd->getAlBuffer())
				_stream = new StaticStreamOAL(snd->getAlBuffer());
			else
				_stream = new OggStreamOAL(snd);
			_stream->play(this);
		}
		


        //alSourcei(_alSource, AL_LOOPING, _looping ? AL_TRUE : AL_FALSE);
        check();

        alSourcei(_alSource, AL_BYTE_OFFSET, _pos);
        check();

        alSourcePlay(_alSource);
        check();
    }

    void SoundHandleOAL::_pause()
    {
		alGetSourcei(_alSource, AL_BYTE_OFFSET, &_pos);
		check();

		alSourceStop(_alSource);
		check();


		_stream->pause(this);


        alSourcei(_alSource, AL_LOOPING, AL_FALSE);



        ss()->freeSource(_alSource);
        _alSource = 0;
        _state = paused;
    }

    void SoundHandleOAL::_update()
    {
        if (!_alSource)
            return;

        _stream->update(this);

        ALint state = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
        check();

        if (state == AL_STOPPED)
        {
            _state = stopped;
            ss()->freeSource(_alSource);
            _alSource = 0;
        }
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

    extern ThreadMessages _messages;

    bool StaticStreamOAL::play(SoundHandleOAL* s)
    {
        alSourceQueueBuffers(s->_alSource, 1, &_buffer);
		check();

        return false;
    }

    void StaticStreamOAL::update(SoundHandleOAL* s)
    {

    }

	void StaticStreamOAL::pause(SoundHandleOAL*s)
	{
		ALuint b;
		alSourceUnqueueBuffers(s->_alSource, 1, &b);
		check();
	}

	void StaticStreamOAL::resume(SoundHandleOAL*s)
	{
		alSourceQueueBuffers(s->_alSource, 1, &_buffer);
		check();
	}

	extern void* getSoundStreamTempBuffer(int& size);

    OggStreamOAL::OggStreamOAL(SoundOAL* snd): _snd(snd)
    {
        snd->initStream(_stream);

		alGenBuffers(STREAM_BUFFERS, _buffers);
    }

    bool OggStreamOAL::play(SoundHandleOAL* s)
    {
        decode(s, _buffers, STREAM_BUFFERS);

        return false;
    }

    void OggStreamOAL::update(SoundHandleOAL* s)
    {
        ALint nump;
        alGetSourcei(s->_alSource, AL_BUFFERS_PROCESSED, &nump);
        ALuint buffers[STREAM_BUFFERS];
		if (nump)
		{
			alSourceUnqueueBuffers(s->_alSource, nump, buffers);
			check();
		}

        decode(s, buffers, nump);
    }

	void OggStreamOAL::resume(SoundHandleOAL* s)
	{
		alSourceQueueBuffers(s->_alSource, STREAM_BUFFERS, _buffers);
		check();
	}

	void OggStreamOAL::pause(SoundHandleOAL* s)
	{
		alSourceUnqueueBuffers(s->_alSource, STREAM_BUFFERS, _buffers);
		check();
	}

	void OggStreamOAL::decode(SoundHandleOAL* s, ALuint* buffers, int num)
    {
        int size;
        void* data = getSoundStreamTempBuffer(size);

        for (int i = 0; i < num; ++i)
        {
			log::messageln("decode %d", getTimeMS());
            ALuint buffer = buffers[i];
            size = _stream.decodeNextBlock(s->_looping, data, size);
            if (!size)
                break;
            alBufferData(buffer, _snd->getFormat(), data, size, _stream.getRate());
			check();
            alSourceQueueBuffers(s->_alSource, 1, &buffer);
			check();
        }
    }

}