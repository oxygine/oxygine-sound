#include "MemoryStream.h"
#include <stdio.h>
#include <string.h>

namespace oxygine
{
    MemoryStream::MemoryStream(const void* data, unsigned int bytesCount):
        _data((const unsigned char*)data),
        _pos(0),
        _bytesCount(bytesCount)
    {
    }

    unsigned int MemoryStream::read(void* ptr, size_t size, size_t count)
    {
        unsigned int real = (_bytesCount - _pos) / size;
        real = (unsigned int)(count > real ? real* size : count * size);
        memcpy(ptr, _data + _pos, real);
        _pos += real;
        return real;
    }

    int MemoryStream::seek(int seek_offset, int type)
    {
        int pos = 0;

        switch (type)
        {
            default:
            case SEEK_CUR:
            {
                pos = _pos + seek_offset;
                if (pos < 0 || pos >= (int)_bytesCount)
                {
                    return -1;
                }
                _pos += seek_offset;
                return 0;
            }
            break;
            case SEEK_END:
            {
                pos = _bytesCount + seek_offset;
                if (pos < 0 || pos > (int)_bytesCount)
                {
                    //WTF??? is it????
                    pos = seek_offset;
                    if (pos < 0 || pos > (int)_bytesCount)  //wtf????
                    {
                        return -1;
                    }
                }
                _pos = pos;
                return 0;
            }
            break;
            case SEEK_SET:
            {
                if (seek_offset < 0 || (unsigned int)seek_offset > _bytesCount)
                {
                    return -1;
                }
                _pos = seek_offset;
                return 0;
            }
            break;
        }
    }

    unsigned int MemoryStream::getPosition() const
    {
        return _pos;
    }

    const void* MemoryStream::getBegin() const
    {
        return (const void*) _data;
    }
}