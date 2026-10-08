#pragma once

#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <numbers>
#include <string>

class Random;

extern bool g_optifineFastMath;

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

    static inline float sqrt(float x) {
#if defined(__GNUC__) || defined(__clang__)
        if (g_optifineFastMath) {
            return __builtin_sqrtf(x);
        }
#endif
        return (float)::sqrt(x);
    }
    static inline float sqrt(double x) {
#if defined(__GNUC__) || defined(__clang__)
        if (g_optifineFastMath) {
            return (float)__builtin_sqrt(x);
        }
#endif
        return (float)::sqrt(x);
    }

    static inline int floor(float v) {
        if (g_optifineFastMath) {
            int i = (int)v;
            return v < (float)i ? i - 1 : i;
        }
        return (int)::floorf(v);
    }
    static inline int floor(double v) {
        if (g_optifineFastMath) {
            int i = (int)v;
            return v < (double)i ? i - 1 : i;
        }
        return (int)::floor(v);
    }
    static inline int64_t lfloor(double v) {
        if (g_optifineFastMath) {
            int64_t i = (int64_t)v;
            return v < (double)i ? i - 1 : i;
        }
        return (int64_t)::floor(v);
    }

    static inline int fastFloor(double x) {
        int i = (int)x;
        return x < (double)i ? i - 1 : i;
    }

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
