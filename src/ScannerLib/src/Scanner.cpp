#include <filesystem>
#include <thread>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <map>
#include <algorithm>

#include "../include/Utils.h"
#include "../include/Scanner.h"

const std::map<std::string, int> EXTENSION_PRIORITIES = {
    {".exe", 1},
    {".dll", 1},
    {".sys", 1},
    {".bat", 1},
    {".cmd", 1},

    {".js", 2},
    {".vbs", 2},
    {".jar", 2},

    {".doc", 3},
    {".docx", 3},
    {".xls", 3},
    {".xlsx", 3},
    {".ppt", 3},
    {".pptx", 3},
    {"oth", 4}
};

struct FileTask {
    std::filesystem::path path;
    int priority{};


    bool operator<(const FileTask& other) const {
        return priority > other.priority;
    }
};

void Scanner::ScanFile(const std::filesystem::path& filePath) {
    try {
        ++totalFiles;

        const std::string hash = Utils::CalculateMD5(filePath);
        if (hash.empty()) {
            ++errors;
            LogError("Can't calc MD5 for file: " + filePath.string());
            return;
        }

        if (const std::string verdict = hashDB.CheckHash(hash); !verdict.empty()) {
            ++suspiciousFiles;
            LogSuspicious(filePath.string(), hash, verdict);
        }

        if (totalFiles % 100 == 0) {
            std::cout << "Processed " << totalFiles << " files" << std::endl;
        }

    } catch (const std::exception& e) {
        ++errors;
        LogError("Error processing file " + filePath.string() + ": " + e.what());
    }
}


void Scanner::LogSuspicious(const std::string& path, const std::string& hash, const std::string& verdict) {
    std::lock_guard<std::mutex> lock(logMutex);

    const auto now = std::chrono::system_clock::now();
    const auto time_t = std::chrono::system_clock::to_time_t(now);

    logFile << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
            << "," << path << "," << hash << "," << verdict << std::endl;
    logFile.flush();

    std::cout << "SUSPICIOUS: " << path << " [" << verdict << "]" << std::endl;
}

void Scanner::LogError(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    const auto now = std::chrono::system_clock::now();
    const auto time_t = std::chrono::system_clock::to_time_t(now);

    logFile << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
            << ",N/A,N/A,ERROR," << message << std::endl;
    logFile.flush();
}

int Scanner::GetFilePriority(const std::filesystem::path& filePath) {
    std::string extension = filePath.extension().string();

    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (const auto it = EXTENSION_PRIORITIES.find(extension); it != EXTENSION_PRIORITIES.end()) {
        return it->second;
    }

    return EXTENSION_PRIORITIES.at("oth");
}

Scanner::~Scanner() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

bool Scanner::Initialize(const std::string& csvPath, const std::string& logiPath) {
    if (!hashDB.LoadFromCSV(csvPath)) {
        return false;
    }

    this->logPath = logiPath;
    logFile.open(logPath, std::ios::out | std::ios::trunc);
    if (!logFile.is_open()) {
        std::cerr << "Error: Cannot create log file: " << logPath << std::endl;
        return false;
    }
    try {
        std::lock_guard<std::mutex> lock(logMutex);
        logFile << "Timestamp,FilePath,Hash,Verdict" << std::endl;
        logFile.flush();
        if (!logFile.good()) {
            logFile.close();
            return false;
        }
    } catch (const std::exception& e) {
        logFile.close();
        return false;
    }

    return true;
}

ScanResult Scanner::ScanDirectory(const std::string& dirPath) {
    const auto startTime = std::chrono::high_resolution_clock::now();

    totalFiles = 0;
    suspiciousFiles = 0;
    errors = 0;

    std::priority_queue<FileTask> fileQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool finished = false;

    std::thread collector([&]() {
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
                if (entry.is_regular_file()) {
                    FileTask task;
                    task.path = entry.path();
                    task.priority = GetFilePriority(entry.path());
                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        fileQueue.push(task);
                    }
                    cv.notify_one();
                }
            }
        } catch (const std::exception& e) {
            if (logFile.is_open()) {
                LogError("Directory traversal error: " + std::string(e.what()));
            } else {
                std::cerr << "Directory traversal error: " << e.what() << std::endl;
            }
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            finished = true;
        }
        cv.notify_all();
    });

    const unsigned int cores = std::thread::hardware_concurrency();
    const int workerCount = (cores > 0) ? static_cast<int>(cores) : 1;

    std::vector<std::thread> workers;
    workers.reserve(workerCount);
    for (int i = 0; i < workerCount; ++i) {
        workers.emplace_back([&]() {
            while (true) {
                FileTask task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    cv.wait(lock, [&] { return !fileQueue.empty() || finished; });

                    if (fileQueue.empty() && finished) {
                        break;
                    }
                    if (fileQueue.empty()) {
                        continue;
                    }

                    task = fileQueue.top();
                    fileQueue.pop();
                }

                ScanFile(task.path);
            }
        });
    }

    collector.join();
    for (auto& worker : workers) {
        worker.join();
    }

    const auto endTime = std::chrono::high_resolution_clock::now();
    const double duration = std::chrono::duration<double>(endTime - startTime).count();

    return {totalFiles.load(), suspiciousFiles.load(), errors.load(), duration};
}