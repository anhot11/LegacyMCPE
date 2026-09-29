#pragma once

#include <string>

class Scoreboard;
class ObjectiveCriteria;

class Objective {
public:
    static const int MAX_NAME_LENGTH = 16;
    static const int MAX_DISPLAY_NAME_LENGTH = 32;

private:
    Scoreboard* scoreboard;
    std::string name;
    ObjectiveCriteria* criteria;
    std::string displayName;

public:
    Objective(Scoreboard* scoreboard, const std::string& name,
              ObjectiveCriteria* criteria);

    Scoreboard* getScoreboard();
    std::string getName();
    ObjectiveCriteria* getCriteria();
    std::string getDisplayName();
    void setDisplayName(const std::string& name);
};