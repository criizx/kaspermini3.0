#include <gtest/gtest.h>
#include "../include/HashDB.h"
#include <filesystem>
#include <fstream>

class ScannerTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream csv("test_hashes.csv");
        csv << "65a8e27d8879283831b664bd8b7f0ad4;Trojan\n";
        csv.close();
    }

    void TearDown() override {
        std::filesystem::remove("test_hashes.csv");
    }
};

TEST_F(ScannerTest, MinimalHashDBTest) {
    std::cerr << "DEBUG: Starting MinimalHashDBTest" << std::endl;
    HashDB hashDB;
    std::cerr << "DEBUG: Exiting MinimalHashDBTest" << std::endl;
}