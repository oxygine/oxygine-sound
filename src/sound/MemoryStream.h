#pragma once
#include <stddef.h>

namespace oxygine
{
    class MemoryStream
    {
    public:
        MemoryStream(const void* data, unsigned int bytesCount);

        unsigned int read(void* ptr, size_t size, size_t count);

        int seek(int offset, int seek_type);
        void reset() {_pos = 0;}

        unsigned int    getPosition() const;
        const void*     getBegin() const;
        unsigned int    getLength() const { return _bytesCount; };

    private:
        const unsigned char* _data;
        unsigned int _bytesCount;
        unsigned int _pos;
    };
}
