#include "minecraft/util/Mth.h"

#include <stddef.h>
#include <stdint.h>

#include <array>
#include <numbers>
#include <sstream>
#include <string>

#include "java/Random.h"

constexpr size_t SIN_TAB_CNT = 65536;
constexpr float SIN_SCALE = SIN_TAB_CNT / (std::numbers::pi_v<float> * 2.0f);

static std::array<float, SIN_TAB_CNT> makeSinTable() {
    std::array<float, SIN_TAB_CNT> t{};
    for (size_t i = 0; i < SIN_TAB_CNT; i++)
        t[i] = (float)::sin(i * std::numbers::pi * 2.0 / (double)SIN_TAB_CNT);
    return t;
}

static const std::array<float, SIN_TAB_CNT> sinTable = makeSinTable();

float Mth::sin(float i) {
    return sinTable[(int32_t)::fmodf(i * SIN_SCALE, (float)SIN_TAB_CNT) &
                    (SIN_TAB_CNT - 1)];
}

float Mth::cos(float i) {
    return sinTable[(int32_t)::fmodf(i * SIN_SCALE + (float)SIN_TAB_CNT / 4,
                                     (float)SIN_TAB_CNT) &
                    (SIN_TAB_CNT - 1)];
}

// 4J Changed this to remove the use of the actual UUID type
std::string Mth::createInsecureUUID(Random* random) {
    char output[33];
    output[32] = 0;
    int64_t high = (random->nextLong() & ~UUID_VERSION) | UUID_VERSION_TYPE_4;
    int64_t low = (random->nextLong() & ~UUID_VARIANT) | UUID_VARIANT_2;
    for (int i = 0; i < 16; i++) {
        char nybbleHigh = high & 0xf;
        char nybbleLow = low & 0xf;
        nybbleHigh =
            (nybbleHigh > 9) ? (nybbleHigh + ('a' - 10)) : (nybbleHigh + '0');
        nybbleLow =
            (nybbleLow > 9) ? (nybbleLow + ('a' - 10)) : (nybbleLow + '0');
        high >>= 4;
        low >>= 4;
        output[31 - i] = nybbleLow;
        output[15 - i] = nybbleHigh;
    }
    return std::string(output);
}

int Mth::getInt(const std::string& input, int def) {
    std::istringstream stream(input);
    int result;
    return (stream >> result) ? result : def;
}

int Mth::getInt(const std::string& input, int def, int min) {
    int result = getInt(input, def);
    return result < min ? min : result;
}

double Mth::getDouble(const std::string& input, double def) {
    std::istringstream stream(input);
    double result;
    return (stream >> result) ? result : def;
}

double Mth::getDouble(const std::string& input, double def, double min) {
    double result = getDouble(input, def);
    return result < min ? min : result;
}
