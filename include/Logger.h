#pragma once

#include "TestTypes.h"
#include <fstream>
#include <mutex>
#include <string>

class Logger {
public:
    explicit Logger(const std::string& csvFilePath);
    void logReport(const ModuleReport& report);

private:
    std::string filePath;
    std::mutex logMutex;
    void writeHeader();
};
