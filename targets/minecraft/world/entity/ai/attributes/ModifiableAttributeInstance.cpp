#include "ModifiableAttributeInstance.h"

#include <assert.h>

#include <utility>

#include "minecraft/world/entity/ai/attributes/Attribute.h"
#include "minecraft/world/entity/ai/attributes/BaseAttributeMap.h"

ModifiableAttributeInstance::ModifiableAttributeInstance(
    BaseAttributeMap* attributeMap, Attribute* attribute) {
    this->attributeMap = attributeMap;
    this->attribute = attribute;

    dirty = true;
    cachedValue = 0.0;

    baseValue = attribute->getDefaultValue();
}

ModifiableAttributeInstance::~ModifiableAttributeInstance() {
    for (int i = 0; i < AttributeModifier::TOTAL_OPERATIONS; i++) {
        for (auto it = modifiers[i].begin(); it != modifiers[i].end(); ++it) {
            // Delete all modifiers
            delete *it;
        }
    }
}

Attribute* ModifiableAttributeInstance::getAttribute() { return attribute; }

double ModifiableAttributeInstance::getBaseValue() { return baseValue; }

void ModifiableAttributeInstance::setBaseValue(double baseValue) {
    if (baseValue == this->getBaseValue()) return;
    this->baseValue = baseValue;
    setDirty();
}

// Returns a pointer to an internally managed vector of modifers by operation
std::unordered_set<AttributeModifier*>*
ModifiableAttributeInstance::getModifiers(int operation) {
    return &modifiers[operation];
}

// Returns a pointer to a new vector of all modifiers
void ModifiableAttributeInstance::getModifiers(
    std::unordered_set<AttributeModifier*>& result) {
    for (int i = 0; i < AttributeModifier::TOTAL_OPERATIONS; i++) {
        std::unordered_set<AttributeModifier*>* opModifiers = &modifiers[i];

        for (auto it = opModifiers->begin(); it != opModifiers->end(); ++it) {
            result.insert(*it);
        }
    }
}

AttributeModifier* ModifiableAttributeInstance::getModifier(eMODIFIER_ID id) {
    AttributeModifier* modifier = nullptr;

    auto it = modifierById.find(id);
    if (it != modifierById.end()) {
        modifier = it->second;
    }

    return modifier;
}

void ModifiableAttributeInstance::addModifiers(
    std::unordered_set<AttributeModifier*>* modifiers) {
    for (auto it = modifiers->begin(); it != modifiers->end(); ++it) {
        addModifier(*it);
    }
}

// Add new modifier to attribute instance (takes ownership of modifier)
void ModifiableAttributeInstance::addModifier(AttributeModifier* modifier) {
    // Can't add modifiers with the same ID (unless the modifier is anonymous)
    if (modifier->getId() != eModifierId_ANONYMOUS &&
        getModifier(modifier->getId()) != nullptr) {
        assert(0);
        // throw new IllegalArgumentException("Modifier is already applied on
        // this attribute!");
        return;
    }

    modifiers[modifier->getOperation()].insert(modifier);
    modifierById[modifier->getId()] = modifier;

    setDirty();
}

void ModifiableAttributeInstance::setDirty() {
    dirty = true;
    attributeMap->onAttributeModified(this);
}

void ModifiableAttributeInstance::removeModifier(AttributeModifier* modifier) {
    for (int i = 0; i < AttributeModifier::TOTAL_OPERATIONS; i++) {
        for (auto it = modifiers[i].begin(); it != modifiers[i].end(); ++it) {
            if (modifier->equals(*it)) {
                modifiers[i].erase(it);
                break;
            }
        }
    }

    modifierById.erase(modifier->getId());

    setDirty();
}

void ModifiableAttributeInstance::removeModifier(eMODIFIER_ID id) {
    AttributeModifier* modifier = getModifier(id);
    if (modifier != nullptr) removeModifier(modifier);
}

void ModifiableAttributeInstance::removeModifiers() {
    std::unordered_set<AttributeModifier*> removingModifiers;
    getModifiers(removingModifiers);

    for (auto it = removingModifiers.begin(); it != removingModifiers.end();
         ++it) {
        removeModifier(*it);
    }
}

double ModifiableAttributeInstance::getValue() {
    if (dirty) {
        cachedValue = calculateValue();
        dirty = false;
    }

    return cachedValue;
}

double ModifiableAttributeInstance::calculateValue() {
    double base = getBaseValue();
    std::unordered_set<AttributeModifier*>* modifiers;

    modifiers = getModifiers(AttributeModifier::OPERATION_ADDITION);
    for (auto it = modifiers->begin(); it != modifiers->end(); ++it) {
        AttributeModifier* modifier = *it;
        base += modifier->getAmount();
    }

    double result = base;

    modifiers = getModifiers(AttributeModifier::OPERATION_MULTIPLY_BASE);
    for (auto it = modifiers->begin(); it != modifiers->end(); ++it) {
        AttributeModifier* modifier = *it;
        result += base * modifier->getAmount();
    }

    modifiers = getModifiers(AttributeModifier::OPERATION_MULTIPLY_TOTAL);
    for (auto it = modifiers->begin(); it != modifiers->end(); ++it) {
        AttributeModifier* modifier = *it;
        result *= 1 + modifier->getAmount();
    }

    return attribute->sanitizeValue(result);
}