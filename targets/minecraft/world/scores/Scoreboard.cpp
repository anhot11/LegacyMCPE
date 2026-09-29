#include "Scoreboard.h"

class Player;

Objective* Scoreboard::getObjective(const std::string& name) {
    return nullptr;
    // return objectivesByName.find(name)->second;
}

Objective* Scoreboard::addObjective(const std::string& name,
                                    ObjectiveCriteria* criteria) {
    return nullptr;
    //	Objective *objective = getObjective(name);
    //	if (objective != nullptr)
    //	{
    // #indef _CONTENT_PACKAGE
    //		assert(0);
    // #endif
    //		//throw new IllegalArgumentException("An objective with the name
    //'" + name + "' already exists!");
    //	}
    //
    //	objective = new Objective(this, name, criteria);
    //
    //	vector<Objective *> *criteriaList =
    // objectivesByCriteria.find(criteria)->second;
    //
    //	if (criteriaList == nullptr)
    //	{
    //		criteriaList = new vector<Objective *>();
    //		objectivesByCriteria[criteria] = criteriaList;
    //	}
    //
    //	criteriaList->push_back(objective);
    //	objectivesByName[name] = objective;
    //	onObjectiveAdded(objective);
    //
    //	return objective;
}

std::vector<Objective*>* Scoreboard::findObjectiveFor(
    ObjectiveCriteria* criteria) {
    return nullptr;
    // vector<Objective *> *objectives =
    // objectivesByCriteria.find(criteria)->second;

    // return objectives == nullptr ? new vector<Objective *>() : new
    // vector<Objective *>(objectives);
}

Score* Scoreboard::getPlayerScore(const std::string& name,
                                  Objective* objective) {
    return nullptr;
    // unordered_map<Objective *, Score *> *scores =
    // playerScores.find(name)->it;

    // if (scores == nullptr)
    //{
    //	scores = new unordered_map<Objective *, Score *>();
    //	playerScores.put(name, scores);
    // }

    // Score *score = scores->get(objective);

    // if (score == nullptr)
    //{
    //	score = new Score(this, objective, name);
    //	scores->put(objective, score);
    // }

    // return score;
}

std::vector<Score*>* Scoreboard::getPlayerScores(Objective* objective) {
    return nullptr;
    // vector<Score *> *result = new vector<Score *>();

    // for (Map<Objective, Score> scores : playerScores.values())
    //{
    //	Score score = scores.get(objective);
    //	if (score != null) result.add(score);
    // }

    // Collections.sort(result, Score.SCORE_COMPARATOR);

    // return result;
}

std::vector<Objective*>* Scoreboard::getObjectives() {
    return nullptr;
    // return objectivesByName.values();
}

std::vector<std::string>* Scoreboard::getTrackedPlayers() {
    return nullptr;
    // return playerScores.keySet();
}

void Scoreboard::resetPlayerScore(const std::string& player) {
    // unordered_map<Objective *, Score *> *removed =
    // playerScores.remove(player);

    // if (removed != nullptr)
    //{
    //	onPlayerRemoved(player);
    // }
}

std::vector<Score*>* Scoreboard::getScores() {
    return nullptr;
    // Collection<Map<Objective, Score>> values = playerScores.values();
    // List<Score> result = new ArrayList<Score>();

    // for (Map<Objective, Score> map : values)
    //{
    //	result.addAll(map.values());
    // }

    // return result;
}

std::vector<Score*>* Scoreboard::getScores(Objective* objective) {
    return nullptr;
    // Collection<Map<Objective, Score>> values = playerScores.values();
    // List<Score> result = new ArrayList<Score>();

    // for (Map<Objective, Score> map : values) {
    //	Score score = map.get(objective);
    //	if (score != null) result.add(score);
    // }

    // return result;
}

std::unordered_map<Objective*, Score*>* Scoreboard::getPlayerScores(
    const std::string& player) {
    return nullptr;
    // Map<Objective, Score> result = playerScores.get(player);
    // if (result == null) result = new HashMap<Objective, Score>();
    // return result;
}

void Scoreboard::removeObjective(Objective* objective) {
    // objectivesByName.remove(objective.getName());

    // for (int i = 0; i < DISPLAY_SLOTS; i++) {
    //	if (getDisplayObjective(i) == objective) setDisplayObjective(i, null);
    // }

    // List<Objective> objectives =
    // objectivesByCriteria.get(objective.getCriteria()); if (objectives !=
    // null) objectives.remove(objective);

    // for (Map<Objective, Score> objectiveScoreMap : playerScores.values()) {
    //	objectiveScoreMap.remove(objective);
    // }

    // onObjectiveRemoved(objective);
}

void Scoreboard::setDisplayObjective(int slot, Objective* objective) {
    // displayObjectives[slot] = objective;
}

Objective* Scoreboard::getDisplayObjective(int slot) {
    return nullptr;
    // return displayObjectives[slot];
}

PlayerTeam* Scoreboard::getPlayerTeam(const std::string& name) {
    return nullptr;
    // return teamsByName.get(name);
}

PlayerTeam* Scoreboard::addPlayerTeam(const std::string& name) {
    return nullptr;
    // PlayerTeam team = getPlayerTeam(name);
    // if (team != null) throw new IllegalArgumentException("An objective with
    // the name '" + name + "' already exists!");

    // team = new PlayerTeam(this, name);
    // teamsByName.put(name, team);
    // onTeamAdded(team);

    // return team;
}

void Scoreboard::removePlayerTeam(PlayerTeam* team) {
    // teamsByName.remove(team.getName());

    //// [TODO]: Loop through scores, remove.

    // for (String player : team.getPlayers()) {
    //	teamsByPlayer.remove(player);
    // }

    // onTeamRemoved(team);
}

void Scoreboard::addPlayerToTeam(const std::string& player, PlayerTeam* team) {
    // if (getPlayersTeam(player) != null) {
    //	removePlayerFromTeam(player);
    // }

    // teamsByPlayer.put(player, team);
    // team.getPlayers().add(player);
}

bool Scoreboard::removePlayerFromTeam(const std::string& player) {
    return false;
    // PlayerTeam team = getPlayersTeam(player);

    // if (team != null) {
    //	removePlayerFromTeam(player, team);
    //	return true;
    // } else {
    //	return false;
    // }
}

void Scoreboard::removePlayerFromTeam(const std::string& player,
                                      PlayerTeam* team) {
    // if (getPlayersTeam(player) != team) {
    //	throw new IllegalStateException("Player is either on another team or not
    // on any team. Cannot remove from team '" + team.getName() + "'.");
    // }

    // teamsByPlayer.remove(player);
    // team.getPlayers().remove(player);
}

std::vector<std::string>* Scoreboard::getTeamNames() {
    return nullptr;
    // return teamsByName.keySet();
}

std::vector<PlayerTeam*>* Scoreboard::getPlayerTeams() {
    return nullptr;
    // return teamsByName.values();
}

std::shared_ptr<Player> Scoreboard::getPlayer(const std::string& name) {
    return nullptr;
    // return MinecraftServer.getInstance().getPlayers().getPlayer(name);
}

PlayerTeam* Scoreboard::getPlayersTeam(const std::string& name) {
    return nullptr;
    // return teamsByPlayer.get(name);
}

void Scoreboard::onObjectiveAdded(Objective* objective) {}

void Scoreboard::onObjectiveChanged(Objective* objective) {}

void Scoreboard::onObjectiveRemoved(Objective* objective) {}

void Scoreboard::onScoreChanged(Score* score) {}

void Scoreboard::onPlayerRemoved(const std::string& player) {}

void Scoreboard::onTeamAdded(PlayerTeam* team) {}

void Scoreboard::onTeamChanged(PlayerTeam* team) {}

void Scoreboard::onTeamRemoved(PlayerTeam* team) {}

std::string Scoreboard::getDisplaySlotName(int slot) {
    switch (slot) {
        case DISPLAY_SLOT_LIST:
            return "list";
        case DISPLAY_SLOT_SIDEBAR:
            return "sidebar";
        case DISPLAY_SLOT_BELOW_NAME:
            return "belowName";
        default:
            return "";
    }
}

int Scoreboard::getDisplaySlotByName(const std::string& name) {
    return -1;
    // if (name.equalsIgnoreCase("list"))
    //{
    //	return DISPLAY_SLOT_LIST;
    // }
    // else if (name.equalsIgnoreCase("sidebar"))
    //{
    //	return DISPLAY_SLOT_SIDEBAR;
    // }
    // else if (name.equalsIgnoreCase("belowName"))
    //{
    //	return DISPLAY_SLOT_BELOW_NAME;
    // }
    // else
    //{
    //	return -1;
    // }
}