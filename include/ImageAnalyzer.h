#pragma once

#include "TestTypes.h"
#include <opencv2/opencv.hpp>

class ImageAnalyzer {
public:
    static TestResult measureSharpness(const cv::Mat& frame, double minVariance = 100.0);
    static TestResult measureUniformity(const cv::Mat& frame, double minRatio = 0.70);
    static TestResult detectDeadPixels(const cv::Mat& darkFrame, int maxAllowedCount = 5, int thresholdVal = 30);
    static TestResult checkColorBalance(const cv::Mat& frame, double maxColorVariance = 20.0);
};
