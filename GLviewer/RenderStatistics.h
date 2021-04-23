//
// Created by Jan Sobol on 26/11/2020.
//

#pragma once

#include <vector>
#include <chrono>

struct Measurement {

    using clock = std::chrono::steady_clock;

    void stop() {
        if (!finished) {
            end = clock::now();
            finished = true;
        }
    }

    unsigned long getMiliseconds() const {
        return finished
            ? std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
            : 0;
    }

    float getSeconds() const  {
        return finished
            ? std::chrono::duration_cast<std::chrono::duration<float>>(end - begin).count()
            : 0;
    }

    bool finished = false;
    clock::time_point begin = clock::now();
    clock::time_point end;
};

class RenderStatistics {
public:
    static const unsigned HISTORY_LEN = 60;

    void start();
    void end();

    void reset() { data.clear(); }
    float getLastSecods() { return lastDuration; }
    float getSampleCount() { return data.size(); }
    float getAverageTime() { return averageTime; }
    bool isMeasuring() { return !data.back().finished; }
    const std::vector<Measurement> &getData() { return data; }
    void resetAverage() { averageTime = 0.0f; averageCounting = 0;}

private:
    std::vector<Measurement> data;
    float lastDuration = 0.0f;
    float averageTime = 0.0f;
    int averageCounting = 0;

    void accountMeasurement(float seconds);
};
