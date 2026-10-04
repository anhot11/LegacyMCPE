#include "minecraft/client/BufferedImage.h"

#include <string.h>

#include <cstdint>
#include <string>
#include <vector>

#include "minecraft/IGameServices.h"
#include "platform/PlatformTypes.h"
#include "platform/fs/fs.h"
#include "platform/renderer/renderer.h"
#include "util/StringHelpers.h"

BufferedImage::BufferedImage(int width, int height, int type) {
    data[0] = new int[width * height];

    for (int i = 1; i < 10; i++) {
        data[i] = nullptr;
    }
    this->width = width;
    this->height = height;
}

void BufferedImage::ByteFlip4(unsigned int& data) {
    data = (data >> 24) | ((data >> 8) & 0x0000ff00) |
           ((data << 8) & 0x00ff0000) | (data << 24);
}
// Loads a bitmap into a buffered image - only currently supports the 2 types of
// 32-bit image that we've made so far and determines which of these is which by
// the compression method. Compression method 3 is a 32-bit image with only
// 24-bits used (ie no alpha channel) whereas method 0 is a full 32-bit image
// with a valid alpha channel.

// Helper to safely convert modern 64x64 skins to 64x32 for legacy Minecraft humanoid models
static void ConvertSkin64To32(int*& pData, int& height) {
    int* px32 = new int[64 * 32];
    // 1. Copy base 64x32 (head, hat, body, right arm, right leg)
    memcpy(px32, pData, 64 * 32 * sizeof(int));

    // 2. Alpha blend the 1.8+ body/limb overlay layers (jacket, sleeves, pants) over base layer
    for (int y = 32; y < 48; y++) {
        for (int x = 0; x < 64; x++) {
            int overlay = pData[y * 64 + x];
            int a = (overlay >> 24) & 0xFF;
            if (a > 10) {
                int baseIdx = (y - 16) * 64 + x;
                int base = px32[baseIdx];
                int ba = (base >> 24) & 0xFF;
                if (a >= 250 || ba == 0) {
                    px32[baseIdx] = overlay;
                } else {
                    float alpha = a / 255.0f;
                    int r = (int)(((overlay >> 16) & 0xFF) * alpha + ((base >> 16) & 0xFF) * (1.0f - alpha));
                    int g = (int)(((overlay >> 8) & 0xFF) * alpha + ((base >> 8) & 0xFF) * (1.0f - alpha));
                    int b = (int)((overlay & 0xFF) * alpha + (base & 0xFF) * (1.0f - alpha));
                    px32[baseIdx] = (0xFF << 24) | (r << 16) | (g << 8) | b;
                }
            }
        }
    }
    delete[] pData;
    pData = px32;
    height = 32;
}

// 4jcraft: mostly rewrote this function
BufferedImage::BufferedImage(const std::string& File, bool filenameHasExtension,
                             bool bTitleUpdateTexture,
                             const std::string& drive) {
    int32_t hr = -1;
    std::string filePath = File;

    for (size_t i = 0; i < filePath.length(); ++i) {
        if (filePath[i] == '\\') filePath[i] = '/';
    }
    for (int l = 0; l < 10; l++) data[l] = nullptr;

    // Check if filePath is an absolute or direct path on disk
    if (filePath.find('/') != std::string::npos && (filePath[0] == '/' || filePath.find(':') != std::string::npos)) {
        std::string directPath = filePath;
        if (!PlatformFilesystem.exists(directPath) && directPath.size() > 4 && directPath.substr(directPath.size() - 4) != ".png") {
            directPath += ".png";
        }
        if (PlatformFilesystem.exists(directPath)) {
            D3DXIMAGE_INFO ImageInfo;
            memset(&ImageInfo, 0, sizeof(D3DXIMAGE_INFO));
            hr = PlatformRenderer.LoadTextureData(directPath.c_str(), &ImageInfo, &data[0]);
            if (hr == 0) {
                width = ImageInfo.Width;
                height = ImageInfo.Height;
                if (width == 64 && height == 64) {
                    ConvertSkin64To32(data[0], height);
                }
                return;
            }
        }
    }

    std::string baseName = filePath;
    if (!filenameHasExtension) {
        if (baseName.size() > 4 &&
            baseName.substr(baseName.size() - 4) == ".png") {
            baseName = baseName.substr(0, baseName.size() - 4);
        }
    }

    while (!baseName.empty() && (baseName[0] == '/' || baseName[0] == '\\'))
        baseName = baseName.substr(1);
    if (baseName.find("res/") == 0) baseName = baseName.substr(4);

    std::string exeDir = PlatformFilesystem.getBasePath().string();

    for (int l = 0; l < 10; l++) {
        std::string mipSuffix =
            (l != 0) ? "MipMapLevel" + toWString<int>(l + 1) : "";
        std::string fileName = baseName + mipSuffix + ".png";
        std::string finalPath;
        bool foundOnDisk = false;

        std::vector<std::string> searchPaths = {
            exeDir + "/Common/res/TitleUpdate/res/" + fileName,
            exeDir + "/Common/res/" + fileName,
            exeDir + "/Common/Media/Graphics/" + fileName,
            exeDir + "/Common/Media/font/" + fileName,
            exeDir + "/Common/res/font/" + fileName,
            exeDir + "/Common/Media/" + fileName};

        if (fileName.find("1_2_2/") == 0) {
            std::string no122 = fileName.substr(6);
            searchPaths.push_back(exeDir + "/Common/res/TitleUpdate/res/" + no122);
            searchPaths.push_back(exeDir + "/Common/res/" + no122);
            searchPaths.push_back(exeDir + "/Common/Media/Graphics/" + no122);
            searchPaths.push_back(exeDir + "/Common/Media/" + no122);
        }

        for (auto& attempt : searchPaths) {
            size_t p;
            while ((p = attempt.find("//")) != std::string::npos)
                attempt.replace(p, 2, "/");
            if (PlatformFilesystem.exists(attempt)) {
                finalPath = attempt;
                foundOnDisk = true;
                break;
            }
        }

        D3DXIMAGE_INFO ImageInfo;
        memset(&ImageInfo, 0, sizeof(D3DXIMAGE_INFO));

        if (foundOnDisk) {
            std::string nativePath = std::filesystem::path(finalPath).string();
            hr = PlatformRenderer.LoadTextureData(nativePath.c_str(),
                                                  &ImageInfo, &data[l]);
        } else {
            std::string archiveKey = "res/" + fileName;
            if (gameServices().hasArchiveFile(archiveKey)) {
                std::vector<uint8_t> ba =
                    gameServices().getArchiveFile(archiveKey);
                hr = PlatformRenderer.LoadTextureData(ba.data(), ba.size(),
                                                      &ImageInfo, &data[l]);
            } else if (fileName.find("1_2_2/") == 0) {
                std::string altKey = "res/" + fileName.substr(6);
                if (gameServices().hasArchiveFile(altKey)) {
                    std::vector<uint8_t> ba =
                        gameServices().getArchiveFile(altKey);
                    hr = PlatformRenderer.LoadTextureData(ba.data(), ba.size(),
                                                          &ImageInfo, &data[l]);
                }
            }
        }

        if (hr == 0) {
            if (l == 0) {
                width = ImageInfo.Width;
                height = ImageInfo.Height;
                if (width == 64 && height == 64 && (fileName.find("mob/") != std::string::npos || fileName.find("skin") != std::string::npos || fileName.find("alex") != std::string::npos || fileName.find("char") != std::string::npos)) {
                    ConvertSkin64To32(data[0], height);
                }
            }
        } else {
            if (l == 0) {
                // safety dummy to prevent crash
                width = 1;
                height = 1;
                data[0] = new int[1];
                data[0][0] = 0xFFFF00FF;
            }
            break;
        }
    }
}
BufferedImage::BufferedImage() {
    for (int l = 0; l < 10; l++) data[l] = nullptr;
    width = 0;
    height = 0;
}

bool BufferedImage::loadMipmapPng(int level, std::uint8_t* bytes,
                                  std::uint32_t numBytes) {
    if (level < 0 || level >= 10 || bytes == nullptr || numBytes == 0) {
        return false;
    }
    D3DXIMAGE_INFO ImageInfo;
    int32_t hr = PlatformRenderer.LoadTextureData(bytes, numBytes, &ImageInfo,
                                                  &data[level]);
    if (hr != 0) {
        return false;
    }
    if (level == 0) {
        width = ImageInfo.Width;
        height = ImageInfo.Height;
    }
    return true;
}

BufferedImage::BufferedImage(std::uint8_t* pbData, std::uint32_t dataBytes) {
    for (int l = 0; l < 10; l++) {
        data[l] = nullptr;
    }

    D3DXIMAGE_INFO ImageInfo;
    memset(&ImageInfo, 0, sizeof(D3DXIMAGE_INFO));
    int32_t hr = PlatformRenderer.LoadTextureData(pbData, dataBytes, &ImageInfo,
                                                  &data[0]);

    if (hr == 0) {
        width = ImageInfo.Width;
        height = ImageInfo.Height;
    } else {
        gameServices().fatalLoadError();
    }
}

BufferedImage::~BufferedImage() {
    for (int i = 0; i < 10; i++) {
        delete[] data[i];
    }
}

int BufferedImage::getWidth() { return width; }

int BufferedImage::getHeight() { return height; }

void BufferedImage::getRGB(int startX, int startY, int w, int h,
                           std::vector<int>& out, int offset, int scansize,
                           int level) {
    int ww = width >> level;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            out[y * scansize + offset + x] =
                data[level][startX + x + ww * (startY + y)];
        }
    }
}

int* BufferedImage::getData() { return data[0]; }

int* BufferedImage::getData(int level) { return data[level]; }

Graphics* BufferedImage::getGraphics() { return nullptr; }

// Returns the transparency. Returns either OPAQUE, BITMASK, or TRANSLUCENT.
// Specified by:
// getTransparency in interface Transparency
// Returns:
// the transparency of this BufferedImage.
int BufferedImage::getTransparency() {
    // TODO - 4J Implement?
    return 0;
}

// Returns a subimage defined by a specified rectangular region. The returned
// BufferedImage shares the same data array as the original image. Parameters:
// x, y - the coordinates of the upper-left corner of the specified rectangular
// region w - the width of the specified rectangular region h - the height of
// the specified rectangular region Returns: a BufferedImage that is the
// subimage of this BufferedImage.
BufferedImage* BufferedImage::getSubimage(int x, int y, int w, int h) {
    // TODO - 4J Implement

    BufferedImage* img = new BufferedImage(w, h, 0);

    // 4jcraft: Copy pixel data directly into img->data[0].
    // The old arrayWithLength.h (custom vector impl) was a non-owning wrapper,
    // std::vector copies so we write to the raw array directly instead.
    int srcW = width;
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            img->data[0][row * w + col] = data[0][(y + row) * srcW + (x + col)];
        }
    }

    int level = 1;
    while (level < 10 && getData(level) != nullptr) {
        int ww = w >> level;
        int hh = h >> level;
        int xx = x >> level;
        int yy = y >> level;
        int srcW = width >> level;
        img->data[level] = new int[ww * hh];
        for (int row = 0; row < hh; row++) {
            for (int col = 0; col < ww; col++) {
                img->data[level][row * ww + col] =
                    data[level][(yy + row) * srcW + (xx + col)];
            }
        }
        ++level;
    }

    return img;
}

void BufferedImage::preMultiplyAlpha() {
    int* curData = data[0];

    int cur = 0;
    int alpha = 0;
    int r = 0;
    int g = 0;
    int b = 0;

    int total = width * height;
    // why was it unsigned??
    for (int i = 0; i < total; ++i) {
        cur = curData[i];
        alpha = (cur >> 24) & 0xff;
        r = ((cur >> 16) & 0xff) * (float)alpha / 255;
        g = ((cur >> 8) & 0xff) * (float)alpha / 255;
        b = (cur & 0xff) * (float)alpha / 255;

        curData[i] = (r << 16) | (g << 8) | (b) | (alpha << 24);
    }
}
