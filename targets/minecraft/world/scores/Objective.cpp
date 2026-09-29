#include "Objective.h"

#include "minecraft/world/scores/Scoreboard.h"

Objective::Objective(Scoreboard* scoreboard, const std::string& name,
                     ObjectiveCriteria* criteria) {
    this->scoreboard = scoreboard;
    this->name = name;
    this->criteria = criteria;

    displayName = name;
}

Scoreboard* Objective::getScoreboard() { return scoreboard; }

std::string Objective::getName() { return name; }

ObjectiveCriteria* Objective::getCriteria() { return criteria; }

std::string Objective::getDisplayName() { return displayName; }

void Objective::setDisplayName(const std::string& name) {
    displayName = name;
    scoreboard->onObjectiveChanged(this);
}