#include "SoundOAL.h"
#include "../OggStream.h"
#include "../oal.h"
#include "SoundSystemOAL.h"
#include "core/ThreadDispatcher.h"
#include "StaticSoundHandleOAL.h"
#include "StreamingSoundHandleOAL.h"

namespace oxygine
{

    using namespace std;

    void OAL_CHECK();

    SoundOAL::SoundOAL(const string& path, file::handle fh): _alBuffer(0), _format(0), _timeMS(0), _fileName(path), _type(Unknown)
    {
        OggStream oggStream;
        WavStream wavStream;
        char header[4];
        if (file::read(fh, header, 4) != 4)
            return;

        file::seek(fh, 0, SEEK_SET);

        SoundStream* stream = 0;

        if (header[0] == 'R' && header[1] == 'I'  && header[2] == 'F' && header[3] == 'F')
        {
            stream = &wavStream;
            wavStream.init(fh, false);
            _type = Wav;
        }
        else if (header[0] == 'O' && header[1] == 'g'  && header[2] == 'g' && header[3] == 'S')
        {
            stream = &oggStream;
            oggStream.init(fh, false);
            _type = Ogg;
        }


        bool streaming = _init(*stream);
        if (streaming)
        {
            if (_fileName.empty())
            {
                file::seek(fh, 0, SEEK_SET);
                file::buffer bf;
                file::read(fh, bf);
                std::swap(_fileBuffer, bf.data);
            }
        }

        file::close(fh);
    }

    SoundOAL::~SoundOAL()
    {
        if (_alBuffer)
        {
            alDeleteBuffers(1, &_alBuffer);
            OAL_CHECK();
            _alBuffer = 0;
        }
    }

    bool SoundOAL::_init(SoundStream& stream)
    {
        if (stream.isStreamEnded())
            return false;


        _timeMS = stream.getDuration();


        int channels = stream.getNumChannels();
        if (channels == 1)
            _format = AL_FORMAT_MONO16;
        if (channels == 2)
            _format = AL_FORMAT_STEREO16;

#ifdef OX_DEBUG
        const int MAX_DURATION = 3500;
#else
        const int MAX_DURATION = 3500;
#endif

        if (_timeMS > MAX_DURATION)//this sound would be streamed
            return true;

        vector<char> buff;
        int pcm = stream.getPCM();
        buff.resize(pcm * 2 * channels);

        void* data = &buff.front();
        size_t size = buff.size();
        stream.decodeAll(data, (int)size);

        alGenBuffers(1, &_alBuffer);
        OAL_CHECK();

        int rate = stream.getRate();

        alBufferData(_alBuffer, _format, data, (ALsizei)size, rate);
        _type = Buffer;
        OAL_CHECK();

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

    void SoundOAL::initStream(WavStream& stream)
    {
        if (_fileName.empty())
            stream.init(&_fileBuffer.front(), (unsigned int)_fileBuffer.size());
        else
            stream.init(_fileName.c_str());
    }
}