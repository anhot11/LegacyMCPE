#pragma once

#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <numbers>
#include <string>

class Random;

class Mth {
public:
    static constexpr float DEG_TO_RAD = std::numbers::pi_v<float> / 180.0f;
    static constexpr float RAD_TO_DEG = 180.0f / std::numbers::pi_v<float>;

    static constexpr int64_t UUID_VERSION = 0x000000000000f000L;
    static constexpr int64_t UUID_VERSION_TYPE_4 = 0x0000000000004000L;
    static constexpr int64_t UUID_VARIANT = 0xc000000000000000L;
    static constexpr int64_t UUID_VARIANT_2 = 0x8000000000000000L;

    static float sin(float i);
    static float cos(float i);

    static float sqrt(float x) { return (float)::sqrt(x); }
    static float sqrt(double x) { return (float)::sqrt(x); }

    static int floor(float v) { return (int)::floorf(v); }
    static int floor(double v) { return (int)::floor(v); }
    static int64_t lfloor(double v) { return (int64_t)::floor(v); }

    static int fastFloor(double x) { return (int)(x + 1024.0) - 1024; }

    static float abs(float v) { return v >= 0.0f ? v : -v; }
    static int abs(int v) { return v >= 0 ? v : -v; }

    static int ceil(float v) { return (int)::ceilf(v); }

    static int clamp(int value, int min, int max) {
        return std::clamp(value, min, max);
    }
    static float clamp(float value, float min, float max) {
        return std::clamp(value, min, max);
    }

    static int intFloorDiv(int a, int b) {
        if (a < 0) return -((-a - 1) / b) - 1;
        return a / b;
    }

    static float wrapDegrees(float input) {
        return (float)::remainder((double)input, 360.0);
    }
    static double wrapDegrees(double input) {
        return ::remainder(input, 360.0);
    }

    static std::string createInsecureUUID(Random* random);

    static int getInt(const std::string& input, int def);
    static int getInt(const std::string& input, int def, int min);
    static double getDouble(const std::string& input, double def);
    static double getDouble(const std::string& input, double def, double min);
};
