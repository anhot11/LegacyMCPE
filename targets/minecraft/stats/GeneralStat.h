#pragma once

#include <string>

#include "Stat.h"

class StatFormatter;

class GeneralStat : public Stat {
public:
    GeneralStat(int id, const std::string& name, StatFormatter* formatter);
    GeneralStat(int id, const std::string& name);
    Stat* postConstruct();
};
