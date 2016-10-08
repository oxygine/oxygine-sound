#pragma once
#include "Channel.h"
#include "Sound_s3e.h"
#include "OggStream.h"

namespace oxygine
{
    class SoundS3E;
    class SoundSystemS3E;
    class OggStream;

    const int STREAM_BUFFERS = 3;


    class ChannelS3E: public Channel
    {
    public:
        ChannelS3E(SoundSystemS3E* ss, int index);
        ~ChannelS3E();

        void play(const sound_desc& desc);
        void pause();
        void resume();
        bool update();
        void clear();
        void stop();

        void setVolume(float v);

        SoundS3E*           getSound() const;
        const sound_desc&   getSoundDesc() const {return _desc;}
        float               getVolume() const;

        Mutex&              GetMutex() { return _MutexSoundDecode; }

    protected:
        friend class SoundSystemS3E;
        void _updateMasterVolume(float masterVolume);

    private:
        void finished();
        struct mixdata
        {
            int _w;
            int _l;
        };

        mixdata _mix;

        static int32 _genAudio(void* systemData, void* userData);
        int32 genAudio(void* systemData);

        static int32 _sampleEnd(void* systemData, void* userData);
        int32 sampleEnd(void* systemData);

        SoundSystemS3E* _soundSystem;

        int _index;

        sound_desc _desc;

        bool _isStereo;

        bool _free;
        bool _paused;

        OggStream _stream;
        bool _streamEnded;
        bool _freePrepare;

        bool _stopProcess;

        Mutex _MutexSoundDecode;

        short _buffer[ 4000/*MAX_READ*/ ];

        std::vector<unsigned char> _playingBuffer;

        static short*   _lastTarget;
        static int      _samples;
    };
}
