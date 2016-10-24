#pragma once
#include <stddef.h>
#include "core/FileSystem.h"

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

    class fileRMem : public file::fileHandle
    {
    public:
        typedef void(*delfn)(void*);
        static void notdel(void*) {}
        static void delcpp(void* ptr) { delete[](char*)ptr; }

        typedef void(*delself)(fileRMem*);
        static void notdelself(fileRMem*) {}

        fileRMem(const void* data, size_t size, delfn fn = notdel, delself fns = notdelself);
        ~fileRMem() { release(); }

        void init(const void* data, size_t size, delfn fn = notdel, delself fns = notdelself);

        void release() override;
        unsigned int read(void* dest, unsigned int size) override;
        unsigned int write(const void* src, unsigned int size) override;
        unsigned int getSize() const override;
        int seek(unsigned int offset, int whence) override;
        unsigned int tell() const override;

    protected:
        const void* _data;
        size_t _size;
        size_t _pos;
        delfn _delete;
        delself _deleteSelf;
    };

}
