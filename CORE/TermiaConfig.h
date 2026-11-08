//
// Created by cutie on 29.08.26.
//

#ifndef TERMIA_TERMIACONFIG_H
#define TERMIA_TERMIACONFIG_H
#include <string>
#include <vector>
#include <linux/input-event-codes.h>

namespace CORE {
    enum RendererType {
        Legacy,
        Nik2D,
        Nik3D
    };

    class TermiaConfig {
    private:
        float _speed = 1.0f;
        float _scrollSpeed = 27.0f;
        float _volume = 0.005f;
        std::vector<char> _keys = {'w','e','o','p'};
        char _keyQuit = 'b';
        char _consoleKey = ':';
        std::string _searchPath = "/mnt/stuff/termia";
        RendererType _rendererType = Nik3D;

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
        void SetRendererType(RendererType type);

        static TermiaConfig *GetInstance();

        float GetSpeed() const;
        double GetScrollSpeed() const;
        double GetScrollSpeedMS() const;
        double GetScrollSpeedMS(double scrollSpeed) const;
        float GetVolume() const;
        std::vector<char> GetKeys() const;
        char GetQuitKey() const;
        std::string GetSearchPath() const;
        char GetConsoleKey() const;
        RendererType GetRendererType() const;
    };
}

#endif //TERMIA_TERMIACONFIG_H