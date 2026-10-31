#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "HashDB.h"
#include "Utils.h"

class HashDBTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream csv("test_hashes.csv");
        csv << "d41d8cd98f00b204e9800998ecf8427e;Trojan\n";
        csv << "invalid_hash;Malware\n";
        csv << "# Comment\n";
        csv << "a948904f2f0f479b8f8197694b30184b;Virus\n";
        csv.close();
    }

    void TearDown() override {
        std::filesystem::remove("test_hashes.csv");
    }
};

TEST_F(HashDBTest, LoadFromCSV_ValidFile) {
    HashDB db;
    EXPECT_TRUE(db.LoadFromCSV("test_hashes.csv"));
    EXPECT_EQ(db.GetHashCount(), 2);
}

TEST_F(HashDBTest, LoadFromCSV_NonExistentFile) {
    HashDB db;
    EXPECT_FALSE(db.LoadFromCSV("nonexistent.csv"));
    EXPECT_EQ(db.GetHashCount(), 0);
}

TEST_F(HashDBTest, CheckHash_ValidHash) {
    HashDB db;
    db.LoadFromCSV("test_hashes.csv");
    EXPECT_EQ(db.CheckHash("d41d8cd98f00b204e9800998ecf8427e"), "Trojan");
    EXPECT_EQ(db.CheckHash("a948904f2f0f479b8f8197694b30184b"), "Virus");
}

TEST_F(HashDBTest, CheckHash_NonExistentHash) {
    HashDB db;
    db.LoadFromCSV("test_hashes.csv");
    EXPECT_EQ(db.CheckHash("not_a_hash"), "");
}

TEST_F(HashDBTest, CheckHash_CaseInsensitive) {
    HashDB db;
    db.LoadFromCSV("test_hashes.csv");
    EXPECT_EQ(db.CheckHash("D41D8CD98F00B204E9800998ECF8427E"), "Trojan");
}

TEST_F(HashDBTest, RejectsNonHexHashesAndEmptyVerdicts) {
    {
        std::ofstream csv("test_hashes.csv");
        csv << "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz;Invalid\n";
        csv << "d41d8cd98f00b204e9800998ecf8427e;   \n";
    }
    HashDB db;
    EXPECT_FALSE(db.LoadFromCSV("test_hashes.csv"));
    EXPECT_EQ(db.GetHashCount(), 0);
}

TEST_F(HashDBTest, FailedReloadPreservesPreviousDatabase) {
    HashDB db;
    ASSERT_TRUE(db.LoadFromCSV("test_hashes.csv"));
    {
        std::ofstream csv("test_hashes.csv");
        csv << "invalid;Malware\n";
    }
    EXPECT_FALSE(db.LoadFromCSV("test_hashes.csv"));
    EXPECT_EQ(db.GetHashCount(), 2);
    EXPECT_EQ(db.CheckHash("d41d8cd98f00b204e9800998ecf8427e"), "Trojan");
}

TEST_F(HashDBTest, SuccessfulReloadReplacesPreviousDatabase) {
    HashDB db;
    ASSERT_TRUE(db.LoadFromCSV("test_hashes.csv"));
    {
        std::ofstream csv("test_hashes.csv");
        csv << "  # Indented comment\n";
        csv << " D41D8CD98F00B204E9800998ECF8427E ; Updated \n";
    }
    ASSERT_TRUE(db.LoadFromCSV("test_hashes.csv"));
    EXPECT_EQ(db.GetHashCount(), 1);
    EXPECT_EQ(db.CheckHash("d41d8cd98f00b204e9800998ecf8427e"), "Updated");
    EXPECT_TRUE(db.CheckHash("a948904f2f0f479b8f8197694b30184b").empty());
}
