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

    timeMS StaticSoundHandleOAL::_getPosition() const
    {
        if (_alSource)
        {
            float tm;
            alGetSourcef(_alSource, AL_SEC_OFFSET, &tm);

            return tm * 1000;
        }

        return 0;
        return _pos;
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

	void StaticSoundHandleOAL::_xsetPosition(int tm)
	{
		if (_alSource)
		{
			alSourcef(_alSource, AL_SEC_OFFSET, tm / 1000.0f);
			check();
		}
		else
		{
			ALint fr;
			alGetBufferi(_buffer, AL_FREQUENCY, &fr);

			ALint ch;
			alGetBufferi(_buffer, AL_CHANNELS, &ch);

			//tm = bytes / Channels / fr;
			//bytes = 
			_pos = (tm * ch * fr) / 1000;
			int q = 0;

		}
	}

	void StaticSoundHandleOAL::_xupdateLoop()
    {
        alSourcei(_alSource, AL_LOOPING, _looping ? 1 : 0);
        check();
    }
}