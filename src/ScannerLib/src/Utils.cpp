#include "../include/Utils.h"
#include <openssl/md5.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <openssl/evp.h>

std::string Utils::CalculateMD5(const std::filesystem::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        return "";
    }

    if (EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr) != 1) {
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    constexpr size_t BUFFER_SIZE = 64 * 1024;
    char buffer[BUFFER_SIZE];

    while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0) {
        if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(mdctx);
            return "";
        }
    }

    unsigned char result[MD5_DIGEST_LENGTH];
    unsigned int len = 0;
    if (EVP_DigestFinal_ex(mdctx, result, &len) != 1) {
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
    }

    return ss.str();
}

std::string Utils::ToLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return std::tolower(c); });
    return result;
}

std::string Utils::Trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

double Utils::GetCurrentTimeSeconds() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}