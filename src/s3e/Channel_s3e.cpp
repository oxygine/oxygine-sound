#include "Channel_s3e.h"
#include "Sound_s3e.h"
#include "OggStream.h"
#include "SoundSystem_s3e.h"
#include "core/logging.h"
#include "core/ox_debug.h"
#include <stdlib.h>
namespace oxygine
{
    // Greatest Common Divisor
    int GCD(int a, int b)
    {
        while (1)
        {
            a = a % b;
            if (a == 0)
                return b;
            b = b % a;

            if (b == 0)
                return a;
        }
    }

    S3E_INLINE int16 ClipToInt16(int32 sval)
    {
        enum
        {
            minval =  INT16_MIN,
            maxval =  INT16_MAX,
            allbits = UINT16_MAX
        };

        // quick overflow test, the addition moves valid range to 0-allbits
        if ((sval - minval) & ~allbits)
        {
            // we overflowed.
            if (sval > maxval)
                sval = maxval;
            else if (sval < minval)
                sval = minval;
        }

        return (int16)sval;
    }


    void check()
    {
        return;
    }

    short* ChannelS3E::_lastTarget = NULL;
    int ChannelS3E::_samples = 0;

    ChannelS3E::ChannelS3E(SoundSystemS3E* ss, int index):
        _soundSystem(ss),
        _index(index),
        _streamEnded(false),
        _paused(false),
        _free(true),
        _isStereo(false),
        _freePrepare(false),
        _stopProcess(false)
    {
    }

    ChannelS3E::~ChannelS3E()
    {

    }

    void ChannelS3E::clear()
    {
        _stream.release();
        _stopProcess = true;
        //_freePrepare = true;
    }

    SoundS3E* ChannelS3E::getSound()const
    {
        return (SoundS3E*)_desc.sound;
    }

    float ChannelS3E::getVolume()const
    {
        return _desc.volume;
    }

    void ChannelS3E::setVolume(float v)
    {
        //printf("volume %d: %.2f\n", _index, v);

        _desc.volume = v;
        int c = S3E_SOUND_MAX_VOLUME * _soundSystem->getVolume() * v;
        s3eSoundChannelSetInt(_index, S3E_CHANNEL_VOLUME, c);
    }

#define min(x,y) (((x)<(y))?(x):(y))

    int32 ChannelS3E::genAudio(void* systemData)
    {
        s3eSoundGenAudioInfo* info = (s3eSoundGenAudioInfo*)systemData;

        int inputSampleSize = _isStereo ? 2 : 1;
        static int  nStep = 0;
        if (_lastTarget != info->m_Target || _samples != info->m_NumSamples)
        {
            memset(info->m_Target, 0, info->m_NumSamples * inputSampleSize * sizeof(int16));
            _samples = info->m_NumSamples;
            _lastTarget = info->m_Target;
        }


        static int nIterations = 0;
        int needSamples = info->m_NumSamples;

        int nChannels = ((SoundS3E*)_desc.sound)->getChannels();
        int nChannelsReal =  nChannels;


        float fRela = (float)(_mix._w * nChannels) / (float)_mix._l;
        int needOrigSamples = (float)needSamples * fRela;


        nChannels = 1;

        needOrigSamples = min(needOrigSamples, MAX_READ);

        bool bUseRel = true;
        //log::messageln("a1");
        int size = needOrigSamples * (sizeof(short));
        nIterations++;

        int haveOrigSamples = 0;

        haveOrigSamples = _stream.GetDecodeBlock(_desc.looping, _buffer, size) / (sizeof(short));



        int haveSamples = (float)haveOrigSamples * (bUseRel ? fRela : 1);


        needSamples = min(haveSamples, needSamples);


        if (!needSamples || _stopProcess)
        {
            if (_stream.IsStreamEnded() || _stopProcess)
            {
                //_stream.clearData();
                _freePrepare = true;
                _stopProcess = false;
                info->m_EndSample = true;
                return 0;
            }
            return 1;
        }



        if (_isStereo)
        {
            if (nChannelsReal == 1)
            {
                short* t = info->m_Target;
                for (int i = 0; i != needSamples; ++i)
                {
                    int nIndex1 = (int)((float)i * (bUseRel ? fRela : 1));
                    int nIndex2 = (int)((float)(i) * (bUseRel ? fRela : 1));
                    *t = *t + (short)(((float)(_buffer[ nIndex1 ])) * _desc.volume);
                    ++t;
                    *t = *t + (short)((float)(_buffer[ nIndex2 ]) * _desc.volume);
                    ++t;

                }
            }
            else if (nChannelsReal == 2)
            {
                needSamples = needSamples * 2 > haveSamples ? haveSamples / 2 : needSamples;
                short* t = info->m_Target;
                for (int i = 0; i != needSamples; ++i)
                {
                    int nIndex = (int)((float)i * (bUseRel ? fRela : 1));

                    short left = *t + (short)((float)_buffer[ nIndex ] * (_desc.volume / 2.f));
                    short right = *t + (short)((float)_buffer[ nIndex + 1 ] * (_desc.volume / 2.f));

                    *t++ = left;
                    *t++ = right;

                }
            }
        }
        else
        {
            if (nChannelsReal == 1)
            {
                short* t = info->m_Target;
                for (int i = 0; i != needSamples; ++i)
                {
                    int nIndex1 = (int)((float)i * (bUseRel ? fRela : 1));
                    *t = *t + (short)(((float)(_buffer[ nIndex1 ])) * _desc.volume);
                    ++t;

                }
            }
            else if (nChannelsReal == 2)
            {
                needSamples = needSamples * 2 > haveSamples ? haveSamples / 2 : needSamples;
                short* t = info->m_Target;
                for (int i = 0; i != needSamples; ++i)
                {
                    int nIndex = (int)((float)i * (bUseRel ? fRela : 1));

                    short left = *t + (short)((float)_buffer[ nIndex ] * (_desc.volume / 2.f));
                    short right = *t + (short)((float)_buffer[ nIndex + 1 ] * (_desc.volume / 2.f));

                    *t++ = left + right;;
                }
            }
        }




        return needSamples;
    }

    int32 ChannelS3E::_genAudio(void* systemData, void* userData)
    {
        int32 samples = ((ChannelS3E*)userData)->genAudio(systemData);
        return samples;
    }

    int32 ChannelS3E::sampleEnd(void* systemData)
    {
        s3eSoundEndSampleInfo* info = (s3eSoundEndSampleInfo*)systemData;
        finished();

        return 0;
    }

    int32 ChannelS3E::_sampleEnd(void* systemData, void* userData)
    {
        return ((ChannelS3E*)userData)->sampleEnd(systemData);
    }

    void ChannelS3E::play(const sound_desc& desc)
    {
        _free = false;
        _stopProcess = false;

        _stream.release();



        _desc = desc;

        SoundS3E* sound = (SoundS3E*)desc.sound;

        int ou = s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ);

        s3eSoundError error = s3eSoundGetError();
        s3eSoundChannelSetInt(_index, S3E_CHANNEL_RATE, sound->getRate() * sound->getChannels());

        _isStereo = s3eSoundGetInt(S3E_SOUND_STEREO_ENABLED) ? true : false;

        const std::vector<unsigned char>& buffer = sound->getDecodedBuffer();
//      if ( !buffer.empty())
//      {
//          const void *data = &buffer.front();
//          size_t size = buffer.size();
//          s3eSoundChannelPlay(_index, (int16*)data, size / 2 , desc.looping ? 0 : 1, 0);
//          s3eSoundChannelRegister(_index, S3E_CHANNEL_END_SAMPLE, _sampleEnd, this);
//      }
//      else


        int input_freq = sound->getRate();
        sound->initStream(_stream);

        if (!buffer.empty())
        {
            _stream.swapToStream(buffer);
        }
        else
        {
            _stream.ReSizeBuffers();
            _stream.decodeNextBlockProcess(_desc.looping);
        }


        s3eSoundChannelRegister(_index, S3E_CHANNEL_GEN_AUDIO, _genAudio, this);
        if (_isStereo)
        {
            s3eSoundChannelRegister(_index, S3E_CHANNEL_GEN_AUDIO_STEREO, _genAudio, this);
        }
        printf("Stereo HERE %d\n", (int)_isStereo);

        printf("Channels Count: %d\n", (int)sound->getChannels());



        int output_freq = s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ);
        int gcd = GCD(input_freq , output_freq);
        printf("input_freq %d\n", input_freq);
        printf("output_freq %d\n", output_freq);
        _mix._w = input_freq / gcd;
        _mix._l = output_freq / gcd;

        short data[ 16 ] ;
        s3eSoundChannelPlay(_index, (int16*)data, 8, 1, 0);


        printf("w: %d, l: %d \n", _mix._w, _mix._l);
    }

    void ChannelS3E::pause()
    {
        if (_free)
            return;

        /*
        ALint v = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &v);


        if (v == AL_PAUSED || v == AL_INITIAL)
            alSourcePlay(_alSource);
        */

        check();
        _paused = true;
        s3eSoundChannelPause(_index);
    }

    void ChannelS3E::resume()
    {
        if (_free)
            return;
        /*
        ALint v = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &v);

        if (v == AL_PAUSED || v == AL_INITIAL)
            alSourcePlay(_alSource);
        */

        check();
        _paused = false;
        s3eSoundChannelResume(_index);
    }

    void ChannelS3E::stop()
    {
        if (_desc.cbDone)
        {
            _desc.cbDone(_desc.cbUserData, this, _desc);
            _desc.cbDone = 0;
        }

        _stopProcess = true;

        //alSourceStop(_alSource);

        clear();

        check();
    }

    void ChannelS3E::finished()
    {
        stop();
    }

    bool ChannelS3E::update()
    {
        if (_free)
            return false;
        if (_freePrepare)   // Thread Save Block FREE.
        {
            _freePrepare = false;
            _free = true;
            _stream.clearData();
            _stream.ClearBuffers();
            return false;
        }
        if (!_stopProcess)
        {
            _stream.decodeNextBlockProcess(_desc.looping);
        }
        if (_paused)
            return false;
        /*

        if (!_stream.isEmpty())
        {
            int processed = 0;
            bool active = true;

            alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &processed);
            int t = processed;
            check();

            bool play = false;
            while(processed--)
            {
                ALuint buffer = 0;

                alSourceUnqueueBuffers(_alSource, 1, &buffer);
                check();

                active = _updateFromStream(&_stream, buffer);
                if (active)
                {
                    alSourceQueueBuffers(_alSource, 1, &buffer);
                    check();
                    play = true;
                }
            }

            if (v == AL_STOPPED)
            {
                if (play)
                {
                    //it could be stopped if we had lag
                    alSourcePlay(_alSource);
                }
                else
                    finished();
            }
        }
        else
        {
            if (v == AL_STOPPED)
                finished();
        }
        check();
        */
        return true;
    }

    void ChannelS3E::_updateMasterVolume(float masterVolume)
    {
        //alSourcef(_alSource, AL_GAIN, _desc.volume * masterVolume);
        check();
    }
}