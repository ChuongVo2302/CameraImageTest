#include "ImageAnalyzer.h"
#include "Logger.h"
#include <opencv2/core/utils/logger.hpp>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int main(int argc, char** argv) {
    // Suppress verbose OpenCV internal dynamic library warnings
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

    // Default to "data/" folder if no directory path argument is provided
    std::string folderPath = (argc > 1) ? argv[1] : "data/";
    std::string logFilePath = "data/test_log.csv";

    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        std::cerr << "[ERROR] Target directory '" << folderPath << "' does not exist or is not a directory." << std::endl;
        return -1;
    }

    Logger logger(logFilePath);
    int totalProcessed = 0;
    int passCount = 0;
    int failCount = 0;

    std::cout << "==================================================" << std::endl;
    std::cout << " Starting Batch Camera Module Inspection" << std::endl;
    std::cout << " Target Directory: " << folderPath << std::endl;
    std::cout << " Log Destination:  " << logFilePath << std::endl;
    std::cout << "==================================================\n" << std::endl;

    // Iterate through all files in the target directory
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) continue;

        std::string ext = entry.path().extension().string();
        // Check for JPEG/PNG image extensions
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".JPG" || ext == ".PNG") {
            std::string imagePath = entry.path().string();
            std::string serialNumber = entry.path().stem().string(); // Uses file name (e.g. SN-2026-0001)

            cv::Mat frame = cv::imread(imagePath);
            if (frame.empty()) {
                std::cerr << "[SKIP] Failed to open image: " << imagePath << std::endl;
                continue;
            }

            // Run Analysis Suite
            TestResult rSharpness  = ImageAnalyzer::measureSharpness(frame, 100.0);
            TestResult rUniformity = ImageAnalyzer::measureUniformity(frame, 0.70);
            TestResult rDeadPixels = ImageAnalyzer::detectDeadPixels(frame, 5, 250); // Threshold bright pixels
            TestResult rColor      = ImageAnalyzer::checkColorBalance(frame, 20.0);

            bool overallPass = rSharpness.passed && rUniformity.passed && rDeadPixels.passed && rColor.passed;

            std::string failureReasons = "";
            if (!rSharpness.passed)  failureReasons += "Low Sharpness; ";
            if (!rUniformity.passed) failureReasons += "Poor Uniformity; ";
            if (!rDeadPixels.passed) failureReasons += "Excess Dead Pixels; ";
            if (!rColor.passed)      failureReasons += "Color Imbalance; ";

            ModuleReport report{
                serialNumber,
                getCurrentTimestamp(),
                overallPass,
                failureReasons,
                rSharpness.measuredValue,
                rUniformity.measuredValue,
                static_cast<int>(rDeadPixels.measuredValue),
                rColor.measuredValue
            };

            // Thread-safe CSV logging
            logger.logReport(report);

            totalProcessed++;
            if (overallPass) passCount++; else failCount++;

            // Console output per file
            std::cout << "[" << (overallPass ? "PASS" : "FAIL") << "] Module: " << std::setw(15) << std::left << serialNumber
                      << " | Sharpness: " << std::setw(7) << std::fixed << std::setprecision(2) << rSharpness.measuredValue
                      << " | Uniformity: " << std::setw(4) << rUniformity.measuredValue
                      << " | DeadPixels: " << std::setw(3) << static_cast<int>(rDeadPixels.measuredValue)
                      << " | Reason: " << (failureReasons.empty() ? "None" : failureReasons)
                      << std::endl;
        }
    }

    double yieldRate = (totalProcessed > 0) ? (static_cast<double>(passCount) / totalProcessed) * 100.0 : 0.0;

    std::cout << "\n==================================================" << std::endl;
    std::cout << " Batch Summary:" << std::endl;
    std::cout << "  - Total Tested: " << totalProcessed << std::endl;
    std::cout << "  - Passed:       " << passCount << std::endl;
    std::cout << "  - Failed:       " << failCount << std::endl;
    std::cout << "  - First-Pass Yield: " << std::fixed << std::setprecision(2) << yieldRate << "%" << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}