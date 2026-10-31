#include <fstream>
#include <algorithm>
#include <utility>
#include <iostream>

#include "../include/HashDB.h"
#include "../include/Utils.h"

bool HashDB::LoadFromCSV(const std::string& csvPath) {

    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open CSV file: " << csvPath << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    decltype(hashes) loadedHashes;

    while (std::getline(file, line)) {
        lineNumber++;

        line = Utils::Trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t semicolonPos = line.find(';');
        if (semicolonPos == std::string::npos) {
            std::cerr << "Warning: Invalid format at line " << lineNumber
                      << ": " << line << std::endl;
            continue;
        }

        std::string hash = Utils::Trim(line.substr(0, semicolonPos));
        std::string verdict = Utils::Trim(line.substr(semicolonPos + 1));

        hash = Utils::ToLower(hash);

        if (hash.length() != 32 || !std::all_of(hash.begin(), hash.end(), [](char c) {
                return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
            }) || verdict.empty()) {
            std::cerr << "Warning: Invalid hash or empty verdict at line " << lineNumber
                      << ": " << hash << std::endl;
            continue;
        }

        loadedHashes[hash] = std::move(verdict);
    }

    if (file.bad() || loadedHashes.empty()) {
        return false;
    }

    hashes.swap(loadedHashes);
    return true;
}

std::string HashDB::CheckHash(const std::string& hash) const {

    std::string lowerHash = Utils::ToLower(hash);
    auto it = hashes.find(lowerHash);

    return (it != hashes.end()) ? it->second : "";
}

size_t HashDB::GetHashCount() const {
    return hashes.size();
}