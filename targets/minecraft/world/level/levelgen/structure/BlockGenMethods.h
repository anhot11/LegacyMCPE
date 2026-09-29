#pragma once

#include <cstdint>
#include <format>
#include <vector>

class Level;

class BlockGenMethods {
public:
    static void generateBox(Level* level, std::vector<uint8_t>& blocks, int sx,
                            int sy, int sz, int ex, int ey, int ez,
                            std::uint8_t edge, std::uint8_t filling);
    static void generateFrame(Level* level, std::vector<uint8_t>& blocks,
                              int sx, int sy, int ex, int ey, int flatZ,
                              int direction, std::uint8_t edge,
                              std::uint8_t filling);
    static void generateDirectionLine(Level* level,
                                      std::vector<uint8_t>& blocks, int sx,
                                      int sy, int sz, int ex, int ey, int ez,
                                      int startDirection, int endDirection,
                                      std::uint8_t block);
    static void generateLine(Level* level, std::vector<uint8_t>& blocks, int sx,
                             int sy, int sz, int ex, int ey, int ez,
                             std::uint8_t block);
};
