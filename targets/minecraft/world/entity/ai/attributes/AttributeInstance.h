#pragma once
#include <unordered_set>

#include "AttributeModifier.h"

class AttributeInstance {
public:
    virtual ~AttributeInstance() {}

    virtual Attribute* getAttribute() = 0;
    virtual double getBaseValue() = 0;
    virtual void setBaseValue(double baseValue) = 0;
    virtual double getValue() = 0;

    virtual std::unordered_set<AttributeModifier*>* getModifiers(
        int operation) = 0;
    virtual void getModifiers(
        std::unordered_set<AttributeModifier*>& result) = 0;
    virtual AttributeModifier* getModifier(eMODIFIER_ID id) = 0;
    virtual void addModifiers(
        std::unordered_set<AttributeModifier*>* modifiers) = 0;
    virtual void addModifier(AttributeModifier* modifier) = 0;
    virtual void removeModifier(AttributeModifier* modifier) = 0;
    virtual void removeModifier(eMODIFIER_ID id) = 0;
    virtual void removeModifiers() = 0;
};