#pragma once

#include <unordered_set>

#include "BaseAttributeMap.h"
#include "minecraft/world/entity/ai/attributes/Attribute.h"

class AttributeInstance;

class ServersideAttributeMap : public BaseAttributeMap {
private:
    std::unordered_set<AttributeInstance*> dirtyAttributes;

protected:
    // 4J: Remove legacy name
    // unordered_map<eATTRIBUTE_ID, AttributeInstance *> attributesByLegacy;

public:
    // 4J-JEV: Changed from ModifiableAttributeInstance to AttributeInstance as
    // they are not 'covariant' on PS4.
    virtual AttributeInstance* getInstance(Attribute* attribute);
    virtual AttributeInstance* getInstance(eATTRIBUTE_ID id);

    virtual AttributeInstance* registerAttribute(Attribute* attribute);
    virtual void onAttributeModified(
        ModifiableAttributeInstance* attributeInstance);
    virtual std::unordered_set<AttributeInstance*>* getDirtyAttributes();
    virtual std::unordered_set<AttributeInstance*>* getSyncableAttributes();
};