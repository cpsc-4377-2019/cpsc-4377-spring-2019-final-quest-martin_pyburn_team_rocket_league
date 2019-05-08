#include "AudioLibrary.h"
#include "SoundDevice.h"
#include "Sound.h"


AudioLibrary::AudioLibrary(SoundDevice* device)
{
	sDevice = device;
}

AudioLibrary::~AudioLibrary()
{
}

bool AudioLibrary::addSoundEffect(std::string path)
{
	soundEffectLibrary[path] = std::make_unique<SoundEffect>(path, sDevice);
	return true;

}

bool AudioLibrary::addBackgroundMusic(std::string path)
{
	musicLibrary[path] = std::make_unique<BackgroundMusic>(path, sDevice);
	return true;
}

SoundEffect * AudioLibrary::playSoundEffect(std::string &name)
{
	auto sound = soundEffectLibrary.find(name);
	if (sound != soundEffectLibrary.end()) return sound->second.get();
	return nullptr;
}

BackgroundMusic * AudioLibrary::playBackgroundMusic(std::string &name)
{
	auto sound = musicLibrary.find(name);
	if (sound == musicLibrary.end())
		addBackgroundMusic(name);

	return musicLibrary[name].get();
}


bool AudioLibrary::hasSounds()
{
	return  !(soundEffectLibrary.empty() && musicLibrary.empty());

}
