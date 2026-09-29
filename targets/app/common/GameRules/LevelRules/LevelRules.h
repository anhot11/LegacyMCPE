#pragma once

#include <cstdint>
#include <string>

class LevelRuleset;

class LevelRules {
public:
    LevelRules();

    void addLevelRule(const std::string& displayName, std::uint8_t* pbData,
                      unsigned int dataLength);
    void addLevelRule(const std::string& displayName, LevelRuleset* rootRule);

    void removeLevelRule(LevelRuleset* removing);
};
