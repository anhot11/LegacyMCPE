#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

class Biome;

class Arrays {
public:
    static void fill(std::vector<double>& arr, unsigned int from,
                     unsigned int to, double value) {
        assert(from >= 0);
        assert(from <= to);
        assert(to <= arr.size());
        std::fill(arr.data() + from, arr.data() + to, value);
    }

    static void fill(std::vector<float>& arr, unsigned int from,
                     unsigned int to, float value) {
        assert(from >= 0);
        assert(from <= to);
        assert(to <= arr.size());
        std::fill(arr.data() + from, arr.data() + to, value);
    }

    static void fill(std::vector<Biome*>& arr, unsigned int from,
                     unsigned int to, Biome* value) {
        assert(from >= 0);
        assert(from <= to);
        assert(to <= arr.size());
        std::fill(arr.data() + from, arr.data() + to, value);
    }

    static void fill(std::vector<uint8_t>& arr, unsigned int from,
                     unsigned int to, uint8_t value) {
        assert(from >= 0);
        assert(from <= to);
        assert(to <= arr.size());
        std::fill(arr.data() + from, arr.data() + to, value);
    }

    static void fill(std::vector<uint8_t>& arr, uint8_t value) {
        std::fill(arr.data(), arr.data() + arr.size(), value);
    }
};
