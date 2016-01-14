/*
Generic implementation of the openalext extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "openalext_internal.h"
s3eResult openalextInit()
{
    //Add any generic initialisation code here
    return openalextInit_platform();
}

void openalextTerminate()
{
    //Add any generic termination code here
    openalextTerminate_platform();
}

unsigned int _alGetError()
{
	return _alGetError_platform();
}

void _alSourceQueueBuffers(_ALuint sid, _ALsizei numEntries, const _ALuint* bids)
{
	_alSourceQueueBuffers_platform(sid, numEntries, bids);
}

void _alSourceUnqueueBuffers(_ALuint sid, _ALsizei numEntries, _ALuint* bids)
{
	_alSourceUnqueueBuffers_platform(sid, numEntries, bids);
}

void _alGetSourcei(_ALuint sid, _ALenum param, _ALint* value)
{
	_alGetSourcei_platform(sid, param, value);
}

void _alGetSourcef(_ALuint sid, _ALenum param, _ALfloat* value)
{
	_alGetSourcef_platform(sid, param, value);
}

void _alSourcef(_ALuint sid, _ALenum param, _ALfloat value)
{
	_alSourcef_platform(sid, param, value);
}

void _alSourcei(_ALuint sid, _ALenum param, _ALint value)
{
	_alSourcei_platform(sid, param, value);
}

void _alSourcePlay(_ALuint sid)
{
	_alSourcePlay_platform(sid);
}

void _alSourceStop(_ALuint sid)
{
	_alSourceStop_platform(sid);
}

void _alSourcePause(_ALuint sid)
{
	_alSourcePause_platform(sid);
}

void _alGenSources(_ALsizei n, _ALuint* sources)
{
	_alGenSources_platform(n, sources);
}

void _alDeleteSources(_ALsizei n, const _ALuint* sources)
{
	_alDeleteSources_platform(n, sources);
}

void _alGenBuffers(_ALsizei n, _ALuint* buffers)
{
	_alGenBuffers_platform(n, buffers);
}

void _alDeleteBuffers(_ALsizei n, const _ALuint* buffers)
{
	_alDeleteBuffers_platform(n, buffers);
}

void _alBufferData(_ALuint bid, _ALenum format, const _ALvoid* data, _ALsizei size, _ALsizei freq)
{
	_alBufferData_platform(bid, format, data, size, freq);
}

_ALCdevice* _alcOpenDevice(const char* devicename)
{
	return _alcOpenDevice_platform(devicename);
}

void _alcCloseDevice(_ALCdevice* device)
{
	_alcCloseDevice_platform(device);
}

_ALCcontext * _alcCreateContext(_ALCdevice* device, const _ALCint* attrlist)
{
	return _alcCreateContext_platform(device, attrlist);
}

_ALCboolean _alcMakeContextCurrent(_ALCcontext* context)
{
	return _alcMakeContextCurrent_platform(context);
}

void _alcProcessContext(_ALCcontext* context)
{
	_alcProcessContext_platform(context);
}

void _alcSuspendContext(_ALCcontext* context)
{
	_alcSuspendContext_platform(context);
}

void _alcDestroyContext(_ALCcontext* context)
{
	_alcDestroyContext_platform(context);
}

_ALCcontext * _alcGetCurrentContext()
{
	return _alcGetCurrentContext_platform();
}
