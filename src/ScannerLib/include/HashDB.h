#pragma once

#include <string>
#include <unordered_map>

#include "ScannerAPI.h"

class SCANNER_API HashDB {
    std::unordered_map<std::string, std::string> hashes;

public:
    HashDB() = default;
    ~HashDB();
    bool LoadFromCSV(const std::string& csvPath);
    std::string CheckHash(const std::string& hash) const;
    size_t GetHashCount() const;
};