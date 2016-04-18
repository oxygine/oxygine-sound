#pragma once
#include "../Channel.h"
#include "../OggStream.h"
#include "Sound_oal.h"

//#include "AL/al.h"

namespace oxygine
{
    class SoundOAL;
    class SoundSystemOAL;
    class OggStream;

    const int STREAM_BUFFERS = 3;

    class ChannelOAL: public Channel
    {
    public:
        static void runThread();
        static void stopThread();
        //control size of buffer for streaming sound
        static void setBufferSize(int);

        ChannelOAL();
        ~ChannelOAL();

        void play(const sound_desc& desc);
        void continuePlay(const sound_desc&);
        void pause();
        void resume();
        bool update();
        void stop();
        void seek(int tm);

        bool isFree() const;

        void setVolume(float v);
        void setPitch(float v);

        SoundOAL*           getSound() const;
        const sound_desc&   getSoundDesc() const {return _desc;}
        float               getVolume() const;
        float               getPitch() const;
        int                 getPosition() const;

        void _updateMasterVolume(float masterVolume);

        void setCoord(float x, float y, float z);

    protected:
        friend class SoundSystemOAL;
        void init(SoundSystemOAL* ss, int index, unsigned int source);

    private:
        void stop(bool sync);
        void reset();

        static void* _staticThreadFunc(void* t);
        void finished(bool sync);

        SoundSystemOAL* _soundSystem;

        int _index;
        ALuint _alSource;

        sound_desc _desc;

        bool _free;
        bool _paused;
        bool _stopped;
        bool _streaming;

        OggStream _stream;
        bool _streamEnded;
        unsigned int _buffers[STREAM_BUFFERS];//streaming buffers;


        void updateStreamBuffers(unsigned int* buffs, int num);
        bool _updateFromStream(OggStream* stream, unsigned int b);
    };
}
