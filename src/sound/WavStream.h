#pragma once
#include "SoundStream.h"
#include "core/file.h"

namespace oxygine
{
    class WavStream : public SoundStream
    {
    public:
        WavStream();
        ~WavStream();

        void init(file::handle fh, bool close);

        int  decodeNextBlock(bool looped, void* data, int bufferSize) override;
        void decodeAll(void* data, int bufferSize) override;

        void reset() override;
        void setPosition(int tm) override;
        int getPosition() const override;

    protected:
        void release();

        file::handle _fh;
        int _dataSize;
        int _dataPos;
        bool _close;
    };
}