/*
 * Internal header for the openalext extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef OPENALEXT_INTERNAL_H
#define OPENALEXT_INTERNAL_H

#include "s3eTypes.h"
#include "openalext.h"
#include "openalext_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult openalextInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult openalextInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void openalextTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void openalextTerminate_platform();
unsigned int _alGetError_platform();

void _alSourceQueueBuffers_platform(_ALuint sid, _ALsizei numEntries, const _ALuint* bids);

void _alSourceUnqueueBuffers_platform(_ALuint sid, _ALsizei numEntries, _ALuint* bids);

void _alGetSourcei_platform(_ALuint sid, _ALenum param, _ALint* value);

void _alGetSourcef_platform(_ALuint sid, _ALenum param, _ALfloat* value);

void _alSourcef_platform(_ALuint sid, _ALenum param, _ALfloat value);

void _alSourcei_platform(_ALuint sid, _ALenum param, _ALint value);

void _alSourcePlay_platform(_ALuint sid);

void _alSourceStop_platform(_ALuint sid);

void _alSourcePause_platform(_ALuint sid);

void _alGenSources_platform(_ALsizei n, _ALuint* sources);

void _alDeleteSources_platform(_ALsizei n, const _ALuint* sources);

void _alGenBuffers_platform(_ALsizei n, _ALuint* buffers);

void _alDeleteBuffers_platform(_ALsizei n, const _ALuint* buffers);

void _alBufferData_platform(_ALuint bid, _ALenum format, const _ALvoid* data, _ALsizei size, _ALsizei freq);

_ALCdevice* _alcOpenDevice_platform(const char* devicename);

void _alcCloseDevice_platform(_ALCdevice* device);

_ALCcontext * _alcCreateContext_platform(_ALCdevice* device, const _ALCint* attrlist);

_ALCboolean _alcMakeContextCurrent_platform(_ALCcontext* context);

void _alcProcessContext_platform(_ALCcontext* context);

void _alcSuspendContext_platform(_ALCcontext* context);

void _alcDestroyContext_platform(_ALCcontext* context);

_ALCcontext * _alcGetCurrentContext_platform();


#endif /* !OPENALEXT_INTERNAL_H */