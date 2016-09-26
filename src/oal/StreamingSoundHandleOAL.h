#pragma once
#include "Sound_oal.h"

namespace oxygine
{
    class StreamingSoundHandleOAL : public SoundHandleOAL
    {
    public:
        StreamingSoundHandleOAL();
        ~StreamingSoundHandleOAL();

        void setStream(SoundStream*);

        void asyncDecode();

    protected:
        void _xplay() override;
        void _xpause() override;
        void _xresume() override;
        void _xupdate() override;
        void _xstop() override;

        void decode(ALuint* items, int num);

    private:
        SoundStream* _stream;
        ALuint _format;
        ALuint _rate;
        ALuint _buffers[STREAM_BUFFERS];
    };


    class SoundOAL;
    class StreamingOggSoundHandleOAL : public StreamingSoundHandleOAL
    {
    public:
        StreamingOggSoundHandleOAL(SoundOAL*);

    protected:
        OggStream _stream;

    };

}
