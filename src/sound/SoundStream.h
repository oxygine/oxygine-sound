#pragma once
namespace oxygine
{
    class SoundStream
    {
    public:
        virtual int decodeNextBlock(bool looped, void* data, int bufferSize) = 0;
        virtual void reset() = 0;

        virtual void setPosition(int tm) = 0;

        virtual bool isStreamEnded() const = 0;
        virtual int getRate() const = 0;
        virtual int getNumChannels() const = 0;
        virtual int getPosition() const = 0;;

        virtual ~SoundStream() {}
    };
}