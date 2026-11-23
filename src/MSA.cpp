#include "MSA.hpp"

#include <cstdint>
#include <array>

MSA::MSA(
        uint32_t a,
        uint32_t b,
        uint32_t c,
        uint32_t d,
        uint32_t e
    ):s0(a),s1(b),s2(c),s3(d),s4(e){};

uint32_t MSA::choose_bits(){
    return (s1&s2)|(~s1&s3);
}

uint32_t MSA::parity(){
    return s1^s2^s3;
}

uint32_t MSA::majority(){
    return (s1&s2)^(s2&s3)^(s1&s3);
}

void MSA::calculate(uint32_t word, int round){
    uint32_t b_dash = s0;
    uint32_t c_dash = (s1<<30)|(s1>>2);
    uint32_t d_dash = s2;
    uint32_t e_dash = s3;
    uint32_t a_dash = word+((s0<<5)|(s0>>27))+s4;

    if (round <= 19)
    {
        a_dash += k0+choose_bits();
    }else if (round <= 39)
    {
        a_dash += k1+parity();
    }
    else if (round <= 59)
    {
        a_dash += k2+majority();
    }else{
        a_dash += k3+parity();
    }
    s0 = a_dash;
    s1 = b_dash;
    s2 = c_dash;
    s3 = d_dash;
    s4 = e_dash;
}

std::array<uint32_t, 5> MSA::ret_h(){
    return {s0, s1, s2, s3, s4};
}