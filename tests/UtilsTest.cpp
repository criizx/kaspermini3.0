#include <gtest/gtest.h>
#include "../include/Utils.h"
#include <fstream>
#include <filesystem>

class UtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream file("test_file.txt");
        file << "Hello, World!";
        file.close();
    }

    void TearDown() override {
        std::filesystem::remove("test_file.txt");
    }
};

TEST_F(UtilsTest, CalculateMD5_ValidFile) {
    std::string md5 = Utils::CalculateMD5("test_file.txt");
    EXPECT_FALSE(md5.empty());
    EXPECT_EQ(md5.length(), 32);
    EXPECT_EQ(md5, "65a8e27d8879283831b664bd8b7f0ad4");
}

TEST_F(UtilsTest, CalculateMD5_NonExistentFile) {
    std::string md5 = Utils::CalculateMD5("nonexistent.txt");
    EXPECT_TRUE(md5.empty());
}

TEST_F(UtilsTest, ToLower) {
    EXPECT_EQ(Utils::ToLower("HeLLo"), "hello");
    EXPECT_EQ(Utils::ToLower("ALREADY_LOWER"), "already_lower");
    EXPECT_EQ(Utils::ToLower(""), "");
}

TEST_F(UtilsTest, Trim) {
    EXPECT_EQ(Utils::Trim("  hello  "), "hello");
    EXPECT_EQ(Utils::Trim("\t\n  "), "");
    EXPECT_EQ(Utils::Trim("no_spaces"), "no_spaces");
}