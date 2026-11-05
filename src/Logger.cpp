#include "Logger.h"
#include <chrono>
#include <iomanip>

Logger::Logger(const std::string& csvFilePath) : filePath(csvFilePath) {
    std::ifstream infile(filePath);
    if (!infile.good()) {
        writeHeader();
    }
}

void Logger::writeHeader() {
    std::ofstream outfile(filePath, std::ios::app);
    outfile << "Timestamp,SerialNumber,OverallStatus,FailureReason,Sharpness,Uniformity,DeadPixels,ColorVariance\n";
}

void Logger::logReport(const ModuleReport& report) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::ofstream outfile(filePath, std::ios::app);

    outfile << report.timestamp << ","
            << report.serialNumber << ","
            << (report.overallPass ? "PASS" : "FAIL") << ","
            << "\"" << report.failureReason << "\","
            << report.sharpness << ","
            << report.uniformity << ","
            << report.deadPixels << ","
            << report.colorVariance << "\n";
}
