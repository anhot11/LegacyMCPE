#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "minecraft/world/item/ItemInstance.h"

class Objective;
class ObjectiveCriteria;
class PlayerTeam;
class Score;

class Scoreboard {
public:
    static const int DISPLAY_SLOT_LIST = 0;
    static const int DISPLAY_SLOT_SIDEBAR = 1;
    static const int DISPLAY_SLOT_BELOW_NAME = 2;
    static const int DISPLAY_SLOTS = 3;

private:
    std::unordered_map<std::string, Objective*> objectivesByName;
    std::unordered_map<ObjectiveCriteria*, std::vector<Objective*>*>
        objectivesByCriteria;
    std::unordered_map<std::string, std::unordered_map<Objective*, Score*> >
        playerScores;
    Objective* displayObjectives[DISPLAY_SLOTS];
    std::unordered_map<std::string, PlayerTeam*> teamsByName;
    std::unordered_map<std::string, PlayerTeam*> teamsByPlayer;

public:
    Objective* getObjective(const std::string& name);
    Objective* addObjective(const std::string& name,
                            ObjectiveCriteria* criteria);
    std::vector<Objective*>* findObjectiveFor(ObjectiveCriteria* criteria);
    Score* getPlayerScore(const std::string& name, Objective* objective);
    std::vector<Score*>* getPlayerScores(Objective* objective);
    std::vector<Objective*>* getObjectives();
    std::vector<std::string>* getTrackedPlayers();
    void resetPlayerScore(const std::string& player);
    std::vector<Score*>* getScores();
    std::vector<Score*>* getScores(Objective* objective);
    std::unordered_map<Objective*, Score*>* getPlayerScores(
        const std::string& player);
    void removeObjective(Objective* objective);
    void setDisplayObjective(int slot, Objective* objective);
    Objective* getDisplayObjective(int slot);
    PlayerTeam* getPlayerTeam(const std::string& name);
    PlayerTeam* addPlayerTeam(const std::string& name);
    void removePlayerTeam(PlayerTeam* team);
    void addPlayerToTeam(const std::string& player, PlayerTeam* team);
    bool removePlayerFromTeam(const std::string& player);
    void removePlayerFromTeam(const std::string& player, PlayerTeam* team);
    std::vector<std::string>* getTeamNames();
    std::vector<PlayerTeam*>* getPlayerTeams();
    std::shared_ptr<Player> getPlayer(const std::string& name);
    PlayerTeam* getPlayersTeam(const std::string& name);
    void onObjectiveAdded(Objective* objective);
    void onObjectiveChanged(Objective* objective);
    void onObjectiveRemoved(Objective* objective);
    void onScoreChanged(Score* score);
    void onPlayerRemoved(const std::string& player);
    void onTeamAdded(PlayerTeam* team);
    void onTeamChanged(PlayerTeam* team);
    void onTeamRemoved(PlayerTeam* team);
    static std::string getDisplaySlotName(int slot);
    static int getDisplaySlotByName(const std::string& name);
};