#include "StaticSoundHandleOAL.h"

namespace oxygine
{
    void check();

    StaticSoundHandleOAL::StaticSoundHandleOAL(ALuint buffer): _buffer(buffer)
    {
    }

    void StaticSoundHandleOAL::_xplay()
    {
        alSourcei(_alSource, AL_BUFFER, _buffer);
        alSourcei(_alSource, AL_LOOPING, _looping ? AL_TRUE : AL_FALSE);
        check();
    }

    void StaticSoundHandleOAL::_xpause()
    {
        alSourcei(_alSource, AL_BUFFER, 0);
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
            alSourcei(_alSource, AL_BUFFER, 0);
            _ended();
        }
    }

    void StaticSoundHandleOAL::_xstop()
    {
        if (_alSource)
            alSourcei(_alSource, AL_BUFFER, 0);
    }

    void StaticSoundHandleOAL::_xupdateLoop()
    {
        alSourcei(_alSource, AL_LOOPING, _looping ? 1 : 0);
        check();
    }
}