#include "GeneralStat.h"

#include <vector>

#include "minecraft/stats/Stat.h"
#include "minecraft/stats/Stats.h"

class StatFormatter;

GeneralStat::GeneralStat(int id, const std::string& name,
                         StatFormatter* formatter)
    : Stat(id, name, formatter) {}

GeneralStat::GeneralStat(int id, const std::string& name) : Stat(id, name) {}

Stat* GeneralStat::postConstruct() {
    Stat::postConstruct();
    Stats::generalStats->push_back(this);
    return this;
}