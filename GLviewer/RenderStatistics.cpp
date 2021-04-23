//
// Created by Petr Fusek
//

#include "RenderStatistics.h"

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
