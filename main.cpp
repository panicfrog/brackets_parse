#include <iostream>
#include "find_brackets_positions.hpp"
#include <chrono>

std::vector<size_t> find_brackets_positions_common(const std::string& json) {
    std::vector<size_t> positions;
    positions.reserve(json.size() / 6);
    for (size_t i = 0; i < json.size(); ++i) {
        if (json[i] == '{' || json[i] == '}') {
            positions.push_back(i);
        }
    }
    return positions;
}

int main() {
//    std::string json = R"({
//        "name": "John Doe",
//        "age": 30,
//        "cars": {
//            "car1": "Ford",
//            "car2": "BMW",
//            "car3": "Fiat"
//        },
//        "friends": [
//            {"name": "Jane", "age": 28},
//            {"name": "Robert", "age": 35}
//        ]
//    })";

    std::string json = R"({
      "employees": [
        {
          "firstName": "John",
          "lastName": "Doe"
        },
        {
          "firstName": "Anna",
          "lastName": "Smith"
        },
        {
          "firstName": "Peter",
          "lastName": "Jones"
        }
      ],
      "locations": {
        "New York": {
          "employees": 100,
          "branch": {
            "name": "Central",
            "manager": "John Doe"
          }
        },
        "San Francisco": {
          "employees": 200,
          "branch": {
            "name": "West Coast",
            "manager": "Anna Smith"
          }
        },
        "Austin": {
          "employees": 150,
          "branch": {
            "name": "South",
            "manager": "Peter Jones"
          }
        }
      },
      "revenue": [
        {
          "year": 2021,
          "amount": 1000000
        },
        {
          "year": 2022,
          "amount": 1500000
        },
        {
          "year": 2023,
          "amount": 2000000
        }
      ]
    })";

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<size_t> positions = find_brackets_positions(json);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::vector<size_t> positions2 = find_brackets_positions_common(json);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::sort(positions.begin(), positions.end());
    std::sort(positions2.begin(), positions2.end());
    std::cout << "Bracket positions 1:";
    for (size_t pos : positions) {
        std::cout << " " << pos;
    }
    std::cout << " time: " << (double)((end1 - start).count() / 1000) / 1000.0 << "ms";
    std::cout << std::endl;

    std::cout << "Bracket positions 2:";
    for (size_t pos : positions2) {
        std::cout << " " << pos;
    }
    std::cout << " time: " << (double)((end2 - end1).count() / 1000) / 1000.0 << "ms";
    std::cout << std::endl;
    std::cout << std::endl;

    return 0;
}
