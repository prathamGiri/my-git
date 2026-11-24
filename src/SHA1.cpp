#include "mygit/sha1.hpp"

#include <string>
#include <array>
#include <cstdint>
#include <string_view>
#include <cstring>

namespace {
    constexpr uint32_t h0 = 0x67452301;
    constexpr uint32_t h1 = 0xEFCDAB89;
    constexpr uint32_t h2 = 0x98BADCFE;
    constexpr uint32_t h3 = 0x10325476;
    constexpr uint32_t h4 = 0xC3D2E1F0;

    constexpr uint32_t k0 = 0x5A827999;
    constexpr uint32_t k1 = 0x6ED9EBA1;
    constexpr uint32_t k2 = 0x8F1BBCDC;
    constexpr uint32_t k3 = 0xCA62C1D6;

    constexpr std::string digits = "0123456789abcdef";
    constexpr size_t block_size = 64;
    constexpr int num_rounds = 80;
    constexpr size_t length_offset = 56;
    constexpr size_t hash_char_size = 40;

    void update(
        const std::array<uint8_t, block_size> &block,
        std::array<uint32_t, 5> &state
    ){
        std::array<uint32_t, 5> curr_state = state;
        std::array<uint32_t, num_rounds> w{};
        for (int i = 0; i < num_rounds; i++)
        {
            if (i >= 16)
            {
                w[i] = w[i-16]^w[i-14]^w[i-8]^w[i-3];
                w[i] = (w[i]<<1) | (w[i]>>31); 
            }else
            {
                for(int j = 0; j < 4; j++){
                    w[i] |= uint32_t(block[4*i+j])<<(8*(3-j));
                } 
            }
            std::array<uint32_t, 5> temp{};
            temp[1] = curr_state[0];
            temp[2] = (curr_state[1]<<30)|(curr_state[1]>>2);
            temp[3] = curr_state[2];
            temp[4] = curr_state[3];
            temp[0] = w[i]+((curr_state[0]<<5)|(curr_state[0]>>27))+curr_state[4];

            if (i <= 19)
            {
                temp[0] += k0+((curr_state[1]&curr_state[2])|(~curr_state[1]&curr_state[3]));
            }else if (i <= 39)
            {
                temp[0] += k1+(curr_state[1]^curr_state[2]^curr_state[3]);
            }
            else if (i <= 59)
            {
                temp[0] += k2+((curr_state[1]&curr_state[2])^(curr_state[2]&curr_state[3])^(curr_state[1]&curr_state[3]));
            }else{
                temp[0] += k3+(curr_state[1]^curr_state[2]^curr_state[3]);
            }
            curr_state=temp;
        }
        for (int i = 0; i < 5; i++)
        {
            state[i] += curr_state[i];
        }
    }

    std::string finalize(
        const std::array<uint32_t, 5> &state
    ){
        std::string sha1_hash;
        sha1_hash.reserve(hash_char_size);
        for (int j = 0; j < 5; j++)
        {
            for (int i = 28; i >= 0; i-=4) sha1_hash.push_back(digits[(state[j]>>i)&0x0F]);
        }
        return sha1_hash;
    }
}

std::string sha1(std::string_view msg){
    std::array<uint32_t, 5> state = {h0, h1, h2, h3, h4};

    const auto* data = reinterpret_cast<const uint8_t*>(msg.data());
    const size_t msg_size = msg.size();
    const uint64_t msg_bits = uint64_t(msg_size)*8;
    size_t off = 0;
    for (; off+block_size <= msg_size; off+=block_size)
    {
        std::array<uint8_t, block_size> block;
        std::memcpy(block.data(), data+off, block_size);
        update(block,state);
    }
    std::array<uint8_t, block_size> block{};
    size_t rem = msg_size-off;
    std::memcpy(block.data(), data+off, rem);
    block[rem] = 0x80;
    if (rem >= length_offset)
    {
        update(block,state);
        block.fill(0);
    }
    for(int i = 0; i < 8; i++){
        block[63-i] = uint8_t(msg_bits >> (8*i));
    }
    update(block,state);
    return finalize(state);
}