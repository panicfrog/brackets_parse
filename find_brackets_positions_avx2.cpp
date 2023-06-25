//
// Created by 叶永平 on 2023/6/25.
//

#include "find_brackets_positions.hpp"
#include <immintrin.h>

std::vector<size_t> find_brackets_positions(const std::string& json) {
    size_t padding = 32 - (json.size() % 32);
    auto total_size = padding + json.size();
    std::string _json;
    _json.reserve(total_size);
    _json += json;
    _json.append(padding, static_cast<char>(padding));

    std::vector<size_t> positions;
    __m256i left_bracket = _mm256_set1_epi8('{');
    __m256i right_bracket = _mm256_set1_epi8('}');



    for (size_t i = 0; i < _json.size(); i += 32) {
        __m256i data = _mm256_loadu_si256((__m256i*)&_json[i]);

        __m256i left_result = _mm256_cmpeq_epi8(data, left_bracket);
        __m256i right_result = _mm256_cmpeq_epi8(data, right_bracket);

        int left_mask = _mm256_movemask_epi8(left_result);
        int right_mask = _mm256_movemask_epi8(right_result);

        while (left_mask) {
            uint32_t bit_position = _tzcnt_u32(left_mask);
            left_mask &= left_mask - 1;  // Clear the lowest set bit.
            positions.push_back(i + bit_position);
        }

        while (right_mask) {
            uint32_t bit_position = _tzcnt_u32(right_mask);
            right_mask &= right_mask - 1;  // Clear the lowest set bit.
            positions.push_back(i + bit_position);
        }
    }

    return positions;
}