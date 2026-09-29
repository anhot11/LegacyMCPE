#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

class Biome;
class Node;

// 4J Jev, just thought it would be easier this way.
#define ArrayCopyFunctionDeclaration(x)                                   \
    static void arraycopy(const std::vector<x>& src, unsigned int srcPos, \
                          std::vector<x>* dst, unsigned int dstPos,       \
                          unsigned int length);
#define ArrayCopyFunctionDefinition(x)                                     \
    void System::arraycopy(const std::vector<x>& src, unsigned int srcPos, \
                           std::vector<x>* dst, unsigned int dstPos,       \
                           unsigned int length) {                          \
        arraycopy<x>(src, srcPos, dst, dstPos, length);                    \
    }

class System {
    template <class T>
    static void arraycopy(const std::vector<T>& src, unsigned int srcPos,
                          std::vector<T>* dst, unsigned int dstPos,
                          unsigned int length);

public:
    ArrayCopyFunctionDeclaration(uint8_t) ArrayCopyFunctionDeclaration(Node*)
        ArrayCopyFunctionDeclaration(Biome*) ArrayCopyFunctionDeclaration(int)

            static int64_t nanoTime();
    static int64_t currentTimeMillis();
    static int64_t currentRealTimeMillis();  // 4J Added to get real-world time
                                             // for timestamps in saves

    static void ReverseUSHORT(unsigned short* pusVal);
    static void ReverseSHORT(short* psVal);
    static void ReverseULONG(unsigned long* pulVal);
    static void ReverseULONG(unsigned int* pulVal);
    static void ReverseINT(int* piVal);
    static void ReverseULONGLONG(int64_t* pullVal);
    static void ReverseWCHARA(char* pwch, int iLen);
};
