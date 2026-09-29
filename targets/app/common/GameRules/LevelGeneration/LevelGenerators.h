#pragma once

#include <format>
#include <string>
#include <vector>
// using namespace std;

class LevelGenerationOptions;

class LevelGenerators {
private:
    std::vector<LevelGenerationOptions*> m_levelGenerators;

public:
    LevelGenerators();

    void addLevelGenerator(const std::string& displayName,
                           LevelGenerationOptions* generator);
    void removeLevelGenerator(LevelGenerationOptions* generator);

    std::vector<LevelGenerationOptions*>* getLevelGenerators() {
        return &m_levelGenerators;
    }
};