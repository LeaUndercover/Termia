//
// Created by cutie on 29.08.26.
//

#ifndef TERMIA_TERMIACONFIG_H
#define TERMIA_TERMIACONFIG_H
#include <string>
#include <vector>
#include <linux/input-event-codes.h>

namespace CORE {
    class TermiaConfig {
    private:
        float _speed = 1.0f;
        float _scrollSpeed = 27.0f;
        float _volume = 0.5f;
        std::vector<int> _keys = {KEY_W,KEY_E,KEY_O,KEY_P};
        int _keyQuit = KEY_B;
        std::string _searchPath = "/mnt/stuff/termia";

        TermiaConfig() = default;
        static TermiaConfig* _instance;
    public:
        TermiaConfig(TermiaConfig &other) = delete;
        void operator=(const TermiaConfig &) = delete;

        void SetSpeed(float speed);
        void SetVolume(float volume);
        void SetScrollSpeed(float scrollSpeed);
        void SetKeys(std::vector<int> keys);
        void SetQuitKey(int key);
        void SetSearchPath(std::string path);

        static TermiaConfig *GetInstance();

        float GetSpeed() const;
        float GetScrollSpeed() const;
        float GetScrollSpeedMS() const;
        float GetVolume() const;
        std::vector<int> GetKeys() const;
        int GetQuitKey() const;
        std::string GetSearchPath() const;
    };
}

#endif //TERMIA_TERMIACONFIG_H