#pragma once

#include <string>
#include <unordered_set>

#include "Team.h"

class Scoreboard;

class PlayerTeam : public Team {
public:
    static const int MAX_NAME_LENGTH = 16;
    static const int MAX_DISPLAY_NAME_LENGTH = 32;
    static const int MAX_PREFIX_LENGTH = 16;
    static const int MAX_SUFFIX_LENGTH = 16;

private:
    static const int BIT_FRIENDLY_FIRE = 0;
    static const int BIT_SEE_INVISIBLES = 1;

    Scoreboard* scoreboard;
    std::string name;
    std::unordered_set<std::string> players;
    std::string displayName;
    std::string prefix;
    std::string suffix;
    bool allowFriendlyFire;
    bool seeFriendlyInvisibles;

public:
    PlayerTeam(Scoreboard* scoreboard, const std::string& name);

    Scoreboard* getScoreboard();
    std::string getName();
    std::string getDisplayName();
    void setDisplayName(const std::string& displayName);
    std::unordered_set<std::string>* getPlayers();
    std::string getPrefix();
    void setPrefix(const std::string& prefix);
    std::string getSuffix();
    void setSuffix(const std::string& suffix);
    std::string getFormattedName(const std::string& teamMemberName);
    static std::string formatNameForTeam(PlayerTeam* team);
    static std::string formatNameForTeam(Team* team, const std::string& name);
    bool isAllowFriendlyFire();
    void setAllowFriendlyFire(bool allowFriendlyFire);
    bool canSeeFriendlyInvisibles();
    void setSeeFriendlyInvisibles(bool seeFriendlyInvisibles);
    int packOptions();
    void unpackOptions(int options);
};