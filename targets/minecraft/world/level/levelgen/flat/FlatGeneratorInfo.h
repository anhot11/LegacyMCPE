#pragma once

#include <format>
#include <string>
#include <unordered_map>
#include <vector>

class FlatLayerInfo;

class FlatGeneratorInfo {
public:
    static const int SERIALIZATION_VERSION = 2;
    static const std::string STRUCTURE_VILLAGE;
    static const std::string STRUCTURE_BIOME_SPECIFIC;
    static const std::string STRUCTURE_STRONGHOLD;
    static const std::string STRUCTURE_MINESHAFT;
    static const std::string STRUCTURE_BIOME_DECORATION;
    static const std::string STRUCTURE_LAKE;
    static const std::string STRUCTURE_LAVA_LAKE;
    static const std::string STRUCTURE_DUNGEON;

private:
    std::vector<FlatLayerInfo*> layers;
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string> >
        structures;
    int biome;

public:
    FlatGeneratorInfo();
    ~FlatGeneratorInfo();

    int getBiome();
    void setBiome(int biome);
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string> >*
    getStructures();
    std::vector<FlatLayerInfo*>* getLayers();
    void updateLayers();
    std::string toString();

private:
    static FlatLayerInfo* getLayerFromString(const std::string& input,
                                             int yOffset);
    static std::vector<FlatLayerInfo*>* getLayersFromString(
        const std::string& input);

public:
    static FlatGeneratorInfo* fromValue(const std::string& input);
    static FlatGeneratorInfo* getDefault();
};