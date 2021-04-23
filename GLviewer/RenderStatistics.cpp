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
        lastFinishedMeasurement = &data.back();
        lastFinishedMeasurement->stop();
        accountMeasurement(*lastFinishedMeasurement);
    }
}

void RenderStatistics::accountMeasurement(Measurement m) {
    averageTime = averageCounting > 0
        ? averageTime + ((m.getSeconds() - averageTime) / float(averageCounting))
        : m.getSeconds();
    ++averageCounting;
}
