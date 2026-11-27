#include "mygit/commands.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(HashObjectTest, LessThanOne){
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "Usage: mygit hash-object [option] [filename / text]\n");
}

TEST(HashObjectTest, UnknownOption){
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {"--unknown"};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "Command Not Found\n");
}

TEST(HashObjectTest, MoreThanTwo){
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {"--stdin", "abc.txt"};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "Usage: mygit hash-object [option] [filename / text]\n");
}

TEST(HashObjectTest, FileNotPresent){
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {std::string(TEST_DATA_DIR) + "/nofile.txt"};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "File not found\n");
}

TEST(HashObjectTest, EmptyFile){
    testing::internal::CaptureStdout();
    std::vector<std::string> input = {std::string(TEST_DATA_DIR) + "/empty.txt"};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "e69de29bb2d1d6434b8b29ae775ad8c2e48c5391\n");
}

TEST(HashObjectTest, ABCFile){
    testing::internal::CaptureStdout();
    std::vector<std::string> input = {std::string(TEST_DATA_DIR) + "/abc.txt"};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "f2ba8f84ab5c1bce84a7b441cb1959cfc7093b7f\n");
}

TEST(HashObjectTest, ContentPassed){
    testing::internal::CaptureStdout();
    std::vector<std::string> input = {"--stdin"};
    std::istringstream in("abc");
    int rc = hash_object(input, in);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "f2ba8f84ab5c1bce84a7b441cb1959cfc7093b7f\n");
}