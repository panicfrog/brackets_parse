//
// Created by 叶永平 on 2023/6/25.
//

#include "find_brackets_positions.hpp"
#include <arm_neon.h>

constexpr size_t BLOCK_SIZE = 16;

void get_indexes(std::vector<size_t>& results, const uint8x16_t& bit_result, size_t i) {
    uint64x2_t x1 = vreinterpretq_u64_u8(bit_result);
    uint64_t bitset[2];
    vst1q_u64(bitset, x1);

    for (int idx = 0; idx < 2; ++idx) {
        while (bitset[idx]) {
            auto mask = __builtin_ctzll(bitset[idx]);
            size_t position = (mask / 8) + (idx * 8);
            results.push_back(i + position);
            bitset[idx] = bitset[idx] & ~(1ull << mask);
        }
    }
}

std::vector<size_t> find_brackets_positions(const std::string& json) {
    size_t padding = (json.size() % BLOCK_SIZE) ? (BLOCK_SIZE - (json.size() % BLOCK_SIZE)) : 0;
    auto total_size = padding + json.size();
    std::string _json;
    _json.reserve(total_size);
    _json += json;
    if (padding) {
        _json.append(padding, static_cast<char>(padding));
    }

    std::vector<size_t> positions;
    positions.reserve(_json.size() / 6);
    uint8x16_t left_bracket = vdupq_n_u8('{');
    uint8x16_t right_bracket = vdupq_n_u8('}');

    // TODO: bit_mask 是非必要操作，可以进一步提升性能
    const uint8x16_t bit_mask = {
            0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
    };

    for (size_t i = 0; i < total_size; i += BLOCK_SIZE) {
        uint8x16_t data = vld1q_u8(reinterpret_cast<const uint8_t*>(&_json[i]));

        uint8x16_t left_result = vceqq_u8(data, left_bracket);
        // TODO: 这个寄存器的操作的开销可以避免，可以直接操作上面的返回值
        uint8x16_t right_result = vceqq_u8(data, right_bracket);

        uint8x16_t left_bit_result = vandq_u8(left_result, bit_mask);
        // TODO: 这个寄存器的操作的开销可以避免，可以直接操作上面的返回值
        uint8x16_t right_bit_result = vandq_u8(right_result, bit_mask);

        get_indexes(positions, left_bit_result, i);
        get_indexes(positions, right_bit_result, i);
    }

    return positions;
}
