#include "Sound_s3e.h"
#include "OggStream.h"

namespace oxygine
{
    using namespace std;

    void check();

    SoundS3E::SoundS3E():/*_format(AL_FORMAT_MONO16), */_timeMS(0), _rate(0), _channels(0)
    {

    }

    bool SoundS3E::_init(OggStream& stream)
    {
        int channels = 0;
        unsigned int pcm = 0;
        stream.getDetails(&pcm, &_channels, &_rate, &_timeMS);

        /*
        if (channels == 1)
            _format = AL_FORMAT_MONO16;
        if (channels == 2)
            _format = AL_FORMAT_STEREO16;
        */
        check();

        int len = pcm / _rate;//len in seconds
        //todo, fix, use only pcm?

        //return;

        int sizeAll = pcm * 2 * _channels;

        if (sizeAll > MAX_ALLOC) //this sound would be streamed
            return true;

        vector<unsigned char>& buff = _unpackedBuffer;
        buff.resize(sizeAll);

        void* data = &buff.front();
        size_t size = buff.size();

        stream.decodeAll(data, size);

        /*
        const std::vector<unsigned char> &decoded = stream.getDecodedBuffer();
        _unpackedBuffer = decoded;
        */

        /*

        alGenBuffers(1, &_alBuffer);

        alBufferData(_alBuffer, _format, &decoded.front(), (ALsizei)decoded.size(), rate);
        check();
        */

        return false;
    }

    void SoundS3E::init(std::vector<unsigned char>& buffer, bool swap)
    {
        OggStream stream;
        stream.init(&buffer.front(), buffer.size());

        bool streaming = _init(stream);
        if (streaming)
        {
            if (swap)
                buffer.swap(_fileBuffer);
            else
                _fileBuffer = buffer;
        }
    }

    void SoundS3E::init(const char* path)
    {
        _fileName = path;

        OggStream stream;
        stream.init(path);

        _init(stream);
    }

    int SoundS3E::getDuration() const
    {
        return _timeMS;
    }

    void SoundS3E::initStream(OggStream& stream)
    {
        if (_fileName.empty())
            stream.init(&_fileBuffer.front(), (unsigned int)_fileBuffer.size());
        else
            stream.init(_fileName.c_str());
    }

    SoundS3E::~SoundS3E()
    {
        /*
        if (_alBuffer)
        {
            alDeleteBuffers(1, &_alBuffer);
            _alBuffer = 0;
        }
        check();
        */
    }
}