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
        std::vector<char> _keys = {'w','e','o','p'};
        char _keyQuit = 'b';
        char _consoleKey = ':';
        std::string _searchPath = "/mnt/stuff/termia";

        TermiaConfig() = default;
        static TermiaConfig* _instance;
    public:
        TermiaConfig(TermiaConfig &other) = delete;
        void operator=(const TermiaConfig &) = delete;

        void SetSpeed(float speed);
        void SetVolume(float volume);
        void SetScrollSpeed(float scrollSpeed);
        void SetKeys(std::vector<char> keys);
        void SetQuitKey(char key);
        void SetSearchPath(std::string path);
        void SetConsoleKey(char key);

        static TermiaConfig *GetInstance();

        float GetSpeed() const;
        float GetScrollSpeed() const;
        float GetScrollSpeedMS() const;
        float GetVolume() const;
        std::vector<char> GetKeys() const;
        char GetQuitKey() const;
        std::string GetSearchPath() const;
        char GetConsoleKey() const;
    };
}

#endif //TERMIA_TERMIACONFIG_H