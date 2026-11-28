#include "mygit/commands.hpp"
#include "mygit/zlib.hpp"

#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

const std::string abcfile = std::string(TEST_DATA_DIR) + "/abc.txt";
const std::string nofile = std::string(TEST_DATA_DIR) + "/nofile.txt";
const std::string emptyfile = std::string(TEST_DATA_DIR) + "/empty.txt";

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
    fs::create_directories(".git/objects");
    testing::internal::CaptureStderr();
    std::vector<std::string> input = {"-w", nofile};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(rc, 1);
    EXPECT_EQ(output, "File not found\n");
}

TEST(HashObjectTest, WriteObjectPassed){
    fs::create_directories(".git/objects");
    testing::internal::CaptureStdout();
    std::vector<std::string> input = {"-w", abcfile};
    int rc = hash_object(input);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, 0);
    EXPECT_EQ(output, "f2ba8f84ab5c1bce84a7b441cb1959cfc7093b7f\n");
    EXPECT_TRUE(fs::exists(".git/objects/f2/ba8f84ab5c1bce84a7b441cb1959cfc7093b7f"));

    std::ifstream actual_in(abcfile, std::ios::binary);
    std::string actual_data(
        (std::istreambuf_iterator<char>(actual_in)),
        std::istreambuf_iterator<char>()
    );
    actual_data = "blob " + std::to_string(actual_data.size()) + '\0' + actual_data;

    std::ifstream comp_in(".git/objects/f2/ba8f84ab5c1bce84a7b441cb1959cfc7093b7f", std::ios::binary);
    std::string comp_data(
        (std::istreambuf_iterator<char>(comp_in)),
        std::istreambuf_iterator<char>()
    );
    EXPECT_EQ(zdecompress(comp_data), actual_data);
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