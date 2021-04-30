//
// Created by Petr Fusek
//

#include "RenderStatistics.h"
#include <fstream>

void RenderStatistics::start() {
    end();
    data.emplace_back(Measurement());
}

void RenderStatistics::end() {
    if (!data.empty()) {
        data.back().stop();
        lastDuration = data.back().getSeconds();
        accountMeasurement(lastDuration);
    }
}

void RenderStatistics::accountMeasurement(float seconds) {
    averageTime = averageCounting > 0
        ? averageTime + ((seconds - averageTime) / float(averageCounting))
        : seconds;
    ++averageCounting;
}

void RenderStatistics::saveToFile(const std::string &filename) const {
    std::ofstream file;
    file.open(filename);

    file << getAverageTime() << std::endl;
    file << getSampleCount() << std::endl;

    /*for (const auto &item : data) {
        file << item.getSeconds() << std::endl;
    }*/

    file.close();
}
