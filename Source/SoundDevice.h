#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include "Definitions.h"
#include "SDL_mixer.h"

class ResourceManager;
class AudioLibrary;
class SoundEffect;
class BackgroundMusic;

class SoundDevice {
public:
	~SoundDevice();
	SoundDevice();

	//passing string value by reference
	bool playSound(std::string &sound, int numLoops = 1, int channel = -1);
	bool playSound(SoundEffect* sound, int numLoops = 1, int channel = -1);

	void setAsBackground(std::string &background);
	void setAsBackground(BackgroundMusic* background);
	int setMusicVolume(int volume);
	int setSoundVolume(int volume);

	void stopChannel(int channel);

	bool initialized{ false };

	string musicPath;
	string soundPath;

private:

	std::unique_ptr<AudioLibrary> aLibrary = make_unique<AudioLibrary>(this);

};


#endif