#include "Chart.h"
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace CORE;

std::vector<std::string> split(const std::string& line, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string part;

    while (std::getline(ss, part, delimiter)) {
        result.push_back(part);
    }

    return result;
}

void Chart::initialize() {
    std::fstream file(_path+"/"+_file, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Unable to open file " << _path << std::endl;
        exit(1);
    }

    std::string line;
    int lineNum = 0;
    std::string curCategory = "";
    while (std::getline(file, line)) {
        if (lineNum==0)
            _fileFormat=line;

        if (line.starts_with("[")) {
            curCategory = line.substr(1, line.length()-3);
            continue;
        }
        // TODO: Fix Metadata

        if (curCategory=="General") {
            if (line.starts_with("AudioFilename: "))
                _audioFilename = line.substr(std::string("AudioFilename: ").length(), line.length()-std::string("AudioFilename: ").length()-1);

            if (line.starts_with("PreviewTime: "))
                _previewTime = std::stoi(line.substr(std::string("PreviewTime: ").length(), line.length()-std::string("PreviewTime: ").length()-1));
        }

        if (curCategory=="Metadata") {
            if (line.starts_with("Title:"))
                _chartTitle = line.substr(std::string("Title:").length(), line.length()-std::string("Title:").length()-1);

            if (line.starts_with("TitleUnicode:"))
                _chartTitleUnicode = line.substr(std::string("TitleUnicode:").length(), line.length()-std::string("TitleUnicode:").length()-1);

            if (line.starts_with("Artist:"))
                _chartArtist = line.substr(std::string("Artist:").length(), line.length()-std::string("Artist:").length()-1);

            if (line.starts_with("ArtistUnicode:"))
                _chartArtistUnicode = line.substr(std::string("ArtistUnicode:").length(), line.length()-std::string("ArtistUnicode:").length()-1);

            if (line.starts_with("Creator:"))
                _chartCreator = line.substr(std::string("Creator:").length(), line.length()-std::string("Creator:").length()-1);

            if (line.starts_with("Version:"))
                _chartVersion = line.substr(std::string("Version:").length(), line.length()-std::string("Version:").length()-1);
        }

        if (curCategory=="Difficulty") {
            if (line.starts_with("OverallDifficulty:")) {
                auto parsed = line.substr(std::string("OverallDifficulty:").length(), line.length()-std::string("OverallDifficulty:").length()-1);
                _od = std::stod(parsed);
            }

            if (line.starts_with("SliderMultiplier:")) {
                auto parsed = line.substr(std::string("SliderMultiplier:").length(), line.length()-std::string("SliderMultiplier:").length()-1);
                _baseSliderMultiplier = std::stod(parsed);
            }

        }

        if (curCategory=="TimingPoints") {
            auto args = split(line, ',');
            if (args.size()!=8)
                continue;

            int time = std::stoi(args[0]);
            double beatLength = std::stod(args[1]);
            int beatCount = std::stoi(args[2]);
            int sampleSet = std::stoi(args[3]);
            int sampleIndex = std::stoi(args[4]);
            int volume = std::stoi(args[5]);
            bool uninherited = std::stoi(args[6]);
            int effects = std::stoi(args[7]);

            _timingPoints.emplace_back(std::make_unique<TimingPoint>(
                    time,
                    beatLength,
                    beatCount,
                    sampleSet,
                    sampleIndex,
                    volume,
                    uninherited,
                    effects));
        }

        if (curCategory=="HitObjects") {
            auto args = split(line, ',');

            int posX = std::stoi(args[0]);
            int posY = std::stoi(args[1]);
            int time = std::stoi(args[2]);
            char type = static_cast<char>(std::stoi(args[3]));
            int hitSound = std::stoi(args[4]);

            //TODO: add holds

            int columnCount = 4; // TODO: Fix Hardcoding
            int lane = (posX-64)/(512/columnCount);

            // Note
            if ((type&0b00000001)>0)
                _objects.emplace_back(std::make_unique<Note>(lane, time, hitSound, type));

            // Hold
            if ((type&0b10000000)>0) {
                auto objectParameterList = split(args[5], ':');
                auto endTime = std::stoi(objectParameterList[0]);
                auto length = endTime-time;

                _objects.emplace_back(std::make_unique<Hold>(lane, time, hitSound, type,length));

                std::cout << "added Note: " << _objects.back().get() << '\n';
            }
        }

        lineNum++;
    }

    // Sort Timing points by time
    std::sort(
        _timingPoints.begin(),
        _timingPoints.end(),
        [](const auto& a, const auto& b) {
            return a->GetTime() < b->GetTime();
        }
    );
}

std::string Chart::GetName() const {
    return "[" + _chartVersion + "]" + _chartTitle + " - " + _chartArtist;
}

std::string Chart::GetPath() const {
    return _path;
}

std::string Chart::GetAudioFileName() const {
    return _audioFilename;
}

int Chart::GetPreviewTime() const {
    return _previewTime;
}

double Chart::GetBaseSliderMultiplier() const {
    return _baseSliderMultiplier;
}

const std::vector<std::unique_ptr<GameObject>>& Chart::GetObjects() const
{
    return _objects;
}

std::vector<GameObject*> Chart::GetVisibleObjects(double curTime, double scrollSpeedMS) const {
    auto visible = std::vector<GameObject*>();

    for (const auto& object:_objects) {
        if (object->IsHit())
            continue;

        if (object->IsNote()) {
            auto* note = static_cast<Note*>(object.get());
            const auto deltaTime = note->GetTime()-curTime;

            if (deltaTime >= 0 && deltaTime <= scrollSpeedMS)
                visible.push_back(object.get());
        }
        else
            if (object->IsHold()) {
                auto* hold = static_cast<Hold*>(object.get());
                const auto deltaTimeStart = hold->GetTime()-curTime;
                const auto deltaTimeEnd = hold->GetTime()+hold->GetLength()-curTime;

                if (    ((deltaTimeStart <= scrollSpeedMS) && (deltaTimeEnd >= 0)) ||
                        (curTime <= deltaTimeStart && curTime >= deltaTimeEnd))
                    visible.push_back(object.get());
            }
    }

    return visible;
}

GameObject* Chart::GetNearestObject(int lane, double hitTime) const {
    int minDelta = 999999;
    GameObject* value = nullptr;

    for (const auto& object:_objects) {
        if (object->GetLane()!=lane)
            continue;

        if (object->IsHit())
            continue;

        int delta = abs(object->GetTime()-hitTime);
        if (delta<minDelta) {
            minDelta=delta;
            value=object.get();
        }
    }

    return value;
}

Hold* Chart::GetNearestReleasableHold(int lane, double hitTime) const {
    int minDelta = 999999;
    Hold* value = nullptr;

    for (const auto& object:_objects) {
        if (object->GetLane()!=lane)
            continue;

        if (!object->IsHold())
            continue;

        auto* hold = static_cast<Hold*>(object.get());

        int delta = abs(hold->GetTime()+hold->GetLength()-hitTime);
        if (delta<minDelta) {
            minDelta=delta;
            value=hold;
        }
    }

    return value;
}


double Chart::GetSVModifier(int time) const {
    auto it = std::upper_bound(
        _timingPoints.begin(),
        _timingPoints.end(),
        time,
        [](int t, const auto& point) {
            return t < point->GetTime();
        }
    );

    // Get the First Inherited point
    while (it != _timingPoints.begin()) {
        --it;

        if ((*it)->GetUninherited())
            continue;

        return (*it)->GetSVModifier();
    }

    return 1.0;
}

double Chart::GetScrollTime(const double songTime) const {
    double scrollTime = 0.0;
    double previousTime = 0.0;
    double sv = 1.0;

    for (const auto& point : _timingPoints) {
        double pointTime = point->GetTime();

        if (pointTime >= songTime)
            break;

        scrollTime += (pointTime - previousTime) * sv;
        previousTime = pointTime;

        if (point->GetUninherited())
            sv = 1.0;
        else
            sv = point->GetSVModifier();
    }

    scrollTime += (songTime - previousTime) * sv;

    return scrollTime;
}

double Chart::GetOD() const {
    return _od;
}
