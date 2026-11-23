#pragma once

#include <cstdint>
#include <array>

class MSA
{
private:
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;

    const uint32_t k0 = 0x5A827999;
    const uint32_t k1 = 0x6ED9EBA1;
    const uint32_t k2 = 0x8F1BBCDC;
    const uint32_t k3 = 0xCA62C1D6;
private:
    uint32_t choose_bits();
    uint32_t parity();
    uint32_t majority();
public:
    MSA(
        uint32_t a,
        uint32_t b,
        uint32_t c,
        uint32_t d,
        uint32_t e
    );
    void calculate(uint32_t word, int round);
    std::array<uint32_t, 5> ret_h();
};   

