#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

class AudioEngine
{
public:
	AudioEngine();
	virtual ~AudioEngine();

	void Init() const;

	void Input();
	void Update(float delta);

private:

};

#endif