#pragma once

#include <string>

#include "minecraft/world/level/saveddata/SavedData.h"

class CompoundTag;

class StructureFeatureSavedData : public SavedData {
private:
    static std::string TAG_FEATURES;
    CompoundTag* pieceTags;

public:
    StructureFeatureSavedData(const std::string& idName);
    ~StructureFeatureSavedData();

    void load(CompoundTag* tag);
    void save(CompoundTag* tag);
    CompoundTag* getFeatureTag(int chunkX, int chunkZ);
    void putFeatureTag(CompoundTag* tag, int chunkX, int chunkZ);
    std::string createFeatureTagId(int chunkX, int chunkZ);
    CompoundTag* getFullTag();
};