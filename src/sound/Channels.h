#pragma once
namespace oxygine
{
    template <class T>
    class Channels
    {
    public:
        typedef  std::vector<T> channels;
        channels _channels;

        void stop()
        {

            for (typename channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
            {
                T& channel = *i;
                channel.stop();
            }
        }

        void setVolume(float v)
        {
            for (typename channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
            {
                T& channel = *i;
                channel._updateMasterVolume(v);
            }
        }

        void update()
        {
            for (typename channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
            {
                T& channel = *i;
                channel.update();
            }
        }

        int getFreeNum()
        {

            int freeNum = 0;
            for (typename channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
            {
                T& channel = *i;

                if (channel.isFree())
                    ++freeNum;
            }

            return freeNum;
        }

        T* getFree()
        {
            for (typename channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
            {
                T& channel = *i;

                if (channel.isFree())
                    return &channel;
            }
            return 0;
        }
    };
}