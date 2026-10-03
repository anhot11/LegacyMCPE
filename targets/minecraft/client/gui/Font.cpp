#include "Font.h"

#include <string.h>

#include <utility>
#include <vector>

#include "java/Random.h"
#include "minecraft/SharedConstants.h"
#include "minecraft/client/BufferedImage.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/renderer/Tesselator.h"
#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/resources/ResourceLocation.h"
#include "platform/fs/fs.h"
#include "platform/renderer/renderer.h"
#include "platform/stubs.h"
#include "util/StringHelpers.h"

Font::Font(Options* options, const std::string& name, Textures* textures,
           bool enforceUnicode, ResourceLocation* textureLocation, int cols,
           int rows, int charWidth, int charHeight,
           unsigned short charMap[] /* = nullptr */)
    : textures(textures) {
    int charC = cols * rows;  // Number of characters in the font

    charWidths = new int[charC];

    // 4J - added initialisers
    memset(charWidths, 0, charC);

    memset(m_unicodeWidth, 0, sizeof(m_unicodeWidth));
    memset(m_unicodeTexID, 0, sizeof(m_unicodeTexID));
    m_lastBoundUnicodeTex = 0;
    loadUnicodeSizes();

    enforceUnicodeSheet = false;
    bidirectional = false;
    xPos = yPos = 0.0f;

    // Set up member variables
    m_cols = cols;
    m_rows = rows;
    m_charWidth = charWidth;
    m_charHeight = charHeight;
    m_textureLocation = textureLocation;

    // Build character map
    if (charMap != nullptr) {
        for (int i = 0; i < charC; i++) {
            m_charMap.insert(std::make_pair(charMap[i], i));
        }
    }

    random = new Random();

    // Load the image
    BufferedImage* img =
        textures->readImage(textureLocation->getTexture(), name);

    /* - 4J - TODO
    try {
    img = ImageIO.read(Textures.class.getResourceAsStream(name));
} catch (IOException e) {
    throw new RuntimeException(e);
}
    */

    int w = img->getWidth();
    int h = img->getHeight();
    std::vector<int> rawPixels(w * h);
    img->getRGB(0, 0, w, h, rawPixels, 0, w);

    for (int i = 0; i < charC; i++) {
        int xt = i % m_cols;
        int yt = i / m_cols;

        int x = 7;
        for (; x >= 0; x--) {
            int xPixel = xt * 8 + x;
            bool emptyColumn = true;
            for (int y = 0; y < 8 && emptyColumn; y++) {
                int yPixel = (yt * 8 + y) * w;
                bool emptyPixel = (rawPixels[xPixel + yPixel] >> 24) ==
                                  0;  // Check the alpha value
                if (!emptyPixel) emptyColumn = false;
            }
            if (!emptyColumn) {
                break;
            }
        }

        bool isSpace = (charMap != nullptr) ? (charMap[i] == ' ') : (i == ' ');
        if (isSpace) x = 4 - 2;
        charWidths[i] = x + 2;
    }

    delete img;

    // calculate colors
    for (int colorN = 0; colorN < 32; ++colorN) {
        int var10 = (colorN >> 3 & 1) * 85;
        int red = (colorN >> 2 & 1) * 170 + var10;
        int green = (colorN >> 1 & 1) * 170 + var10;
        int blue = (colorN >> 0 & 1) * 170 + var10;

        if (colorN == 6) {
            red += 85;
        }

        if (options->anaglyph3d) {
            int tmpRed = (red * 30 + green * 59 + blue * 11) / 100;
            int tmpGreen = (red * 30 + green * 70) / 100;
            int tmpBlue = (red * 30 + blue * 70) / 100;
            red = tmpRed;
            green = tmpGreen;
            blue = tmpBlue;
        }

        if (colorN >= 16) {
            red /= 4;
            green /= 4;
            blue /= 4;
        }

        colors[colorN] = (red & 255) << 16 | (green & 255) << 8 | (blue & 255);
    }
}

// 4J Stu - This dtor clashes with one in xui! We never delete these anyway so
// take it out for now. Can go back when we have got rid of XUI
Font::~Font() { delete[] charWidths; }

static int nextUtf8Codepoint(const std::string& str, size_t& i) {
    if (i >= str.length()) return 0;
    unsigned char c = (unsigned char)str[i];
    if (c < 0x80) {
        i++;
        return c;
    } else if ((c & 0xE0) == 0xC0) {
        if (i + 1 < str.length()) {
            int cp = ((c & 0x1F) << 6) | ((unsigned char)str[i + 1] & 0x3F);
            i += 2;
            return cp;
        }
    } else if ((c & 0xF0) == 0xE0) {
        if (i + 2 < str.length()) {
            int cp = ((c & 0x0F) << 12) | (((unsigned char)str[i + 1] & 0x3F) << 6) |
                     ((unsigned char)str[i + 2] & 0x3F);
            i += 3;
            return cp;
        }
    } else if ((c & 0xF8) == 0xF0) {
        if (i + 3 < str.length()) {
            int cp = ((c & 0x07) << 18) | (((unsigned char)str[i + 1] & 0x3F) << 12) |
                     (((unsigned char)str[i + 2] & 0x3F) << 6) |
                     ((unsigned char)str[i + 3] & 0x3F);
            i += 4;
            return cp;
        }
    }
    i++;
    return c;
}

void Font::renderCharacter(int c) {
    if (c < 0 || c >= m_cols * m_rows) return;
    float xOff = (c % m_cols) * m_charWidth;
    float yOff = (c / m_cols) * m_charHeight;

    float width = charWidths[c] - .01f;
    float height = m_charHeight - .01f;

    float fontWidth = m_cols * m_charWidth;
    float fontHeight = m_rows * m_charHeight;

    Tesselator* t = Tesselator::getInstance();
    // 4J Stu - Changed to a quad so that we can use within a command buffer
    t->begin();
    t->tex(xOff / fontWidth, (yOff + 7.99f) / fontHeight);
    t->vertex(xPos, yPos + height, 0.0f);

    t->tex((xOff + width) / fontWidth, (yOff + 7.99f) / fontHeight);
    t->vertex(xPos + width, yPos + height, 0.0f);

    t->tex((xOff + width) / fontWidth, yOff / fontHeight);
    t->vertex(xPos + width, yPos, 0.0f);

    t->tex(xOff / fontWidth, yOff / fontHeight);
    t->vertex(xPos, yPos, 0.0f);

    t->end();

    xPos += (float)charWidths[c];
}

void Font::drawShadow(const std::string& str, int x, int y, int color) {
    draw(str, x + 1, y + 1, color, true);
    draw(str, x, y, color, false);
}

void Font::drawShadowWordWrap(const std::string& str, int x, int y, int w,
                              int color, int h) {
    drawWordWrapInternal(str, x + 1, y + 1, w, color, true, h);
    drawWordWrapInternal(str, x, y, w, color, h);
}

void Font::draw(const std::string& str, int x, int y, int color) {
    draw(str, x, y, color, false);
}

std::string Font::reorderBidi(const std::string& str) {
    // 4J Not implemented
    return str;
}

void Font::draw(const std::string& str, bool dropShadow) {
    if (str.empty()) return;

    // Bind the texture
    textures->bindTexture(m_textureLocation);
    m_lastBoundUnicodeTex = 0;

    bool noise = false;
    size_t i = 0;

    while (i < str.length()) {
        unsigned char byte0 = (unsigned char)str[i];

        // Check for § formatting code (supports UTF-8 0xC2 0xA7 or single byte 0xA7)
        bool isUtf8Sec = (i + 2 < str.length() && byte0 == 0xC2u &&
                          (unsigned char)str[i + 1] == 0xA7u);
        bool isSingleSec = (i + 1 < str.length() && byte0 == 0xA7u);

        if (isUtf8Sec || isSingleSec) {
            char ca = isUtf8Sec ? str[i + 2] : str[i + 1];
            int colorN = -1;
            bool isNoise = false;

            if ((ca >= '0') && (ca <= '9'))
                colorN = ca - '0';
            else if ((ca >= 'a') && (ca <= 'f'))
                colorN = (ca - 'a') + 10;
            else if ((ca >= 'A') && (ca <= 'F'))
                colorN = (ca - 'A') + 10;
            else if (ca == 'k' || ca == 'K')
                isNoise = true;
            else if (ca == 'r' || ca == 'R')
                colorN = 15;

            if (isNoise) {
                noise = true;
            } else if (colorN >= 0) {
                noise = false;
                if (colorN > 15) colorN = 15;

                if (dropShadow) colorN += 16;

                int color = colors[colorN];
                glColor3f((color >> 16) / 255.0F, ((color >> 8) & 255) / 255.0F,
                          (color & 255) / 255.0F);
            }

            i += isUtf8Sec ? 3 : 2;
            continue;
        }

        int codepoint = nextUtf8Codepoint(str, i);

        // "noise" for crazy splash screen message
        if (noise) {
            int maxLetters = SharedConstants::acceptableLetters.length();
            if (maxLetters > 0) {
                int newc = random->nextInt(maxLetters);
                codepoint = (unsigned char)SharedConstants::acceptableLetters[newc];
            }
        }

        bool isUnicode = (codepoint >= 256) || (!m_charMap.empty() && m_charMap.find(codepoint) == m_charMap.end());
        if (isUnicode) {
            renderUnicodeCharacter(codepoint, dropShadow);
        } else {
            if (m_lastBoundUnicodeTex != 0) {
                textures->bindTexture(m_textureLocation);
                m_lastBoundUnicodeTex = 0;
            }
            int glyph = MapCharacter(codepoint);
            renderCharacter(glyph);
        }
    }

    if (m_lastBoundUnicodeTex != 0) {
        textures->bindTexture(m_textureLocation);
        m_lastBoundUnicodeTex = 0;
    }
}

void Font::draw(const std::string& str, int x, int y, int color,
                bool dropShadow) {
    if (!str.empty()) {
        if ((color & 0xFC000000) == 0) color |= 0xFF000000;  // force alpha
        // if not set

        if (dropShadow)  // divide RGB by 4, preserve alpha
            color = (color & 0xfcfcfc) >> 2 | (color & (0xFFFFFFFF << 24));

        glColor4f((color >> 16 & 255) / 255.0F, (color >> 8 & 255) / 255.0F,
                  (color & 255) / 255.0F, (color >> 24 & 255) / 255.0F);

        xPos = x;
        yPos = y;
        draw(str, dropShadow);
    }
}

int Font::width(const std::string& str) {
    if (str.empty()) return 0;
    int len = 0;
    size_t i = 0;

    while (i < str.length()) {
        unsigned char byte0 = (unsigned char)str[i];

        // skip § (used for color codes)
        bool isUtf8Sec = (i + 2 < str.length() && byte0 == 0xC2u &&
                          (unsigned char)str[i + 1] == 0xA7u);
        bool isSingleSec = (i + 1 < str.length() && byte0 == 0xA7u);

        if (isUtf8Sec || isSingleSec) {
            i += isUtf8Sec ? 3 : 2;
            continue;
        }

        int codepoint = nextUtf8Codepoint(str, i);
        bool isUnicode = (codepoint >= 256) || (!m_charMap.empty() && m_charMap.find(codepoint) == m_charMap.end());
        if (isUnicode) {
            if (codepoint >= 0 && codepoint < 65536) {
                unsigned char size = m_unicodeWidth[codepoint];
                if (size == 0 && ((codepoint >= 0x2E80 && codepoint <= 0xA4CF) || (codepoint >= 0xAC00 && codepoint <= 0xD7AF) ||
                                  (codepoint >= 0xF900 && codepoint <= 0xFAFF) || (codepoint >= 0xFF00 && codepoint <= 0xFFEE))) {
                    size = 0x0F;
                }
                if (size != 0) {
                    int firstLeft = (size >> 4) & 0x0F;
                    int firstRight = size & 0x0F;
                    if (firstRight < firstLeft) {
                        firstRight = 15;
                        firstLeft = 0;
                    }
                    len += (int)((firstRight - firstLeft) / 2.0f + 1.0f);
                }
            }
        } else {
            int glyph = MapCharacter(codepoint);
            if (glyph >= 0 && glyph < m_cols * m_rows) {
                len += charWidths[glyph];
            }
        }
    }

    return len;
}

std::string Font::sanitize(const std::string& str) {
    return str;
}

int Font::MapCharacter(int codepoint) {
    if (!m_charMap.empty()) {
        auto it = m_charMap.find(codepoint);
        if (it != m_charMap.end()) {
            return it->second;
        }
        return 0;
    } else {
        return (codepoint >= 0 && codepoint < m_cols * m_rows) ? codepoint : 0;
    }
}

bool Font::CharacterExists(int codepoint) {
    if (codepoint >= 256) {
        if (codepoint < 65536) {
            return (m_unicodeWidth[codepoint] != 0) ||
                   (codepoint >= 0x2E80 && codepoint <= 0xA4CF) ||
                   (codepoint >= 0xAC00 && codepoint <= 0xD7AF) ||
                   (codepoint >= 0xF900 && codepoint <= 0xFAFF) ||
                   (codepoint >= 0xFF00 && codepoint <= 0xFFEE);
        }
        return false;
    }
    if (!m_charMap.empty()) {
        return m_charMap.find(codepoint) != m_charMap.end();
    } else {
        return codepoint >= 0 && codepoint < m_rows * m_cols;
    }
}

void Font::drawWordWrap(const std::string& string, int x, int y, int w, int col,
                        int h) {
    // if (bidirectional)
    //{
    //	string = reorderBidi(string);
    // }
    drawWordWrapInternal(string, x, y, w, col, h);
}

void Font::drawWordWrapInternal(const std::string& string, int x, int y, int w,
                                int col, int h) {
    drawWordWrapInternal(string, x, y, w, col, false, h);
}

void Font::drawWordWrap(const std::string& string, int x, int y, int w, int col,
                        bool darken, int h) {
    // if (bidirectional)
    //{
    //	string = reorderBidi(string);
    // }
    drawWordWrapInternal(string, x, y, w, col, darken, h);
}

void Font::drawWordWrapInternal(const std::string& string, int x, int y, int w,
                                int col, bool darken, int h) {
    std::vector<std::string> lines = stringSplit(string, '\n');
    if (lines.size() > 1) {
        auto itEnd = lines.end();
        for (auto it = lines.begin(); it != itEnd; it++) {
            // 4J Stu - Don't draw text that will be partially cutoff/overlap
            // something it shouldn't
            if ((y + this->wordWrapHeight(*it, w)) > h) break;
            drawWordWrapInternal(*it, x, y, w, col, h);
            y += this->wordWrapHeight(*it, w);
        }
        return;
    }
    std::vector<std::string> words = stringSplit(string, ' ');
    unsigned int pos = 0;
    while (pos < words.size()) {
        std::string line = words[pos++] + " ";
        while (pos < words.size() && width(line + words[pos]) < w) {
            line += words[pos++] + " ";
        }
        while (width(line) > w) {
            int l = 0;
            while (width(line.substr(0, l + 1)) <= w) {
                l++;
            }
            if (trimString(line.substr(0, l)).length() > 0) {
                draw(line.substr(0, l), x, y, col);
                y += 8;
            }
            line = line.substr(l);

            // 4J Stu - Don't draw text that will be partially cutoff/overlap
            // something it shouldn't
            if ((y + 8) > h) break;
        }
        // 4J Stu - Don't draw text that will be partially cutoff/overlap
        // something it shouldn't
        if (trimString(line).length() > 0 && !((y + 8) > h)) {
            draw(line, x, y, col);
            y += 8;
        }
    }
}

int Font::wordWrapHeight(const std::string& string, int w) {
    std::vector<std::string> lines = stringSplit(string, '\n');
    if (lines.size() > 1) {
        int h = 0;
        auto itEnd = lines.end();
        for (auto it = lines.begin(); it != itEnd; it++) {
            h += this->wordWrapHeight(*it, w);
        }
        return h;
    }
    std::vector<std::string> words = stringSplit(string, ' ');
    unsigned int pos = 0;
    int y = 0;
    while (pos < words.size()) {
        std::string line = words[pos++] + " ";
        while (pos < words.size() && width(line + words[pos]) < w) {
            line += words[pos++] + " ";
        }
        while (width(line) > w) {
            int l = 0;
            while (width(line.substr(0, l + 1)) <= w) {
                l++;
            }
            if (trimString(line.substr(0, l)).length() > 0) {
                y += 8;
            }
            line = line.substr(l);
        }
        if (trimString(line).length() > 0) {
            y += 8;
        }
    }
    if (y < 8) y += 8;
    return y;
}

void Font::setEnforceUnicodeSheet(bool enforceUnicodeSheet) {
    this->enforceUnicodeSheet = enforceUnicodeSheet;
}

void Font::setBidirectional(bool bidirectional) {
    this->bidirectional = bidirectional;
}

bool Font::AllCharactersValid(const std::string& str) {
    return true;
}

void Font::renderFakeCB(IntBuffer* cb) {
    // 4J - legacy XUI command buffer stub
}

void Font::loadUnicodeSizes() {
    std::filesystem::path basePath = PlatformFilesystem.getBasePath();
    std::filesystem::path candidates[] = {
        basePath / "Common" / "res" / "1_2_2" / "font" / "glyph_sizes.bin",
        basePath / "res" / "1_2_2" / "font" / "glyph_sizes.bin",
        basePath / "1_2_2" / "font" / "glyph_sizes.bin",
        "/sdcard/LegacyMCPE/Common/res/1_2_2/font/glyph_sizes.bin",
        "/storage/emulated/0/LegacyMCPE/Common/res/1_2_2/font/glyph_sizes.bin"
    };

    for (const auto& path : candidates) {
        if (PlatformFilesystem.exists(path)) {
            auto result = PlatformFilesystem.readFile(path, m_unicodeWidth, sizeof(m_unicodeWidth));
            if (result.status == IPlatformFilesystem::ReadStatus::Ok && result.bytesRead >= 65536) {
                break;
            }
        }
    }

    // Default / fallback widths for CJK codepoints
    for (int cp = 0x2E80; cp <= 0xA4CF; cp++) {
        if (m_unicodeWidth[cp] == 0) m_unicodeWidth[cp] = 0x0F;
    }
    for (int cp = 0xAC00; cp <= 0xD7AF; cp++) {
        if (m_unicodeWidth[cp] == 0) m_unicodeWidth[cp] = 0x0F;
    }
    for (int cp = 0xF900; cp <= 0xFAFF; cp++) {
        if (m_unicodeWidth[cp] == 0) m_unicodeWidth[cp] = 0x0F;
    }
    for (int cp = 0xFF00; cp <= 0xFFEE; cp++) {
        if (m_unicodeWidth[cp] == 0) m_unicodeWidth[cp] = 0x0F;
    }
}

void Font::loadUnicodePage(int page) {
    if (page < 0 || page >= 256) return;
    char fileName[64];
    snprintf(fileName, sizeof(fileName), "1_2_2/font/glyph_%02X.png", page);
    int texId = textures->loadTexture(TN_COUNT, fileName);
    if (texId <= 0) {
        snprintf(fileName, sizeof(fileName), "font/glyph_%02X.png", page);
        texId = textures->loadTexture(TN_COUNT, fileName);
    }
    m_unicodeTexID[page] = texId;
}

void Font::renderUnicodeCharacter(int c, bool dropShadow) {
    if (c < 0 || c >= 65536) return;
    unsigned char size = m_unicodeWidth[c];
    if (size == 0) {
        if ((c >= 0x2E80 && c <= 0xA4CF) || (c >= 0xAC00 && c <= 0xD7AF) ||
            (c >= 0xF900 && c <= 0xFAFF) || (c >= 0xFF00 && c <= 0xFFEE)) {
            size = 0x0F;
        } else {
            return;
        }
    }

    int page = (c >> 8) & 0xFF;
    if (m_unicodeTexID[page] == 0) {
        loadUnicodePage(page);
    }
    if (m_unicodeTexID[page] <= 0) return;

    if (m_lastBoundUnicodeTex != m_unicodeTexID[page]) {
        glBindTexture(GL_TEXTURE_2D, m_unicodeTexID[page]);
        m_lastBoundUnicodeTex = m_unicodeTexID[page];
    }

    int firstLeft = (size >> 4) & 0x0F;
    int firstRight = size & 0x0F;
    if (firstRight < firstLeft) {
        firstRight = 15;
        firstLeft = 0;
    }

    float left = (float)firstLeft;
    float right = (float)(firstRight + 1);

    float xOff = (float)((c % 16) * 16) + left;
    float yOff = (float)(((c & 0xFF) / 16) * 16);
    float width = right - left - 0.02f;
    float shadowOffset = dropShadow ? 1.0f : 0.0f;

    Tesselator* t = Tesselator::getInstance();
    t->begin(GL_TRIANGLE_STRIP);
    t->tex(xOff / 256.0f, yOff / 256.0f);
    t->vertex(xPos + shadowOffset, yPos + shadowOffset, 0.0f);

    t->tex(xOff / 256.0f, (yOff + 15.98f) / 256.0f);
    t->vertex(xPos + shadowOffset, yPos + 7.99f + shadowOffset, 0.0f);

    t->tex((xOff + width) / 256.0f, yOff / 256.0f);
    t->vertex(xPos + width / 2.0f + shadowOffset, yPos + shadowOffset, 0.0f);

    t->tex((xOff + width) / 256.0f, (yOff + 15.98f) / 256.0f);
    t->vertex(xPos + width / 2.0f + shadowOffset, yPos + 7.99f + shadowOffset, 0.0f);
    t->end();

    xPos += (right - left) / 2.0f + 1.0f;
}
