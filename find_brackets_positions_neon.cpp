//
// Created by 叶永平 on 2023/6/25.
//

#include "find_brackets_positions.hpp"
#include <arm_neon.h>

std::vector<size_t> find_brackets_positions(const std::string& json) {
    size_t padding = 16 - (json.size() % 16);
    auto total_size = padding + json.size();
    std::string _json;
    _json.reserve(total_size);
    _json += json;
    _json.append(padding, static_cast<char>(padding));

    std::vector<size_t> positions;
    uint8x16_t left_bracket = vdupq_n_u8('{');
    uint8x16_t right_bracket = vdupq_n_u8('}');

    for (size_t i = 0; i < _json.size(); i += 16) {
        uint8x16_t data = vld1q_u8(reinterpret_cast<const uint8_t*>(&_json[i]));

        uint8x16_t left_result = vceqq_u8(data, left_bracket);
        uint8x16_t right_result = vceqq_u8(data, right_bracket);

        uint16_t left_mask = vgetq_lane_u16(vreinterpretq_u16_u8(left_result), 0);
        uint16_t right_mask = vgetq_lane_u16(vreinterpretq_u16_u8(right_result), 0);

        while (left_mask) {
            uint32_t bit_position = 31 - __builtin_clz(left_mask);
            left_mask &= left_mask - 1;  // Clear the lowest set bit.
            positions.push_back(i + bit_position);
        }
        while (right_mask) {
            uint32_t bit_position = 31 - __builtin_clz(right_mask);
            right_mask &= right_mask - 1;  // Clear the lowest set bit.
            positions.push_back(i + bit_position);
        }
    }

    return positions;
}