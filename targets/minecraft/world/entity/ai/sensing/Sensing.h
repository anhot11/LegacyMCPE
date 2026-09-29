#pragma once

#include <memory>
#include <vector>

class Entity;
class Mob;

class Sensing {
private:
    Mob* mob;
    std::vector<std::weak_ptr<Entity> > seen;
    std::vector<std::weak_ptr<Entity> > unseen;

public:
    Sensing(Mob* mob);

    void tick();
    bool canSee(std::shared_ptr<Entity> target);
};