#include "Synth.h"

std::vector<double> Synth::create(int width, int height) {
    std::vector<double> result = std::vector<double>(width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result[x + y * width] = getValue(x, y);
        }
    }
    return result;
}
