#include "mygit/zlib.hpp"

#include <gtest/gtest.h>
#include <string>
 
TEST(Zlib_Tests, CompressionTest){
    std::string test_string = "This is a test string to Compress!";
    std::string compressed_data = zcompress(test_string);
    EXPECT_EQ(zdecompress(compressed_data), test_string);
}