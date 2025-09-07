#pragma once
#include "ScannerAPI.h"
#include <string>
#include <unordered_map>
#include <shared_mutex>

class SCANNER_API HashDB {
private:
    std::unordered_map<std::string, std::string> hashes;
    mutable std::shared_mutex mutex;

public:
    bool LoadFromCSV(const std::string& csvPath);
    std::string CheckHash(const std::string& hash) const;
    size_t GetHashCount() const;
};