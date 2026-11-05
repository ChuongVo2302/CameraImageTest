#pragma once

#include <string>

struct TestResult {
    std::string testName;
    double measuredValue;
    double thresholdMin;
    double thresholdMax;
    bool passed;
};

struct ModuleReport {
    std::string serialNumber;
    std::string timestamp;
    bool overallPass;
    std::string failureReason;
    double sharpness;
    double uniformity;
    int deadPixels;
    double colorVariance;
};
