#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <al.h>
#include <alc.h>

class AudioEngine
{
public:
	AudioEngine();
	virtual ~AudioEngine();

	void Input();
	void Update(float delta);
private:
	ALCdevice* device;
	ALCcontext* context;
};

#endif