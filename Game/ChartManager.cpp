//
// Created by cutie on 29.08.26.
//

#include "ChartManager.h"

ChartManager* ChartManager::_instance = nullptr;

ChartManager *ChartManager::GetInstance()
{
    if(_instance==nullptr){
        _instance = new ChartManager();
    }
    return _instance;
}

void ChartManager::LoadChart(const std::string& path, const std::string& file) {
    _charts.push_back(std::make_unique<Chart>(path, file));
}
