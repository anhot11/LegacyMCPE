#include "BaseAttributeMap.h"

#include <utility>
#include <vector>

#include "minecraft/world/entity/ai/attributes/Attribute.h"
#include "minecraft/world/entity/ai/attributes/AttributeInstance.h"
#include "minecraft/world/entity/ai/attributes/AttributeModifier.h"
#include "minecraft/world/item/ItemInstance.h"

BaseAttributeMap::~BaseAttributeMap() {
    for (auto it = attributesById.begin(); it != attributesById.end(); ++it) {
        delete it->second;
    }
}

AttributeInstance* BaseAttributeMap::getInstance(Attribute* attribute) {
    return getInstance(attribute->getId());
}

AttributeInstance* BaseAttributeMap::getInstance(eATTRIBUTE_ID id) {
    auto it = attributesById.find(id);
    if (it != attributesById.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

void BaseAttributeMap::getAttributes(std::vector<AttributeInstance*>& atts) {
    for (auto it = attributesById.begin(); it != attributesById.end(); ++it) {
        atts.push_back(it->second);
    }
}

void BaseAttributeMap::onAttributeModified(
    ModifiableAttributeInstance* attributeInstance) {}

void BaseAttributeMap::removeItemModifiers(std::shared_ptr<ItemInstance> item) {
    attrAttrModMap* modifiers = item->getAttributeModifiers();

    for (auto it = modifiers->begin(); it != modifiers->end(); ++it) {
        AttributeInstance* attribute = getInstance(it->first);
        AttributeModifier* modifier = it->second;

        if (attribute != nullptr) {
            attribute->removeModifier(modifier);
        }

        delete modifier;
    }

    delete modifiers;
}

void BaseAttributeMap::addItemModifiers(std::shared_ptr<ItemInstance> item) {
    attrAttrModMap* modifiers = item->getAttributeModifiers();

    for (auto it = modifiers->begin(); it != modifiers->end(); ++it) {
        AttributeInstance* attribute = getInstance(it->first);
        AttributeModifier* modifier = it->second;

        if (attribute != nullptr) {
            attribute->removeModifier(modifier);
            attribute->addModifier(new AttributeModifier(*modifier));
        }

        delete modifier;
    }

    delete modifiers;
}
