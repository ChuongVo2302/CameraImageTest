#include "ImageAnalyzer.h"
#include <cmath>

TestResult ImageAnalyzer::measureSharpness(const cv::Mat& frame, double minVariance) {
    cv::Mat gray, laplacian;
    if (frame.channels() == 3) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = frame;
    }

    cv::Laplacian(gray, laplacian, CV_64F);
    cv::Scalar mean, stddev;
    cv::meanStdDev(laplacian, mean, stddev);

    double variance = stddev.val[0] * stddev.val[0];
    return {"Sharpness (Laplacian Var)", variance, minVariance, -1.0, variance >= minVariance};
}

TestResult ImageAnalyzer::measureUniformity(const cv::Mat& frame, double minRatio) {
    cv::Mat gray;
    if (frame.channels() == 3) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = frame;
    }

    int w = gray.cols;
    int h = gray.rows;
    int roiW = w / 8;
    int roiH = h / 8;

    cv::Scalar centerMean = cv::mean(gray(cv::Rect(w / 2 - roiW / 2, h / 2 - roiH / 2, roiW, roiH)));
    cv::Scalar topLeftMean = cv::mean(gray(cv::Rect(0, 0, roiW, roiH)));
    cv::Scalar topRightMean = cv::mean(gray(cv::Rect(w - roiW, 0, roiW, roiH)));
    cv::Scalar botLeftMean = cv::mean(gray(cv::Rect(0, h - roiH, roiW, roiH)));
    cv::Scalar botRightMean = cv::mean(gray(cv::Rect(w - roiW, h - roiH, roiW, roiH)));

    double cornerAvg = (topLeftMean[0] + topRightMean[0] + botLeftMean[0] + botRightMean[0]) / 4.0;
    double ratio = (centerMean[0] > 0) ? (cornerAvg / centerMean[0]) : 0.0;

    return {"Luminance Uniformity", ratio, minRatio, 1.0, ratio >= minRatio};
}

TestResult ImageAnalyzer::detectDeadPixels(const cv::Mat& darkFrame, int maxAllowedCount, int thresholdVal) {
    cv::Mat gray, binary;
    if (darkFrame.channels() == 3) {
        cv::cvtColor(darkFrame, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = darkFrame;
    }

    cv::threshold(gray, binary, thresholdVal, 255, cv::THRESH_BINARY);
    int deadPixelCount = cv::countNonZero(binary);

    return {"Dead/Hot Pixels", static_cast<double>(deadPixelCount), 0.0, static_cast<double>(maxAllowedCount), deadPixelCount <= maxAllowedCount};
}

TestResult ImageAnalyzer::checkColorBalance(const cv::Mat& frame, double maxColorVariance) {
    cv::Scalar meanBGR = cv::mean(frame);
    double avg = (meanBGR[0] + meanBGR[1] + meanBGR[2]) / 3.0;
    double var = (std::pow(meanBGR[0] - avg, 2) + std::pow(meanBGR[1] - avg, 2) + std::pow(meanBGR[2] - avg, 2)) / 3.0;

    return {"Color Balance Variance", var, 0.0, maxColorVariance, var <= maxColorVariance};
}
