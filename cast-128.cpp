#include "cast-128.hpp"
#include "s-boxes.hpp"
#include <array>
#include <cstdint>
#include <vector>

// std::array<uint8_t, 16> key = {};

uint32_t circ_shift(uint32_t a, uint32_t b) {
    uint32_t res = (a << b) | (a >> (32 - b));
    return res;
}
std::vector<uint8_t> divide_I(uint32_t I) {
    std::vector<uint8_t> res;
    for (int i = 0; i < 4; i++) {
        uint8_t b = (I >> (24 - (8 * i))) & 0xFF;
        res.push_back(b);
    }
    return res;
}
uint32_t f1(uint32_t Km, uint32_t D, uint32_t Kr) {
    uint32_t I = circ_shift((Km + D), Kr);
    std::vector<uint8_t> divided_I = divide_I(I);
    uint32_t f = ((S1[divided_I[0]] ^ S2[divided_I[1]]) - S3[divided_I[2]]) +
                 S4[divided_I[3]];
    return f;
}
