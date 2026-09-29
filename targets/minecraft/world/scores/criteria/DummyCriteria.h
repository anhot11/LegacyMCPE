#pragma once

#include <string>

#include "ObjectiveCriteria.h"

class DummyCriteria : public ObjectiveCriteria {
private:
    std::string name;

public:
    DummyCriteria(const std::string& name);

    virtual std::string getName();
    virtual int getScoreModifier(
        std::vector<std::shared_ptr<Player> >* players);
    virtual bool isReadOnly();
};