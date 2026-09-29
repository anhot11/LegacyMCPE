#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "minecraft/world/scores/Scoreboard.h"

class MinecraftServer;
class ScoreboardSaveData;
class Score;
class Objective;
class PlayerTeam;
class Packet;

class ServerScoreboard : public Scoreboard {
private:
    MinecraftServer* server;
    std::unordered_set<Objective*> trackedObjectives;
    ScoreboardSaveData* saveData;

public:
    ServerScoreboard(MinecraftServer* server);

    MinecraftServer* getServer();
    void onScoreChanged(Score* score);
    void onPlayerRemoved(const std::string& player);
    void setDisplayObjective(int slot, Objective* objective);
    void addPlayerToTeam(const std::string& player, PlayerTeam* team);
    void removePlayerFromTeam(const std::string& player, PlayerTeam* team);
    void onObjectiveAdded(Objective* objective);
    void onObjectiveChanged(Objective* objective);
    void onObjectiveRemoved(Objective* objective);
    void onTeamAdded(PlayerTeam* team);
    void onTeamChanged(PlayerTeam* team);
    void onTeamRemoved(PlayerTeam* team);
    void setSaveData(ScoreboardSaveData* data);

protected:
    void setDirty();

public:
    std::vector<std::shared_ptr<Packet> >* getStartTrackingPackets(
        Objective* objective);
    void startTrackingObjective(Objective* objective);
    std::vector<std::shared_ptr<Packet> >* getStopTrackingPackets(
        Objective* objective);
    void stopTrackingObjective(Objective* objective);
    int getObjectiveDisplaySlotCount(Objective* objective);
};