#ifndef AUDIOLIBRARY_H
#define AUDIOLIBRARY_H

#include <string>
#include <map>
#include <memory>
#include "Definitions.h"

class SoundDevice;
class SoundEffect;
class BackgroundMusic;

class AudioLibrary
{
public:
	AudioLibrary(SoundDevice*);
	AudioLibrary() = delete;
	~AudioLibrary();

	bool addSoundEffect(std::string path);
	bool addBackgroundMusic(std::string path);

	SoundEffect* playSoundEffect(std::string &name);
	BackgroundMusic* playBackgroundMusic(std::string &name);

	bool hasSounds();

private:
	//Will pass string values to pass by reference
	std::map<std::string, std::unique_ptr<SoundEffect>> soundEffectLibrary;
	std::map<std::string, std::unique_ptr<BackgroundMusic>> musicLibrary;

	SoundDevice* sDevice;

};
#endif // !AUDIOLIBRARY_H