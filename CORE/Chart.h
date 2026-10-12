//
// Created by cutie on 28.08.26.
//

#ifndef TERMIA_CHART_H
#define TERMIA_CHART_H
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Hold.h"
#include "Note.h"
#include "TimingPoint.h"

namespace CORE {
    class Chart {
    private:
        std::string _path;
        std::string _file;

        std::string _fileFormat;

        // General
        std::string _audioFilename;
        int _previewTime;

        // Difficulty
        double _od;
        double _baseSliderMultiplier;

        // Metadata
        std::string _chartTitle;
        std::string _chartTitleUnicode;
        std::string _chartArtist;
        std::string _chartArtistUnicode;
        std::string _chartCreator;
        std::string _chartVersion;

        std::vector<std::shared_ptr<TimingPoint>> _SVTimingPoints;
        std::vector<std::shared_ptr<TimingPoint>> _bpmTimingPoints;
        std::vector<std::unique_ptr<GameObject>> _objects;

        void initialize();
    public:
        Chart(const std::string &path, const std::string &file): _path(path), _file(file) {
            initialize();
        };
        std::string GetName() const;
        std::string GetPath() const;
        std::string GetAudioFileName() const;
        int GetPreviewTime() const;
        double GetBaseSliderMultiplier() const;

        const std::vector<std::unique_ptr<GameObject>> &GetObjects() const;
        std::vector<GameObject*> GetVisibleObjects(double curTime) const;
        GameObject* GetNearestObject(int lane, double hitTime) const;
        Hold *GetNearestReleasableHold(int lane, double hitTime) const;

        double GetBPM(int time) const;
        double GetSVModifier(int time) const;
        double GetOD() const;
        double GetAverageBPM() const;
        TimingPoint *GetSVPoint(int time) const;
    };
}


#endif //TERMIA_CHART_H