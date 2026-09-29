#pragma once

#include <format>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "minecraft/world/Icon.h"

class Texture;
class BufferedReader;

class StitchedTexture : public Icon {
private:
    const std::string name;

public:
    std::string m_fileName;

protected:
    Texture* source;
    std::vector<Texture*>* frames;

private:
    typedef std::vector<std::pair<int, int> > intPairVector;
    intPairVector* frameOverride;
    int flags;

protected:
    bool rotated;

    int x;
    int y;

protected:
    int width;
    int height;

    float u0;
    float u1;
    float v0;
    float v1;

    float widthTranslation;
    float heightTranslation;

protected:
    int frame;
    int subFrame;

public:
    static StitchedTexture* create(const std::string& name);

    ~StitchedTexture();

protected:
    StitchedTexture(const std::string& name, const std::string& filename);

public:
    void initUVs(float U0, float V0, float U1, float V1);
    void init(Texture* source, std::vector<Texture*>* frames, int x, int y,
              int width, int height, bool rotated);
    void replaceWith(StitchedTexture* texture);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    float getU0(bool adjust = false) const;
    float getU1(bool adjust = false) const;
    float getU(double offset, bool adjust = false) const;
    float getV0(bool adjust = false) const;
    float getV1(bool adjust = false) const;
    float getV(double offset, bool adjust = false) const;
    std::string getName() const;
    virtual int getSourceWidth() const;
    virtual int getSourceHeight() const;
    virtual void cycleFrames();
    Texture* getSource();
    Texture* getFrame(int i);
    virtual int getFrames();

    /**
     * Loads animation frames from a file with the syntax, <code>
     * 0,1,2,3,
     * 4*10,5*10,
     * 4*10,3,2,1,
     * 0
     * </code> or similar
     *
     * @param bufferedReader
     */
    void loadAnimationFrames(BufferedReader* bufferedReader);
    void loadAnimationFrames(const std::string& string);  // 4J Added

    int getFlags() const;              // 4J added
    void setFlags(int flags);          // 4J added
    virtual void freeFrameTextures();  // 4J added
    virtual bool hasOwnData();         // 4J Added
};