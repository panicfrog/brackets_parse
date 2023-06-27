//
// Created by 叶永平 on 2023/6/25.
//

#include "find_brackets_positions.hpp"

std::vector<size_t> find_brackets_positions(const std::string& json) {
    std::vector<size_t> positions;
    positions.reserve(json.size() / 6);
    for (size_t i = 0; i < json.size(); ++i) {
        if (json[i] == '{' || json[i] == '}') {
            positions.push_back(i);
        }
    }
    return positions;
}