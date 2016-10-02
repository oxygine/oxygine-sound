#include "StreamingSoundHandleOAL.h"
#include "core/ThreadDispatcher.h"

namespace oxygine
{
    void check();

    extern ThreadMessages _messages;


    void* getSoundStreamTempBuffer(int& size);

    void threadDecode(const ThreadMessages::message& msg)
    {
        StreamingSoundHandleOAL* s = (StreamingSoundHandleOAL*)msg.arg1;
        s->asyncDecode();
    }

    void threadStopProcessing(const ThreadMessages::message& msg)
    {
        std::vector<ThreadDispatcher::message>& messages = _messages.lockMessages();
        size_t size = messages.size();
        for (size_t i = 0; i < size; ++i)
        {
            ThreadDispatcher::message& msg2 = messages[i];
            if (msg2.arg1 == msg.arg1)
            {
                messages.erase(messages.begin() + i);
                size--;
                i--;
            }
        }
        _messages.unlockMessages();
    }

    void StreamingSoundHandleOAL::asyncDecode()
    {
        ALint nump;
        alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &nump);

        ALuint buffers[STREAM_BUFFERS];

        alSourceUnqueueBuffers(_alSource, nump, buffers);
        check();
        decode(buffers, nump);


        ALint state = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
        if (state == AL_STOPPED)
            alSourcePlay(_alSource);
        check();
    }



    void StreamingSoundHandleOAL::decode(ALuint* buffers, int num)
    {
        int size;
        void* data = getSoundStreamTempBuffer(size);

        for (int i = 0; i < num; ++i)
        {
            log::messageln("decode %d", getTimeMS());
            ALuint buffer = buffers[i];
            size = _stream->decodeNextBlock(_looping, data, size);
            if (!size)
                break;
            alBufferData(buffer, _format, data, size, _rate);
            check();

            alSourceQueueBuffers(_alSource, 1, &buffer);
            check();
        }
    }


    StreamingSoundHandleOAL::StreamingSoundHandleOAL(): _stream(0)
    {
        alGenBuffers(STREAM_BUFFERS, _buffers);
        check();
    }

    StreamingSoundHandleOAL::~StreamingSoundHandleOAL()
    {
        alDeleteBuffers(STREAM_BUFFERS, _buffers);
        check();
    }

    void StreamingSoundHandleOAL::setStream(SoundStream* s)
    {
        _stream = s;

        int channels = s->getNumChannels();
        if (channels == 1)
            _format = AL_FORMAT_MONO16;
        if (channels == 2)
            _format = AL_FORMAT_STEREO16;

        _rate = s->getRate();
    }

    void StreamingSoundHandleOAL::stopAsyncDecode()
    {
        _messages.sendCallback(this, 0, threadStopProcessing, 0, true);
    }

    void StreamingSoundHandleOAL::_xplay()
    {
        _stream->reset();
        decode(_buffers, STREAM_BUFFERS);
    }

    void StreamingSoundHandleOAL::_xpause()
    {
        stopAsyncDecode();
        alSourceUnqueueBuffers(_alSource, STREAM_BUFFERS, _buffers);
        check();
    }

    void StreamingSoundHandleOAL::_xresume()
    {
        alSourceQueueBuffers(_alSource, STREAM_BUFFERS, _buffers);
        check();
    }

    void StreamingSoundHandleOAL::_xupdate()
    {
        if (_stream->isStreamEnded())
        {
            ALint state = 0;
            alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
            check();

            if (state == AL_STOPPED)
            {
                alSourcei(_alSource, AL_BUFFER, 0);
                _ended();
            }

            return;
        }

        ALint nump;
        alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &nump);

        if (nump)
        {
            //for (int i = 0; i < 1000; ++i)
            _messages.postCallback(this, 0, threadDecode, 0);

        }
    }

    timeMS StreamingSoundHandleOAL::_getPosition() const
    {
        return _stream->getPosition();

    }


    void StreamingSoundHandleOAL::_xstop()
    {
        stopAsyncDecode();
        //_stream->reset();
        if (_alSource)
            alSourcei(_alSource, AL_BUFFER, 0);
        check();
    }

    StreamingOggSoundHandleOAL::StreamingOggSoundHandleOAL(SoundOAL* snd) : StreamingSoundHandleOAL()
    {
        snd->initStream(_stream);
        setStream(&_stream);
    }
}