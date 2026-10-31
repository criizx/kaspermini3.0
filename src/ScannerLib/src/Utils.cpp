#include <array>
#include <memory>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <openssl/evp.h>

#include "../include/Utils.h"

std::string Utils::CalculateMD5(const std::filesystem::path& filePath) {
    std::error_code error;
    if (!std::filesystem::is_regular_file(filePath, error)) {
        return "";
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }

    const std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> mdctx(
        EVP_MD_CTX_new(), EVP_MD_CTX_free);
    if (mdctx == nullptr) {
        return "";
    }

    if (EVP_DigestInit_ex(mdctx.get(), EVP_md5(), nullptr) != 1) {
        return "";
    }

    constexpr size_t BUFFER_SIZE = 64 * 1024;
    std::array<char, BUFFER_SIZE> buffer{};

    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        if (EVP_DigestUpdate(mdctx.get(), buffer.data(), static_cast<size_t>(file.gcount())) != 1) {
            return "";
        }
    }

    if (file.bad() || (file.fail() && !file.eof())) {
        return "";
    }

    std::array<unsigned char, EVP_MAX_MD_SIZE> result{};
    unsigned int len = 0;
    if (EVP_DigestFinal_ex(mdctx.get(), result.data(), &len) != 1) {
        return "";
    }

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
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

double Utils::GetCurrentTimeSeconds() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}