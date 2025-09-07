#include "../include/HashDB.h"
#include "../include/Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>

bool HashDB::LoadFromCSV(const std::string& csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open CSV file: " << csvPath << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    std::unique_lock<std::shared_mutex> lock(mutex);
    hashes.clear();

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

        hashes[hash] = verdict;
    }

    std::cout << "Loaded " << hashes.size() << " malicious hashes from " << csvPath << std::endl;
    return !hashes.empty();
}

std::string HashDB::CheckHash(const std::string& hash) const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    
    std::string lowerHash = Utils::ToLower(hash);
    auto it = hashes.find(lowerHash);
    
    return (it != hashes.end()) ? it->second : "";
}

size_t HashDB::GetHashCount() const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    return hashes.size();
}