#include "OggStream.h"
#include "core/file.h"


#if TREMOLO
#include "ogg.h"
#else
#include "ogg/ogg.h"
#endif

#include "core/ox_debug.h"

//#include "s3eDevice.h"

#ifndef _WIN32
//typedef ogg_int64_t __int64;
#endif

namespace oxygine
{

    size_t ms_ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
    {
        return ((MemoryStream*)datasource)->read(ptr, size, nmemb);
    }

    int ms_ov_seek_func(void* datasource, ogg_int64_t offset, int whence)
    {
        return ((MemoryStream*)datasource)->seek((int)offset, whence);
    }

    int ms_ov_close_func(void* datasource)
    {
        ((MemoryStream*)datasource)->reset();
        return 0;
    }

    long int ms_ov_tell_func(void* datasource)
    {
        return ((MemoryStream*)datasource)->getPosition();
    }

    ov_callbacks cb_memoryStream =
    {
        ms_ov_read_func,
        ms_ov_seek_func,
        ms_ov_close_func,
        ms_ov_tell_func
    };


    size_t oxfile_ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
    {
        file::handle h = (file::handle)datasource;
        return file::read(h, ptr, (unsigned int)(size * nmemb));
    }

    int oxfile_ov_seek_func(void* datasource, ogg_int64_t offset, int whence)
    {
        file::handle h = (file::handle)datasource;
        return file::seek(h, (int)offset, whence);
    }

    int oxfile_ov_close_func(void* datasource)
    {
        file::handle h = (file::handle)datasource;
        file::close(h);
        return 0;
    }

    long int oxfile_ov_tell_func(void* datasource)
    {
        file::handle h = (file::handle)datasource;
        return file::tell(h);
    }

    ov_callbacks cb_oxfile =
    {
        oxfile_ov_read_func,
        oxfile_ov_seek_func,
        oxfile_ov_close_func,
        oxfile_ov_tell_func
    };



    size_t file_ov_read_func(void* ptr, size_t size, size_t nmemb, void* datasource)
    {
        FILE* file = (FILE*)datasource;
        return fread(ptr, size, nmemb, file);
    }

    int file_ov_seek_func(void* datasource, ogg_int64_t offset, int whence)
    {
        FILE* file = (FILE*)datasource;
        return fseek(file, (int)offset, whence);
    }

    int file_ov_close_func(void* datasource)
    {
        FILE* file = (FILE*)datasource;
        fclose(file);
        return 0;
    }

    long int file_ov_tell_func(void* datasource)
    {
        FILE* file = (FILE*)datasource;
        return ftell(file);
    }

    ov_callbacks cb_fileStream =
    {
        file_ov_read_func,
        file_ov_seek_func,
        file_ov_close_func,
        file_ov_tell_func
    };



    OggStream::OggStream():
        _memStream(0, 0), _section(0), _streamEnded(false),
        _file(0), _empty(true), _info(0)
    {
        memset(&_vorbisFile, 0, sizeof(_vorbisFile));
    }

    OggStream::~OggStream()
    {
        release();
    }

    void OggStream::getDetails(unsigned int* pcm, int* channels, int* rate, int* timeMS)
    {
        vorbis_info* vi = ov_info(&_vorbisFile, -1);

        if (pcm)
        {
            *pcm = (unsigned int)ov_pcm_total(&_vorbisFile, -1);
        }

        if (channels)
        {
            *channels = vi->channels;
        }

        if (rate)
        {
            *rate = (int)vi->rate;
        }

        if (timeMS)
        {
            *timeMS = (int)(ov_time_total(&_vorbisFile, -1));
        }
    }

    bool OggStream::init(const void* data, unsigned int len)
    {
        release();
        _memStream = MemoryStream(data, len);

        return _init(cb_memoryStream, &_memStream);
    }

    bool  OggStream::init(const char* name)
    {
        release();

#ifdef __S3E__
        _file = fopen(name, "rb");
        return _init(cb_fileStream, _file);
#else
        _oxfile = file::open(name, "srb");
        if (_oxfile)
            return _init(cb_oxfile, _oxfile);
        return false;
#endif
    }

    bool OggStream::_init(const ov_callbacks& cb, void* userData)
    {
        _empty = false;
        _section = 0;
        _streamEnded = false;

        if (ov_open_callbacks(userData, &_vorbisFile, 0, 0, cb) < 0)
            return false;

        char** ptr = ov_comment(&_vorbisFile, -1)->user_comments;
        if (ptr)
        {
        }
        _info = ov_info(&_vorbisFile, -1);


        return true;
    }

    int OggStream::getRate() const
    {
        return (int)_info->rate;
    }

    int OggStream::getNumChannels() const
    {
        return _info->channels;
    }

    int OggStream::getCurrentPCM() const
    {
        int r = (int)ov_pcm_tell(const_cast<OggVorbis_File*>(&_vorbisFile));
        return r;
    }

    int OggStream::getCurrentMS() const
    {
        int r = (int)ov_time_tell(const_cast<OggVorbis_File*>(&_vorbisFile));
        return r;
    }

    int OggStream::getTotalPCM() const
    {
        return (int)ov_pcm_total(const_cast<OggVorbis_File*>(&_vorbisFile), -1);
    }

    int OggStream::getTotalMS() const
    {
        return (int)ov_time_total(const_cast<OggVorbis_File*>(&_vorbisFile), -1);
    }

    void OggStream::decodeAll(void* data, int bufferSize)
    {
        while (1)
        {
            /*
            int r = (int)ov_read(&_vorbisFile,
                (char*)data, bufferSize,
                0, 2, 1, &_section);
                */

            int r = (int)ov_read(&_vorbisFile,
                                 (char*)data, bufferSize,
                                 &_section);

            bufferSize -= r;
            data = (char*)data + r;

            if (!r)
                break;
        }

        //OX_ASSERT(bufferSize == 0);
        _streamEnded = true;
    }

    void OggStream::reset()
    {
        ov_pcm_seek(&_vorbisFile, 0);
        _streamEnded = false;
    }

    void OggStream::release()
    {
        if (_vorbisFile.datasource)
            ov_clear(&_vorbisFile);

        //if (_file)
        //  fclose(_file);
        _file = 0;
        //if (_oxfile)
        //  file::close(_oxfile);

        _empty = true;
        _streamEnded = false;
    }

    int OggStream::seekPCM(int pcm)
    {
        return ov_pcm_seek(&_vorbisFile, pcm);
    }

    int OggStream::seekMS(int ms)
    {
        return ov_time_seek(&_vorbisFile, ms);
    }

    int OggStream::decodeNextBlock(bool looped, void* data, int bufferSize)
    {
        unsigned int bytesUnpacked = 0;

        int p = (int)ov_pcm_tell(&_vorbisFile);

        while (1)
        {
            int r = (int)ov_read(&_vorbisFile, (char*)data, bufferSize, &_section);
            if (!r)
            {
                if (looped)
                {
                    ov_pcm_seek(&_vorbisFile, 0);

                }
                else
                {
                    _streamEnded = true;
                    break;
                }
            }

            data = (char*)data + r;
            bufferSize -= r;

            bytesUnpacked += r;
            if (bufferSize == 0)
                break;
        }

        p = int(ov_pcm_tell(&_vorbisFile)) - p;

        return bytesUnpacked;
    }
}

