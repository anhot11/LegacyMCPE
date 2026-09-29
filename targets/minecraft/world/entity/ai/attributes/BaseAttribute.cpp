#include "BaseAttribute.h"

#include "minecraft/world/entity/ai/attributes/Attribute.h"

BaseAttribute::BaseAttribute(eATTRIBUTE_ID id, double defaultValue) {
    this->id = id;
    this->defaultValue = defaultValue;
    syncable = false;
}

eATTRIBUTE_ID BaseAttribute::getId() { return id; }

double BaseAttribute::getDefaultValue() { return defaultValue; }

bool BaseAttribute::isClientSyncable() { return syncable; }

BaseAttribute* BaseAttribute::setSyncable(bool syncable) {
    this->syncable = syncable;
    return this;
}