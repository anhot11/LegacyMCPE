#pragma once

#include <unordered_map>
#include <unordered_set>

#include "AttributeInstance.h"
#include "minecraft/world/entity/ai/attributes/AttributeModifier.h"

class Attribute;
class BaseAttributeMap;

class ModifiableAttributeInstance : public AttributeInstance {
private:
    BaseAttributeMap* attributeMap;
    Attribute* attribute;
    std::unordered_set<AttributeModifier*>
        modifiers[AttributeModifier::TOTAL_OPERATIONS];
    std::unordered_map<unsigned int, AttributeModifier*> modifierById;
    double baseValue;
    bool dirty;
    double cachedValue;

public:
    ModifiableAttributeInstance(BaseAttributeMap* attributeMap,
                                Attribute* attribute);
    ~ModifiableAttributeInstance();

    Attribute* getAttribute();
    double getBaseValue();
    void setBaseValue(double baseValue);
    std::unordered_set<AttributeModifier*>* getModifiers(int operation);
    void getModifiers(std::unordered_set<AttributeModifier*>& result);
    AttributeModifier* getModifier(eMODIFIER_ID id);
    void addModifiers(std::unordered_set<AttributeModifier*>* modifiers);
    void addModifier(AttributeModifier* modifier);

private:
    void setDirty();

public:
    void removeModifier(AttributeModifier* modifier);
    void removeModifier(eMODIFIER_ID id);
    void removeModifiers();
    double getValue();

private:
    double calculateValue();
};