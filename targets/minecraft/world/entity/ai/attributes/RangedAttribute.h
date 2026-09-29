#pragma once

#include "BaseAttribute.h"
#include "minecraft/world/entity/ai/attributes/Attribute.h"

class RangedAttribute : public BaseAttribute {
private:
    double minValue;
    double maxValue;

public:
    RangedAttribute(eATTRIBUTE_ID id, double defaultValue, double minValue,
                    double maxValue);

    double getMinValue();
    double getMaxValue();
    double sanitizeValue(double value);

    // 4J: Removed legacy name
    // RangedAttribute *importLegacyName(const std::string &name);
    // std::string getImportLegacyName();
};