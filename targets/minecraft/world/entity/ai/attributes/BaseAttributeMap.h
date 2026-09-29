#pragma once

#include <format>
#include <memory>
#include <unordered_map>
#include <vector>

#include "minecraft/world/entity/ai/attributes/Attribute.h"

class ModifiableAttributeInstance;
class AttributeInstance;
class ItemInstance;

class BaseAttributeMap {
protected:
    // unordered_map<Attribute *, AttributeInstance *> attributesByObject;
    std::unordered_map<eATTRIBUTE_ID, AttributeInstance*> attributesById;

public:
    virtual ~BaseAttributeMap();

    virtual AttributeInstance* getInstance(Attribute* attribute);
    virtual AttributeInstance* getInstance(eATTRIBUTE_ID name);

    virtual AttributeInstance* registerAttribute(Attribute* attribute) = 0;

    virtual void getAttributes(std::vector<AttributeInstance*>& atts);
    virtual void onAttributeModified(
        ModifiableAttributeInstance* attributeInstance);

    // 4J: Changed these into specialised functions for adding/removing the
    // modifiers of an item (it's cleaner)
    virtual void removeItemModifiers(std::shared_ptr<ItemInstance> item);
    virtual void addItemModifiers(std::shared_ptr<ItemInstance> item);
};