#include "mygit/commands.hpp"

#include <gtest/gtest.h>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

const std::string abcfile = std::string(TEST_DATA_DIR) + "/file.txt";
const std::string nofile = std::string(TEST_DATA_DIR) + "/nofile.txt";
const std::string emptyfile = std::string(TEST_DATA_DIR) + "/empty.txt"

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

// TEST(HashObjectTest, MoreThanTwo){
//     testing::internal::CaptureStderr();
//     std::vector<std::string> input = {"--stdin", "abc.txt"};
//     int rc = hash_object(input);
//     std::string output = testing::internal::GetCapturedStderr();

//     EXPECT_EQ(rc, 1);
//     EXPECT_EQ(output, "Usage: mygit hash-object [option] [filename / text]\n");
// }

TEST(HashObjectTest, WriteObjectSingleArg){
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {"-w"};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "Usage: mygit hash-object -w [filename]\n");
}

TEST(HashObjectTest, WriteObjectNoGit){
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {"-w", abcfile};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, ".git Folder not found: Use git init\n");
}

TEST(HashObjectTest, WriteObjectNoFile){
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {"-w", nofile};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "File not found\n");
}

TEST(HashObjectTest, WriteObjectPassed){
    fs::create_directory(".git/objects");
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {"-w", abcfile};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "f2ba8f84ab5c1bce84a7b441cb1959cfc7093b7f\n");
    EXPECT_TRUE(fs::exists(".git/objects/f2/ba8f84ab5c1bce84a7b441cb1959cfc7093b7f"));
}

TEST(HashObjectTest, FileNotPresent){
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {nofile};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "File not found\n");
}

TEST(HashObjectTest, EmptyFile){
    testing::internal::CaptureStdout();
    std::vector<std::string> input = {emptyfile};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "e69de29bb2d1d6434b8b29ae775ad8c2e48c5391\n");
}

TEST(HashObjectTest, ABCFile){
    testing::internal::CaptureStdout();
    std::vector<std::string> input = {abcfile};
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