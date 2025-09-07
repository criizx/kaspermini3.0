#pragma once
#include "ScannerAPI.h"
#include "HashDB.h"
#include <string>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <mutex>

struct SCANNER_API ScanResult {
    int totalFiles = 0;
    int maliciousFiles = 0;
    int errors = 0;
    double timeSeconds = 0.0;
};

class SCANNER_API Scanner {
private:
    HashDB hashDB;
    std::string logPath;
    std::ofstream logFile;
    std::mutex logMutex;
    
    std::atomic<int> totalFiles{0};
    std::atomic<int> maliciousFiles{0};  
    std::atomic<int> errors{0};

public:
    bool Initialize(const std::string& csvPath, const std::string& logPath);
    ScanResult ScanDirectory(const std::string& dirPath);
    ~Scanner();
    size_t GetHashCount() const {
        return hashDB.GetHashCount();
    }
    
private:
    void ScanFile(const std::filesystem::path& filePath);
    void LogMalicious(const std::string& path, const std::string& hash, const std::string& verdict);
    void LogError(const std::string& message);
};