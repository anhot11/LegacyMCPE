#pragma once

#include <memory>

class Entity;
class ItemInstance;

class EntitySelector {
public:
    static const EntitySelector* ENTITY_STILL_ALIVE;
    static const EntitySelector* CONTAINER_ENTITY_SELECTOR;

    virtual ~EntitySelector() = default;
    virtual bool matches(std::shared_ptr<Entity> entity) const = 0;
};

class AliveEntitySelector : public EntitySelector {
public:
    bool matches(std::shared_ptr<Entity> entity) const;
};

class ContainerEntitySelector : public EntitySelector {
public:
    bool matches(std::shared_ptr<Entity> entity) const;
};

class MobCanWearArmourEntitySelector : public EntitySelector {
private:
    std::shared_ptr<ItemInstance> item;

public:
    MobCanWearArmourEntitySelector(std::shared_ptr<ItemInstance> item);
    bool matches(std::shared_ptr<Entity> entity) const;
};
