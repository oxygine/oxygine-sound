/*
 * windows-specific implementation of the openalext extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "openalext_internal.h"
#include "AL/al.h"
#include "AL/alc.h"


s3eResult openalextInit_platform()
{
    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void openalextTerminate_platform()
{
    // Add any platform-specific termination code here
}

unsigned int _alGetError_platform()
{
    return alGetError();
}

void _alSourceQueueBuffers_platform(_ALuint sid, _ALsizei numEntries, const _ALuint* bids)
{
	alSourceQueueBuffers(sid, numEntries, bids);
}

void _alSourceUnqueueBuffers_platform(_ALuint sid, _ALsizei numEntries, _ALuint* bids)
{
	alSourceUnqueueBuffers(sid, numEntries, bids);
}

void _alGetSourcei_platform(_ALuint sid, _ALenum param, _ALint* value)
{
	alGetSourcei(sid, param, value);
}

void _alGetSourcef_platform(_ALuint sid, _ALenum param, _ALfloat* value)
{
	alGetSourcef(sid, param, value);
}

void _alSourcef_platform(_ALuint sid, _ALenum param, _ALfloat value)
{
	alSourcef(sid, param, value);
}

void _alSourcei_platform(_ALuint sid, _ALenum param, _ALint value)
{
	alSourcei(sid, param, value);
}

void _alSourcePlay_platform(_ALuint sid)
{
	alSourcePlay(sid);
}

void _alSourceStop_platform(_ALuint sid)
{
	alSourceStop(sid);
}

void _alSourcePause_platform(_ALuint sid)
{
	alSourcePause(sid);
}

void _alGenSources_platform(_ALsizei n, _ALuint* sources)
{
	alGenSources(n, sources);
}

void _alDeleteSources_platform(_ALsizei n, const _ALuint* sources)
{
	alDeleteSources(n, sources);
}

void _alGenBuffers_platform(_ALsizei n, _ALuint* buffers)
{
	alGenBuffers(n, buffers);
}

void _alDeleteBuffers_platform(_ALsizei n, const _ALuint* buffers)
{
	alDeleteBuffers(n, buffers);
}

void _alBufferData_platform(_ALuint bid, _ALenum format, const _ALvoid* data, _ALsizei size, _ALsizei freq)
{
	alBufferData(bid, format, data, size, freq);
}

_ALCdevice* _alcOpenDevice_platform(const char* devicename)
{
    return (_ALCdevice*)alcOpenDevice(devicename);
}

void _alcCloseDevice_platform(_ALCdevice* device)
{
	alcCloseDevice((ALCdevice*)device);
}

_ALCcontext * _alcCreateContext_platform(_ALCdevice* device, const _ALCint* attrlist)
{
    return (_ALCcontext *)alcCreateContext((ALCdevice*)device, attrlist);
}

_ALCboolean _alcMakeContextCurrent_platform(_ALCcontext* context)
{
    return alcMakeContextCurrent((ALCcontext*)context);
}

void _alcProcessContext_platform(_ALCcontext* context)
{
	alcProcessContext((ALCcontext*)context);
}

void _alcSuspendContext_platform(_ALCcontext* context)
{
	alcSuspendContext((ALCcontext*)context);
}

void _alcDestroyContext_platform(_ALCcontext* context)
{
	alcDestroyContext((ALCcontext*)context);
}

_ALCcontext * _alcGetCurrentContext_platform()
{
    return (_ALCcontext*)alcGetCurrentContext();
}
