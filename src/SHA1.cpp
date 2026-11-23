#include "SHA1.hpp"
#include "MSA.hpp"

#include <string_view>
#include <array>
#include <cstring>
#include <cstdint>
#include <string>

std::string SHA1::block_stream(std::string_view msg){
    const auto* data = reinterpret_cast<const uint8_t*>(msg.data());
    const size_t msg_size = msg.size();
    const uint64_t msg_bits = uint64_t(msg_size)*8;
    size_t off = 0;
    for (; off+64 <= msg_size; off+=64)
    {
        std::array<uint8_t, 64> block;
        std::memcpy(block.data(), data+off, 64);
        update(block);
    }
    std::array<uint8_t, 64> block{};
    size_t rem = msg_size-off;
    std::memcpy(block.data(), data+off, rem);
    block[rem] = 0x80;
    if (rem >= 56)
    {
        update(block);
        block.fill(0);
    }
    for(int i = 0; i < 8; i++){
        block[63-i] = uint8_t(msg_bits >> (8*i));
    }
    update(block);
    std::string digits = "0123456789abcdef";
    std::string sha1_hash;
    for (int i = 28; i >= 0; i-=4) sha1_hash.push_back(digits[(s0>>i)&0x0F]);
    for (int i = 28; i >= 0; i-=4) sha1_hash.push_back(digits[(s1>>i)&0x0F]);
    for (int i = 28; i >= 0; i-=4) sha1_hash.push_back(digits[(s2>>i)&0x0F]);
    for (int i = 28; i >= 0; i-=4) sha1_hash.push_back(digits[(s3>>i)&0x0F]);
    for (int i = 28; i >= 0; i-=4) sha1_hash.push_back(digits[(s4>>i)&0x0F]);
    s0 = h0;
    s1 = h1;
    s2 = h2;
    s3 = h3;
    s4 = h4;
    return sha1_hash;
}

void SHA1::update(std::array<uint8_t, 64> block){
    uint32_t a = s0;
    uint32_t b = s1;
    uint32_t c = s2;
    uint32_t d = s3;
    uint32_t e = s4;

    MSA msa(a,b,c,d,e);

    std::array<uint32_t, 80> w{};
    for (int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 4; j++){
            w[i] |= uint32_t(block[4*i+j])<<(8*(3-j));
        }   
    } 
    for (int i = 0; i < 80; i++)
    {
        if (i >= 16)
        {
            w[i] = w[i-16]^w[i-14]^w[i-8]^w[i-3];
            w[i] = (w[i]<<1) | (w[i]>>31); 
        }
        msa.calculate(w[i], i);
    }
    std::array<uint32_t, 5> res = msa.ret_h();
    s4 = res[4]+s4;
    s3 = res[3]+s3;
    s2 = res[2]+s2;
    s1 = res[1]+s1;
    s0 = res[0]+s0;
}

