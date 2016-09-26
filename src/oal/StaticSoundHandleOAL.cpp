#include "StaticSoundHandleOAL.h"

namespace oxygine
{
    void check();

    StaticSoundHandleOAL::StaticSoundHandleOAL(ALuint buffer): _buffer(buffer)
    {

    }

    void StaticSoundHandleOAL::_xplay()
    {
        alSourceQueueBuffers(_alSource, 1, &_buffer);
        alSourcei(_alSource, AL_LOOPING, _looping ? AL_TRUE : AL_FALSE);
        check();
    }

    void StaticSoundHandleOAL::_xpause()
    {
        ALuint b;
        alSourceUnqueueBuffers(_alSource, 1, &b);
    }


    void StaticSoundHandleOAL::_xresume()
    {
        _xplay();
    }

    void StaticSoundHandleOAL::_xupdate()
    {
        ALint state = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
        check();

        if (state == AL_STOPPED)
        {
            _ended();
        }
    }

    void StaticSoundHandleOAL::_xupdateLoop()
    {
        alSourcei(_alSource, AL_LOOPING, _looping ? 1 : 0);
        check();
    }
}