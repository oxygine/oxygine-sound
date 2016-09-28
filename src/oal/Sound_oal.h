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



    class SoundHandleOAL : public SoundHandle
    {
    public:
        static SoundHandleOAL* create(Sound*);

        SoundHandleOAL();
        bool isPlaying() const { return _alSource != 0; }

    protected:
        friend class StaticStreamOAL;
        friend class OggStreamOAL;


        void _init();
        void _play();
        void _pause();
        void _resume();
        void _stop();

        void _update();
        void _updateVolume();
        void _updatePitch();
        void _updateLoop() override;

        void _ended();

        virtual void _xplay() {}
        virtual void _xpause() {}
        virtual void _xupdate() {}
        virtual void _xresume() {}
        virtual void _xstop() {}
        virtual void _xupdateLoop() {}

        ALuint _alSource;
        ALint _pos;

        //StreamOAL* _stream;
    };


    class SoundStream;


}
