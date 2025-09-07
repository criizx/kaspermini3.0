#include <fstream>
#include <sstream>
#include <iostream>

#include "../include/HashDB.h"
#include "../include/Utils.h"

HashDB::~HashDB() {
    try {
        hashes.clear();
    } catch (const std::exception& e) {
    }
}

bool HashDB::LoadFromCSV(const std::string& csvPath) {

    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open CSV file: " << csvPath << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    try {
        hashes.clear();
    } catch (const std::exception& e) {
        file.close();
        return false;
    }

    while (std::getline(file, line)) {
        lineNumber++;

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

        if (hash.length() != 32) {
            std::cerr << "Warning: Invalid MD5 hash length at line " << lineNumber
                      << ": " << hash << std::endl;
            continue;
        }

        try {
            hashes[hash] = verdict;
        } catch (const std::exception& e) {
            file.close();
            return false;
        }
    }

    file.close();
    return !hashes.empty();
}

std::string HashDB::CheckHash(const std::string& hash) const {

    std::string lowerHash = Utils::ToLower(hash);
    auto it = hashes.find(lowerHash);

    return (it != hashes.end()) ? it->second : "";
}

size_t HashDB::GetHashCount() const {
    size_t count = hashes.size();
    return count;
}