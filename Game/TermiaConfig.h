//
// Created by cutie on 29.08.26.
//

#ifndef TERMIA_TERMIACONFIG_H
#define TERMIA_TERMIACONFIG_H


class TermiaConfig {
private:
    float _speed = 1.0f;
    float _scrollSpeed = 27.0f;
    float _volume = 0.5f;

    TermiaConfig() = default;
    static TermiaConfig* _instance;
public:
    TermiaConfig(TermiaConfig &other) = delete;
    void operator=(const TermiaConfig &) = delete;

    void SetSpeed(float speed);
    void SetVolume(float volume);
    void SetScrollSpeed(float scrollSpeed);

    static TermiaConfig *GetInstance();

    float GetSpeed() const;
    float GetScrollSpeed() const;
    float GetScrollSpeedMS() const;
    float GetVolume() const;
};


#endif //TERMIA_TERMIACONFIG_H