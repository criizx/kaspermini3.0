#pragma once

#include <string>
#include <filesystem>

#include "ScannerAPI.h"

class SCANNER_API Utils {
public:
    static std::string CalculateMD5(const std::filesystem::path& filePath);
    static std::string ToLower(const std::string& str);
    static std::string Trim(const std::string& str);
    static double GetCurrentTimeSeconds();
};