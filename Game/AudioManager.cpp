//
// Created by cutie on 29.08.26.
//

#define MINIAUDIO_IMPLEMENTATION
#include "../miniaudio.h"

#include "AudioManager.h"

#include "TermiaConfig.h"

AudioManager* AudioManager::_instance = nullptr;

AudioManager *AudioManager::GetInstance()
{
    if(_instance==nullptr){
        _instance = new AudioManager();
    }
    return _instance;
}

void AudioManager::PlaySound(const std::string &path, float speed, float startTime) {
    auto sound = std::make_unique<ma_sound>();
    auto config = TermiaConfig::GetInstance();

    ma_sound_config soundConfig = ma_sound_config_init();
    soundConfig.pFilePath = path.c_str();

    ma_result result = ma_sound_init_ex(
        &_engine,
        &soundConfig,
        sound.get()
    );

    if (result != MA_SUCCESS) {
        std::cerr << "ma_sound_init_ex failed: " << result << '\n';
        return;
    }

    ma_sound_set_pitch(sound.get(), speed);
    ma_sound_seek_to_second(sound.get(), startTime/1000.0f);
    ma_sound_set_volume(sound.get(),config->GetVolume());
    ma_sound_start(sound.get());

    _sounds.push_back(std::move(sound));

    //ma_engine_play_sound(&_engine, path.c_str(), NULL);


}

void AudioManager::StopSounds() const {
    for (auto& sound: _sounds)
        ma_sound_stop(sound.get());
}
