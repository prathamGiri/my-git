#pragma once

#include <cstdint>
#include <array>
#include <string_view>
#include <string>

class SHA1
{
private:
    const uint32_t h0 = 0x67452301;
    const uint32_t h1 = 0xEFCDAB89;
    const uint32_t h2 = 0x98BADCFE;
    const uint32_t h3 = 0x10325476;
    const uint32_t h4 = 0xC3D2E1F0;

    uint32_t s0 = h0;
    uint32_t s1 = h1;
    uint32_t s2 = h2;
    uint32_t s3 = h3;
    uint32_t s4 = h4;

private:
    void update(std::array<uint8_t, 64> block);
public:
    std::string block_stream(std::string_view msg);
};
