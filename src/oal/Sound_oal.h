#pragma once
#include <vector>
#include <string>

#include "../Sound.h"
#include "../oal.h"


namespace oxygine
{
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
        SoundHandleOAL();

    protected:
        void _init();
        void _play();
        void _pause();

        void _update();
        void _updateVolume();
        void _updatePitch();

        ALuint _alSource;

        ALint _pos;
    };
}
