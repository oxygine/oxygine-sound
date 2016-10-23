#include "WavStream.h"
namespace oxygine
{
    struct  WAV_HEADER
    {
        char                RIFF[4];
        uint32_t             ChunkSize;
        char                WAVE[4];
        char                fmt[4];
        uint32_t             Subchunk1Size;
        uint16_t             AudioFormat;
        uint16_t             NumOfChan;
        uint32_t             SamplesPerSec;
        uint32_t             bytesPerSec;
        uint16_t             blockAlign;
        uint16_t             bitsPerSample;
        char                Subchunk2ID[4];
        uint32_t             Subchunk2Size;
    };

    WavStream::WavStream(): _fh(0), _close(false), _dataSize(0)
    {

    }

    WavStream::~WavStream()
    {
        release();
    }

    void WavStream::release()
    {
        if (_close && _fh)
            file::close(_fh);
        _fh = 0;
    }

    void WavStream::init(file::handle fh, bool close)
    {
        _fh = fh;
        _close = close;

        WAV_HEADER header;
        file::read(fh, &header, sizeof(header));
        _rate = header.SamplesPerSec;
        _channels = header.NumOfChan;

        _duration = (header.Subchunk2Size * 1000) / header.bytesPerSec;
        _pcm = header.Subchunk2Size  / _channels / 2;
        _dataSize = header.Subchunk2Size;
        _dataPos = 0;
        OX_ASSERT(header.Subchunk2Size == _pcm * _channels * 2);
        _ended = false;
    }

    int WavStream::decodeNextBlock(bool looped, void* data, int bufferSize)
    {
        wip
        int leftSize = _dataSize - _dataPos;
        if (leftSize <= bufferSize)
        {
            file::read(_fh, data, leftSize);
            if (looped)
            {
                file::seek(_fh, sizeof(WAV_HEADER), SEEK_SET);
                file::read(_fh, (char*)data + leftSize, bufferSize - leftSize);
                _dataPos = bufferSize - leftSize;
            }
            else
            {
                _dataPos = _dataSize;
                _ended = true;
            }
        }
        else
        {
            file::read(_fh, data, bufferSize);
            _dataPos += bufferSize;
        }

        return 0;
    }

    void WavStream::decodeAll(void* data, int bufferSize)
    {
        file::read(_fh, data, bufferSize);
        int p = file::tell(_fh);
        _dataPos = _dataSize;
        _ended = true;
    }

    void WavStream::reset()
    {
        file::seek(_fh, sizeof(WAV_HEADER), SEEK_SET);
        _dataPos = 0;
        _ended = false;
    }

    void WavStream::setPosition(int tm)
    {
    }


    int WavStream::getPosition() const
    {
        return 0;
    }

}