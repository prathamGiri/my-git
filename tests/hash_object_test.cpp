#include "mygit/commands.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(HashObjectTest, LessThanOne){
    testing::internal::CaptureStderr();
    int rc = hash_object({});
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "Usage: mygit hash-object [option] [filename / text]\n");
}

TEST(HashObjectTest, UnknownOption){
    testing::internal::CaptureStderr();
    int rc = hash_object({"--unknown"});
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "Command Not Found\n");
}

TEST(HashObjectTest, MoreThanTwo){
    testing::internal::CaptureStderr();
    int rc = hash_object({"--stdin", "abc.txt"});
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "Usage: mygit hash-object [option] [filename / text]\n");
}

TEST(HashObjectTest, FileNotPresent){
    testing::internal::CaptureStderr();
    int rc = hash_object({std::string(TEST_DATA_DIR) + "/nofile.txt"});
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "File not found\n");
}

TEST(HashObjectTest, EmptyFile){
    testing::internal::CaptureStdout();
    int rc = hash_object({std::string(TEST_DATA_DIR) + "/empty.txt"});
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "e69de29bb2d1d6434b8b29ae775ad8c2e48c5391\n");
}

TEST(HashObjectTest, ABCFile){
    testing::internal::CaptureStdout();
    int rc = hash_object({std::string(TEST_DATA_DIR) + "/abc.txt"});
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "8baef1b4abc478178b004d62031cf7fe6db6f903\n");
}

TEST(HashObjectTest, ContentPassed){
    testing::internal::CaptureStdout();
    int rc = hash_object({"--stdin"});
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "8baef1b4abc478178b004d62031cf7fe6db6f903\n");
}