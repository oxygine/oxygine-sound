#include "MemoryStream.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>

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
                OX_ASSERT(0);
                break;

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

    fileRMem::fileRMem(const void* data, size_t size, delfn del, delself fns): _data(data), _size(size), _pos(0), _delete(del), _deleteSelf(fns)
    {

    }

    void fileRMem::init(const void* data, size_t size, delfn fn /*= notdel*/, delself fns /*= notdelself*/)
    {
        release();
        _data = data;
        _size = size;
        _delete = fn;
        _deleteSelf = fns;
    }

    void fileRMem::release()
    {
        _delete(const_cast<void*>(_data));
        _data = 0;
        _size = 0;
        _pos = 0;
        _deleteSelf(this);
    }

    unsigned int fileRMem::read(void* dest, unsigned int size)
    {
        unsigned int bytes = std::min(size, _size - _pos);
        memcpy(dest, (const char*)_data + _pos, bytes);
        return bytes;
    }

    unsigned int fileRMem::write(const void* src, unsigned int size)
    {
        return 0;
    }

    unsigned int fileRMem::getSize() const
    {
        return _size;
    }

    int fileRMem::seek(unsigned int offset, int whence)
    {
        switch (whence)
        {
            case SEEK_CUR:
            {
                size_t pos = _pos + offset;
                if (pos > _size)
                    return -1;
                _pos += offset;
            }
            break;
            case SEEK_END:
            {
                OX_ASSERT(offset == 0);
                _pos = _size;
                return 0;
            }
            break;
            case SEEK_SET:
            {
                if (offset > _size)
                    return -1;

                _pos = offset;
            }
            break;

            default:
                OX_ASSERT(0);
                break;
        }

        return 0;
    }

    unsigned int fileRMem::tell() const
    {
        return _pos;
    }

}