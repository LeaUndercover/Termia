//
// Created by cutie on 29.08.26.
//

#ifndef TERMIA_CHARTMANAGER_H
#define TERMIA_CHARTMANAGER_H
#include <memory>
#include <vector>

#include "Chart.h"


class ChartManager {
private:
    ChartManager() = default;
    static ChartManager* _instance;
    std::vector<std::unique_ptr<Chart>> _charts;

public:
    ChartManager(ChartManager &other) = delete;
    void operator=(const ChartManager &) = delete;
    static ChartManager *GetInstance();

    void Clear() {
        _charts.clear();
    }

    void LoadChart(const std::string &path, const std::string &file);

    std::vector<std::unique_ptr<Chart>>& GetCharts() {
        return _charts;
    }
};


#endif //TERMIA_CHARTMANAGER_H