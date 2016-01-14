#pragma once
#include "Sound.h"
#include <vector>
#include <string>
#include "s3eSound.h"

namespace oxygine
{
    class OggStream;

    class SoundS3E: public Sound
    {
    public:
        SoundS3E();
        ~SoundS3E();

        void init(std::vector<unsigned char>& buffer, bool swap);
        void init(const char* path);


        //unsigned int  getAlBuffer() const {return _alBuffer;}
        const std::vector<unsigned char>&   getDecodedBuffer() const {return _unpackedBuffer;}
        int             getDuration() const;
        int             getRate() const {return _rate;}
        unsigned int    getFormat() const {return _format;}
        unsigned int    getChannels() const {return _channels;}


        void initStream(OggStream& stream);


    private:
        bool _init(OggStream& stream);

        unsigned int _format;
        int _rate;
        int _timeMS;
        int _channels;

        std::string _fileName;

        std::vector<unsigned char> _unpackedBuffer;
        std::vector<unsigned char> _fileBuffer;
    };
}