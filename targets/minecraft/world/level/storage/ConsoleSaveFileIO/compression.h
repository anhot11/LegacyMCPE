#pragma once
#include <mutex>
#include <vector>

#include "minecraft/world/level/storage/ConsoleSaveFileIO/FileHeader.h"

class Compression {
public:
    enum ECompressionTypes {
        eCompressionType_None = 0,
        eCompressionType_RLE = 1,
        eCompressionType_LZXRLE = 2,
        eCompressionType_ZLIBRLE = 3,
    };

private:
    class ThreadStorage {
    public:
        Compression* compression;
        ThreadStorage();
        ~ThreadStorage();
    };
    static thread_local ThreadStorage* m_tlsCompression;
    static ThreadStorage* m_tlsCompressionDefault;

public:
    static void CreateNewThreadStorage();
    static void UseDefaultThreadStorage();
    static void ReleaseThreadStorage();
    static Compression* getCompression();

    int32_t Compress(void* pDestination, unsigned int* pDestSize, void* pSource,
                     unsigned int SrcSize);
    int32_t Decompress(void* pDestination, unsigned int* pDestSize,
                       void* pSource, unsigned int SrcSize);
    int32_t CompressLZXRLE(void* pDestination, unsigned int* pDestSize,
                           void* pSource, unsigned int SrcSize);
    int32_t DecompressLZXRLE(void* pDestination, unsigned int* pDestSize,
                             void* pSource, unsigned int SrcSize);
    int32_t CompressRLE(void* pDestination, unsigned int* pDestSize,
                        void* pSource, unsigned int SrcSize);
    int32_t DecompressRLE(void* pDestination, unsigned int* pDestSize,
                          void* pSource, unsigned int SrcSize);

    void SetDecompressionType(ECompressionTypes type) {
        m_decompressType = type;
    }
    ECompressionTypes GetDecompressionType() { return m_decompressType; }
    void SetDecompressionType(ESavePlatform platform);

    Compression();
    ~Compression();

private:
    std::mutex rleCompressLock;
    std::mutex rleDecompressLock;

    std::vector<unsigned char> rleCompressBuf;
    std::vector<unsigned char> rleDecompressBuf;
    ECompressionTypes m_decompressType;
    ECompressionTypes m_localDecompressType;
};

#define APPROPRIATE_COMPRESSION_TYPE Compression::eCompressionType_ZLIBRLE
