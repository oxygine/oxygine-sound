#pragma once
#include <vector>
#include <string>
#include "OggStream.h"
#include "../Sound.h"
#include "../oal.h"


namespace oxygine
{

    const int STREAM_BUFFERS = 3;

    class OggStream;

    class SoundOAL: public Sound
    {
    public:
        SoundOAL();
        ~SoundOAL();

        void init(std::vector<unsigned char>& buffer, bool swap);
        bool init(const char* path);


        unsigned int    getAlBuffer() const {return _alBuffer;}
        int             getDuration() const;
        unsigned int    getFormat() const {return _format;}

        void initStream(OggStream& stream);


    private:
        bool _init(OggStream& stream);

        unsigned int _format;
        int _timeMS;

        std::string _fileName;

        ALuint _alBuffer;
        std::vector<unsigned char> _fileBuffer;
    };

    class SoundHandleOAL;

    class StreamOAL : public Object
    {
    public:
        virtual bool play(SoundHandleOAL*) = 0;
        virtual void update(SoundHandleOAL*) = 0;
		virtual void pause(SoundHandleOAL*) = 0;
		virtual void resume(SoundHandleOAL*) = 0;
    };

    class StaticStreamOAL : public StreamOAL
    {
    public:

        StaticStreamOAL(ALuint buffer): _buffer(buffer) {}

        bool play(SoundHandleOAL* s);
        void update(SoundHandleOAL* s);
		void pause(SoundHandleOAL*s);
		void resume(SoundHandleOAL*s);

    protected:
        ALuint _buffer;
    };




    class OggStreamOAL: public StreamOAL
    {
    public:
        OggStreamOAL(SoundOAL*);

        bool play(SoundHandleOAL*) override;
        void update(SoundHandleOAL*) override;

		void resume(SoundHandleOAL*) override;
		void pause(SoundHandleOAL*) override;

		void asyncDecode(SoundHandleOAL* s);
    protected:
        void decode(SoundHandleOAL* s, ALuint* items, int num);

        SoundOAL* _snd;
        OggStream _stream;
        ALuint _buffers[STREAM_BUFFERS];
		ALuint _pos;
    };

    class SoundHandleOAL : public SoundHandle
    {
    public:
        SoundHandleOAL();

    protected:
        friend class StaticStreamOAL;
        friend class OggStreamOAL;


        void _init();
        void _play();
        void _pause();

        void _update();
        void _updateVolume();
        void _updatePitch();

		void _ended();

        ALuint _alSource;
        ALint _pos;

        StreamOAL* _stream;
    };

}
