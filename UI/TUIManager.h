//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_TUIMANAGER_H
#define TERMIA_TUIMANAGER_H
#include <cstring>
#include <memory>

#include "TUIMenu.h"
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <iostream>

class TUIManager {
private:
    libevdev* _dev;
    int _devFd;
    TUIManager(): _menu(nullptr) {
        _devFd = open("/dev/input/event5", O_RDONLY | O_NONBLOCK); // TODO: Remove Hardcode

        if (_devFd < 0) {
            perror("open");
            exit(1);
        }

        _dev = nullptr;

        int rc = libevdev_new_from_fd(_devFd, &_dev);

        if (rc < 0) {
            std::cerr << "libevdev_new_from_fd failed: "
                      << std::strerror(-rc) << '\n';
            close(_devFd);
            exit(1);
        }

        std::cout << "Device: "
                  << libevdev_get_name(_dev)
                  << '\n';
    };

    ~TUIManager() {
        libevdev_free(_dev);
        close(_devFd);
    }

    static TUIManager* _instance;

    std::unique_ptr<TUIMenu> _menu;
    std::pair<unsigned int, unsigned int> _resolution;

public:
    TUIManager(TUIManager &other) = delete;
    void operator=(const TUIManager &) = delete;
    static TUIManager *GetInstance();

    void DoEvents() const;
    void ChangeMenu(std::unique_ptr<TUIMenu> menu);
    void SetResolution(const std::pair<unsigned int, unsigned int> &res);
    std::pair<unsigned int, unsigned int> GetResolution() const;
};

#endif //TERMIA_TUIMANAGER_H