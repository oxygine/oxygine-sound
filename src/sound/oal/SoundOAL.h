#pragma once
#include <vector>
#include <string>
#include "../OggStream.h"
#include "../Sound.h"
#include "../oal.h"


namespace oxygine
{
    class WavStream;
    class OggStream;

    class SoundOAL: public Sound
    {
    public:
        enum Type
        {
            Unknown, Buffer, Ogg, Wav
        };

        SoundOAL(const std::string& path, file::handle fh);
        ~SoundOAL();

        void init(std::vector<unsigned char>& buffer, bool swap);
        bool init(const char* path);


        unsigned int    getAlBuffer() const {return _alBuffer;}
        int             getDuration() const;
        unsigned int    getFormat() const {return _format;}
        Type            getType() const { return _type; }

        void initStream(OggStream& stream);
        void initStream(WavStream& stream);


    private:
        bool _init(SoundStream& stream);

        unsigned int _format;
        int _timeMS;

        std::string _fileName;

        Type _type;

        ALuint _alBuffer;
        std::vector<unsigned char> _fileBuffer;
    };
}