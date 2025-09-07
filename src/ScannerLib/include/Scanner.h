#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <atomic>
#include <filesystem>

#include "ScannerAPI.h"
#include "HashDB.h"

struct SCANNER_API ScanResult {
    int totalFiles;
    int suspiciousFiles;
    int errors;
    double duration;
};

class SCANNER_API Scanner {
private:
    HashDB hashDB;
    std::ofstream logFile;
    std::string logPath;
    std::mutex logMutex;
    std::atomic<int> totalFiles{0};
    std::atomic<int> suspiciousFiles{0};
    std::atomic<int> errors{0};

    void ScanFile(const std::filesystem::path& filePath);
    void LogSuspicious(const std::string& path, const std::string& hash, const std::string& verdict);
    void LogError(const std::string& message);

public:
    Scanner() = default;
    ~Scanner();
    bool Initialize(const std::string& csvPath, const std::string& logPath);
    ScanResult ScanDirectory(const std::string& dirPath);
    size_t GetHashCount() const {
        return hashDB.GetHashCount();
    }
};