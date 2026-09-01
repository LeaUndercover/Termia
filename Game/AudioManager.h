//
// Created by cutie on 29.08.26.
//

#ifndef TERMIA_AUDIOMANAGER_H
#define TERMIA_AUDIOMANAGER_H
#include <iostream>
#include <memory>
#include <ostream>
#include "vector"
#include "../miniaudio.h"

class AudioManager {
private:
    ma_engine _engine;
    std::vector<std::unique_ptr<ma_sound>> _sounds;
    AudioManager() {
        ma_result result = ma_engine_init(nullptr, &_engine);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to initialize audio engine" << std::endl;
            exit(1);
        }
    };
    static AudioManager* _instance;

public:
    AudioManager(AudioManager &other) = delete;
    void operator=(const AudioManager &) = delete;
    static AudioManager *GetInstance();

    void PlaySound(const std::string &path, float speed, float startTime=0.0f);
    void StopSounds() const;
};


#endif //TERMIA_AUDIOMANAGER_H