#include "Channel_oal.h"
#include "Sound_oal.h"
#include "../OggStream.h"
#include "SoundSystem_oal.h"
#include "core/log.h"
#include "core/ox_debug.h"
#include "core/ThreadMessages.h"
#include "core/Object.h"
#include "pthread.h"
//#include <windows.h>

namespace oxygine
{
    using namespace std;

#define CHANNEL_DEBUG 0

#if CHANNEL_DEBUG
#define  LOG2DN(...)  log::messageln(__VA_ARGS__)
#define  LOG2D(...)  log::message(__VA_ARGS__)

#else
#define  LOG2DN(...) ((void)0)
#define  LOG2D(...)  ((void)0)
#endif

#if CHANNEL_DEBUG
#define  LOGDN(...)  log::messageln(__VA_ARGS__)
#define  LOGD(...)  log::message(__VA_ARGS__)

#else
#define  LOGDN(...)  ((void)0)
#define  LOGD(...)  ((void)0)
#endif

    void check()
    {
#ifdef CHANNEL_DEBUG
        int error = alGetError();

        if (error != AL_NO_ERROR)
        {
#ifdef CHANNEL_DEBUG
            log::messageln("AL error: %d\n", error);
#endif
            //OX_ASSERT(error == AL_NO_ERROR);
            //throw std::string("OpenAL error was raised.");
        }
#endif
    }

    pthread_t _thread = pthread_self();
    ThreadMessages _messages;
    pthread_key_t _tls;

    bool _synchronized = true;
    //const int BUFF_SIZE = 64000;
    int BUFF_SIZE = 12000;
    void* localMem = 0;



    enum evnt
    {
        evnt_exit = 1,
        evnt_stop,
        evnt_sync,
        evnt_decode,
    };

    void wrapSourceQueueBuffers(ALuint sid, ALsizei numEntries, const ALuint* bids)
    {
        /*
        LOGD("%d - alSourceQueueBuffers: ", getTimeMS());
        for (int i = 0; i < numEntries; ++i)
            LOGD("%d ", bids[i]);
            */

        if (numEntries)
        {
            alSourceQueueBuffers(sid, numEntries, bids);
        }
        check();
        //LOGDN("");
    }

    void wrapSourceUnqueueBuffers(ALuint sid, ALsizei numEntries, ALuint* bids)
    {
        if (numEntries)
            alSourceUnqueueBuffers(sid, numEntries, bids);
        /*
        check();
        LOGD("%d - alSourceUnqueueBuffers: ", getTimeMS());
        for (int i = 0; i < numEntries; ++i)
            LOGD("%d ", bids[i]);
        LOGDN("");
        */
    }

    void sync(const char* reason_)
    {
        if (!_synchronized)
        {
            static int sid = 0;
            sid++;
            LOGDN("synchronization '%d' from %s...", sid, reason_);
#if CHANNEL_DEBUG
            int t = getTimeMS();
#endif
            _messages.send(evnt_sync, 0, 0);
            LOGDN("synchronization '%d' done: %d", sid, getTimeMS() - t);

            _synchronized = true;
        }
    }

    void ChannelOAL::setBufferSize(int v)
    {
        BUFF_SIZE = v;
    }

    void* ChannelOAL::_staticThreadFunc(void* t)
    {
        void* mem = malloc(BUFF_SIZE);
        pthread_setspecific(_tls, mem);

        while (true)
        {
            ThreadMessages::message msg;
            _messages.get(msg);
            if (msg.msgid == evnt_exit)
            {
                LOGDN("evnt_exit");
                break;
            }
            switch (msg.msgid)
            {
                case evnt_sync:
                {
                    LOGDN("evnt_sync");
                }
                break;
                case evnt_decode:
                {
                    LOGDN("evnt_decode");

                    ChannelOAL* channel = (ChannelOAL*)msg.arg1;

                    if (!channel->_streamEnded)
                    {
                        ALuint buffer = 0;

                        int num = 0;
                        alGetSourcei(channel->_alSource, AL_BUFFERS_PROCESSED, &num);
                        //OX_ASSERT(num != 0);

                        if (num > 0)
                        {
                            wrapSourceUnqueueBuffers(channel->_alSource, 1, &buffer);
                            channel->updateStreamBuffers(&buffer, 1);
                        }
                    }

                    LOGDN("evnt_decode done");
                    //InterlockedDecrement(&channel->_tasks);
                }
                break;
                case evnt_stop:
                {
                    LOGDN("evnt_stop");

                    ChannelOAL* channel = (ChannelOAL*)msg.arg1;
                    channel->reset();
                }
                break;
            }
        }
        free(mem);
        return 0;
    }

    void ChannelOAL::runThread()
    {
        pthread_key_create(&_tls, 0);
        localMem = malloc(BUFF_SIZE);
        pthread_setspecific(_tls, localMem);

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&_thread, &attr, _staticThreadFunc, 0);

        //stopThread();
    }

    void ChannelOAL::stopThread()
    {
        _messages.post(evnt_exit, 0, 0);
        if (pthread_equal(_thread, pthread_self()))
            return;
        void* ptr = 0;
        pthread_join(_thread, &ptr);
        free(localMem);
        localMem = 0;
    }


    ChannelOAL::ChannelOAL():
        _soundSystem(0),
        _index(0),
        _alSource(0),
        _streamEnded(false),
        _paused(false),
        _free(true),
        _streaming(false)
    {
        for (int i = 0; i < STREAM_BUFFERS; ++i)
            _buffers[i] = 0;
    }

    void ChannelOAL::init(SoundSystemOAL* ss, int index, unsigned int source)
    {
        _soundSystem = ss;
        _index = index;
        _alSource = source;
    }

    ChannelOAL::~ChannelOAL()
    {

    }

    void ChannelOAL::reset()
    {
        _free = true;
        _paused = false;
        _stream.release();
        _streamEnded = false;

        setCoord(0, 0, 0);
        alSourcei(_alSource, AL_BUFFER, 0);
        check();

        alDeleteBuffers(STREAM_BUFFERS, _buffers);
        check();
        memset(_buffers, 0, sizeof(_buffers));
    }


    bool ChannelOAL::isFree() const
    {
        return _free;// && _tasks == 0;
    }

    SoundOAL* ChannelOAL::getSound()const
    {
        return (SoundOAL*)_desc.sound;
    }

    float ChannelOAL::getVolume()const
    {
        return _desc.volume;
    }

    float ChannelOAL::getPitch() const
    {
        return _desc.pitch;
    }

    int ChannelOAL::getPosition() const
    {
        return _stream.getCurrentMS();
    }

    void ChannelOAL::setVolume(float v)
    {
        //printf("volume %d: %.2f\n", _index, v);

        _desc.volume = v;
        alSourcef(_alSource, AL_GAIN, _soundSystem->getVolume() * v);
        check();
    }

    void ChannelOAL::setPitch(float v)
    {
        _desc.pitch = v;
        alSourcef(_alSource, AL_PITCH, v);
        check();
    }

    bool ChannelOAL::_updateFromStream(OggStream* stream, ALuint b)
    {
        void* data = pthread_getspecific(_tls);

        int decodedBytes = stream->decodeNextBlock(_desc.looping, data, BUFF_SIZE);
        _streamEnded = stream->isStreamEnded();

        if (!decodedBytes)
        {
            return false;
        }

        alBufferData(b, ((SoundOAL*)_desc.sound)->getFormat(), data, decodedBytes, stream->getRate());
        check();

        return true;
    }

    void ChannelOAL::play(const sound_desc& desc)
    {
        LOG2DN("%d ChannelOAL::play %s", _index, desc.id.c_str());

        OX_ASSERT(_free);

        _free = false;

        _stream.release();
        _streamEnded = false;

        _desc = desc;

        SoundOAL* sound = (SoundOAL*)desc.sound;

        alSourcef(_alSource, AL_GAIN, _soundSystem->getVolume() * _desc.volume);
        check();

        ALint v = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &v);

        alSourcef(_alSource, AL_PITCH, _desc.pitch);

        ALuint buffer = sound->getAlBuffer();
        if (buffer)
        {
            _streaming = false;
            alSourcei(_alSource, AL_BUFFER, buffer);

            check();
            alSourcei(_alSource, AL_LOOPING, desc.looping ? AL_TRUE : AL_FALSE);
            check();
        }
        else
        {
            _streaming = true;
            alSourcei(_alSource, AL_LOOPING, AL_FALSE);
            check();
            sound->initStream(_stream);
            if (desc.seek != 0)
                _stream.seekMS(desc.seek);
            alGenBuffers(STREAM_BUFFERS, _buffers);
            check();

            updateStreamBuffers(_buffers, STREAM_BUFFERS);
        }

        if (!desc.paused)
            alSourcePlay(_alSource);
        alGetSourcei(_alSource, AL_SOURCE_STATE, &v);
        check();
    }

    void ChannelOAL::continuePlay(const sound_desc& desc)
    {
        sync(__FUNCTION__);

        _desc = desc;
        _streamEnded = false;

        SoundOAL* sound = (SoundOAL*)desc.sound;
        alSourcef(_alSource, AL_GAIN, _soundSystem->getVolume() * _desc.volume);
        sound->initStream(_stream);
    }

    void ChannelOAL::pause()
    {
        if (_free)
            return;

        ALint v = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &v);


        if (v == AL_PLAYING)
            alSourcePause(_alSource);
        check();
        _paused = true;
    }

    void ChannelOAL::resume()
    {
        if (_free)
            return;


        ALint v = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &v);

        if (v == AL_PAUSED || v == AL_INITIAL)
            alSourcePlay(_alSource);

        check();
        _paused = false;
    }


    void ChannelOAL::stop(bool needSync)
    {
        LOG2DN("%d ChannelOAL::stop", _index);
        alSourceStop(_alSource);
        check();

        /*
        pthread_mutex_t mutex;
        ThreadMessages::messages &all = _messages.getMessages(mutex);
        MutexPthreadLock lock(mutex);

        ThreadMessages::messages::iterator i = all.begin();
        while(i != all.end())
        //for (ThreadMessages::messages::iterator i = all.begin(); i != all.end(); ++i)
        {
            const ThreadMessages::message &msg = *i;
            if (msg.arg1 == this)
            {
                i = all.erase(i);
            }
            else
            {
                ++i;
            }
        }

        all.insert(all.begin(), );
        */

        if (_streaming && needSync)
            sync(__FUNCTION__);
        reset();
    }

    void ChannelOAL::stop()
    {
        stop(true);
    }

    void ChannelOAL::seek(int tm)
    {
        /*
        if (_streaming)
        {
            _desc.seek = tm;
            continuePlay(_desc);
        }
        else
        {
            SoundOAL* sound = (SoundOAL*)_desc.sound;
            //alSourcei(_alSource, AL_BYTE_OFFSET, v);
        }

        check();
        */
    }

    void ChannelOAL::finished(bool needSync)
    {
        LOG2DN("%d ChannelOAL::finished", _index);
        if (_desc.cbDone)
        {
            _desc.cbDone(_desc.cbUserData, this, _desc);
            _desc.cbDone = 0;
        }

        stop(needSync);
    }

    void ChannelOAL::updateStreamBuffers(unsigned int* buffs, int num)
    {
        int i = 0;
        for (i = 0; i < num; ++i)
        {
            if (_updateFromStream(&_stream, buffs[i]) == false)
            {
                _stream.release();
                break;
            }
        }

        wrapSourceQueueBuffers(_alSource, i, buffs);
    }

    bool ChannelOAL::update()
    {
        if (_free)
            return false;

        if (_paused)
            return false;


        if (_streaming)
        {
            if (_streamEnded)
            {
                //if (!_stream.isEmpty())

                bool restarted = false;
                {
                    //_stream.release();

                    if (_desc.cbAboutEnd)
                    {
                        sound_desc::callback cb = _desc.cbAboutEnd;
                        _desc.cbAboutEnd = 0;
                        cb(_desc.cbUserData, this, _desc);

                        //stream was restarted
                        if (!_stream.isEmpty())
                        {
                            restarted = true;
                            //return true;
                        }
                    }
                }


                LOG2DN("%d ChannelOAL::update _streamEnded", _index);
                unsigned int processed[STREAM_BUFFERS];

                int nump;
                alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &nump);
                if (nump)
                    wrapSourceUnqueueBuffers(_alSource, nump, processed);
                if (restarted)
                {
                    if (nump)
                        updateStreamBuffers(processed, nump);
                    return true;
                }

                //stream ended, wait while last buffers are playing
                int nq = 0;
                alGetSourcei(_alSource, AL_BUFFERS_QUEUED, &nq);
                if (nq == 0)
                {
                    finished(false);
                }
            }
            else
            {

                int v = 0;
                alGetSourcei(_alSource, AL_SOURCE_STATE, &v);
                if (v == AL_STOPPED)
                {
                    sync(__FUNCTION__);

                    int num;
                    alGetSourcei(_alSource, AL_BUFFERS_QUEUED, &num);


                    unsigned int processed[STREAM_BUFFERS];
                    memcpy(processed, _buffers, sizeof(processed));

                    int nump;
                    alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &nump);
                    if (nump)
                        wrapSourceUnqueueBuffers(_alSource, nump, processed);


                    updateStreamBuffers(processed, nump);


                    alSourcePlay(_alSource);
                }
                else
                {
                    int num = 0;
                    alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &num);

                    for (int i = 0; i < num; ++i)
                    {
                        _synchronized = false;
                        LOGDN("_synchronized = false");
                        _messages.post(evnt_decode, this, 0);
                    }
                }
            }
        }
        else
        {
            ALint v = 0;
            alGetSourcei(_alSource, AL_SOURCE_STATE, &v);

            if (v == AL_STOPPED)
            {
                //log::messageln("ChannelOAL::finished2");
                finished(false);
            }
        }
        check();
        return true;
    }

    void ChannelOAL::_updateMasterVolume(float masterVolume)
    {
        alSourcef(_alSource, AL_GAIN, _desc.volume * masterVolume);
        check();
    }


    void  ChannelOAL::setCoord(float x, float y, float z)
    {
        alSource3f(_alSource, AL_POSITION, x, y, z);
    }
}
