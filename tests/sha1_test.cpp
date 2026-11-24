#include <gtest/gtest.h>
#include "mygit/sha1.hpp"

TEST(SHA1_Test, EmptyString){
    EXPECT_EQ(sha1(""), "da39a3ee5e6b4b0d3255bfef95601890afd80709");
}

TEST(SHA1_Test, ABC){
    EXPECT_EQ(sha1("abc"), "a9993e364706816aba3e25717850c26c9cd0d89d");
}

TEST(SHA1_Test, TwoBlocks){
    EXPECT_EQ(sha1("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"), "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
}

