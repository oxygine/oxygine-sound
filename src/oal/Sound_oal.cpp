#include "Sound_oal.h"
#include "../OggStream.h"
#include "../oal.h"

namespace oxygine
{
    using namespace std;

    void check();

    SoundOAL::SoundOAL(): _alBuffer(0), _format(AL_FORMAT_MONO16), _timeMS(0)
    {

    }

    SoundOAL::~SoundOAL()
    {
        check();
        if (_alBuffer)
        {
            alDeleteBuffers(1, &_alBuffer);
            _alBuffer = 0;
        }
        check();
    }

    bool SoundOAL::_init(OggStream& stream)
    {
        if (stream.isEmpty())
            return false;
        int channels = 0, rate = 0;
        unsigned int pcm = 0;
        stream.getDetails(&pcm, &channels, &rate, &_timeMS);

        if (channels == 1)
            _format = AL_FORMAT_MONO16;
        if (channels == 2)
            _format = AL_FORMAT_STEREO16;

        check();

        int len = pcm / rate;//len in seconds
        //todo, fix, use only pcm?

        //return;
        //return true;


        if (len > 3)//this sound would be streamed
            return true;

        vector<char> buff;
        buff.resize(pcm * 2 * channels);

        void* data = &buff.front();
        size_t size = buff.size();
        stream.decodeAll(data, (int)size);

        //const std::vector<char> &decoded = stream.getDecodedBuffer();

        alGenBuffers(1, &_alBuffer);
        check();

        alBufferData(_alBuffer, _format, data, (ALsizei)size, rate);
        check();

        return false;
    }

    void SoundOAL::init(std::vector<unsigned char>& buffer, bool swap)
    {
        OggStream stream;
        stream.init(&buffer.front(), (unsigned int)buffer.size());

        bool streaming = _init(stream);
        if (streaming)
        {
            if (swap)
                buffer.swap(_fileBuffer);
            else
                _fileBuffer = buffer;
        }
    }

    bool SoundOAL::init(const char* path)
    {
        _fileName = path;

        OggStream stream;
        bool res = stream.init(path);

        _init(stream);
        return res;
    }

    int SoundOAL::getDuration() const
    {
        return _timeMS;
    }

    void SoundOAL::initStream(OggStream& stream)
    {
        if (_fileName.empty())
            stream.init(&_fileBuffer.front(), (unsigned int)_fileBuffer.size());
        else
            stream.init(_fileName.c_str());
    }
}