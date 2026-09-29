#pragma once

#include <string>

#include "DummyCriteria.h"

class HealthCriteria : public DummyCriteria {
public:
    HealthCriteria(const std::string& id);

    int getScoreModifier(std::vector<std::shared_ptr<Player> >* players);
    bool isReadOnly();
};