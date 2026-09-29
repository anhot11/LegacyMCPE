#pragma once

#include <string>
#include <unordered_map>

class GameRules {
private:
    class GameRule {
    private:
        std::string value;
        bool booleanValue;
        int intValue;
        double doubleValue;

    public:
        GameRule(const std::string& startValue);

        void set(const std::string& newValue);
        std::string get();
        bool getBoolean();
        int getInt();
        double getDouble();
    };

public:
    // 4J: Originally strings
    // default rules
    static const int RULE_DOFIRETICK;
    static const int RULE_MOBGRIEFING;
    static const int RULE_KEEPINVENTORY;
    static const int RULE_DOMOBSPAWNING;
    static const int RULE_DOMOBLOOT;
    static const int RULE_DOTILEDROPS;
    static const int RULE_COMMANDBLOCKOUTPUT;
    static const int RULE_NATURAL_REGENERATION;
    static const int RULE_DAYLIGHT;

private:
    std::unordered_map<std::string, GameRule*> rules;

public:
    GameRules();
    ~GameRules();

    bool getBoolean(const int rule);

    // 4J: Removed unused functions
    /*void set(const std::string &ruleName, const std::string &newValue);
    void registerRule(const std::string &name, const std::string &startValue);
    std::string get(const std::string &ruleName);
    int getInt(const std::string &ruleName);
    double getDouble(const std::string &ruleName);
    CompoundTag *createTag();
    void loadFromTag(CompoundTag *tag);
    vector<std::string> *getRuleNames();
    bool contains(const std::string &rule);*/
};