#pragma once

#include <stdint.h>

#include <format>
#include <vector>

class BlockReplacements {
public:
    static void staticCtor();

private:
    static std::vector<uint8_t> replacements;

public:
    static void replace(std::vector<uint8_t>& blocks);
};