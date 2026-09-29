#include "DummyCriteria.h"

#include <unordered_map>

#include "minecraft/world/scores/criteria/ObjectiveCriteria.h"

DummyCriteria::DummyCriteria(const std::string& name) {
    this->name = name;
    ObjectiveCriteria::CRITERIA_BY_NAME[name] = this;
}

std::string DummyCriteria::getName() { return name; }

int DummyCriteria::getScoreModifier(
    std::vector<std::shared_ptr<Player> >* players) {
    return 0;
}

bool DummyCriteria::isReadOnly() { return false; }