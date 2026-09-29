#pragma once

#include <string>

#include "minecraft/world/entity/ai/attributes/Attribute.h"
/*
4J - Both modifier uuid and name have been replaced by an id enum. Note that we
have special value "eModifierId_ANONYMOUS" for attribute modifiers that
previously didn't have a fixed UUID and are never removed.

To all intents and purposes anonymous modifiers don't have an ID and so are
handled differently in some cases, for instance:
        1. You can have multiple modifiers with the anonymous ID on a single
attribute instance
        2. Anonymous modifiers can't be removed from attribute instance by ID

IMPORTANT: Saved out to file so don't change order. All new values should be
added at the end.
*/

class HtmlString;

enum eMODIFIER_ID {
    eModifierId_ANONYMOUS = 0,

    eModifierId_ITEM_BASEDAMAGE,

    eModifierId_MOB_FLEEING,
    eModifierId_MOB_SPRINTING,

    eModifierId_MOB_ENDERMAN_ATTACKSPEED,
    eModifierId_MOB_PIG_ATTACKSPEED,
    eModifierId_MOB_WITCH_DRINKSPEED,
    eModifierId_MOB_ZOMBIE_BABYSPEED,

    eModifierId_POTION_DAMAGEBOOST,
    eModifierId_POTION_HEALTHBOOST,
    eModifierId_POTION_MOVESPEED,
    eModifierId_POTION_MOVESLOWDOWN,
    eModifierId_POTION_WEAKNESS,

    eModifierId_COUNT,
};

class AttributeModifier {
public:
    static const int OPERATION_ADDITION = 0;
    static const int OPERATION_MULTIPLY_BASE = 1;
    static const int OPERATION_MULTIPLY_TOTAL = 2;
    static const int TOTAL_OPERATIONS = 3;

private:
    double amount;
    int operation;
    std::string name;
    eMODIFIER_ID id;
    bool serialize;

    void _init(eMODIFIER_ID id, const std::string name, double amount,
               int operation);

public:
    AttributeModifier(double amount, int operation);
    AttributeModifier(eMODIFIER_ID id, double amount, int operation);

    eMODIFIER_ID getId();
    std::string getName();
    int getOperation();
    double getAmount();
    bool isSerializable();
    AttributeModifier* setSerialize(bool serialize);
    bool equals(AttributeModifier* modifier);
    std::string toString();
    HtmlString getHoverText(
        eATTRIBUTE_ID attribute);  // 4J: Added to keep modifier readable
                                   // std::string creation in one place
};