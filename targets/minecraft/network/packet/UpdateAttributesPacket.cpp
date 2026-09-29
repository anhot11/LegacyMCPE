#include "UpdateAttributesPacket.h"

#include <stdint.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/entity/ai/attributes/Attribute.h"
#include "minecraft/world/entity/ai/attributes/AttributeInstance.h"
#include "minecraft/world/entity/ai/attributes/AttributeModifier.h"

UpdateAttributesPacket::UpdateAttributesPacket() { entityId = 0; }

UpdateAttributesPacket::UpdateAttributesPacket(
    int entityId, std::unordered_set<AttributeInstance*>* values) {
    this->entityId = entityId;

    for (auto it = values->begin(); it != values->end(); ++it) {
        AttributeInstance* value = *it;
        std::unordered_set<AttributeModifier*> mods;
        value->getModifiers(mods);
        attributes.insert(new AttributeSnapshot(value->getAttribute()->getId(),
                                                value->getBaseValue(), &mods));
    }
}

UpdateAttributesPacket::~UpdateAttributesPacket() {
    // Delete modifiers - these are always copies, either on construction or on
    // read
    for (auto it = attributes.begin(); it != attributes.end(); ++it) {
        delete (*it);
    }
}

void UpdateAttributesPacket::read(DataInputStream* dis) {
    entityId = dis->readInt();

    int attributeCount = dis->readInt();
    for (int i = 0; i < attributeCount; i++) {
        eATTRIBUTE_ID id = static_cast<eATTRIBUTE_ID>(dis->readShort());
        double base = dis->readDouble();
        std::unordered_set<AttributeModifier*> modifiers =
            std::unordered_set<AttributeModifier*>();
        int modifierCount = dis->readShort();

        for (int j = 0; j < modifierCount; j++) {
            eMODIFIER_ID id = static_cast<eMODIFIER_ID>(dis->readInt());
            double amount = dis->readDouble();
            uint8_t operation = dis->readByte();
            modifiers.insert(new AttributeModifier(
                id, /*"Unknown synced attribute modifier",*/ amount,
                operation));
        }

        attributes.insert(new AttributeSnapshot(id, base, &modifiers));

        // modifiers is copied in AttributeSnapshot ctor so delete contents
        for (auto it = modifiers.begin(); it != modifiers.end(); ++it) {
            delete *it;
        }
    }
}

void UpdateAttributesPacket::write(DataOutputStream* dos) {
    dos->writeInt(entityId);
    dos->writeInt(attributes.size());

    for (auto it = attributes.begin(); it != attributes.end(); ++it) {
        AttributeSnapshot* attribute = (*it);

        std::unordered_set<AttributeModifier*>* modifiers =
            attribute->getModifiers();

        dos->writeShort(attribute->getId());
        dos->writeDouble(attribute->getBase());
        dos->writeShort(modifiers->size());

        for (auto it2 = modifiers->begin(); it2 != modifiers->end(); ++it2) {
            AttributeModifier* modifier = (*it2);
            dos->writeInt(modifier->getId());
            dos->writeDouble(modifier->getAmount());
            dos->writeByte(modifier->getOperation());
        }
    }
}

void UpdateAttributesPacket::handle(PacketListener* listener) {
    listener->handleUpdateAttributes(shared_from_this());
}

int UpdateAttributesPacket::getEstimatedSize() {
    return 4 + 4 + attributes.size() * (8 + 8 + 8);
}

int UpdateAttributesPacket::getEntityId() { return entityId; }

std::unordered_set<UpdateAttributesPacket::AttributeSnapshot*>
UpdateAttributesPacket::getValues() {
    return attributes;
}

UpdateAttributesPacket::AttributeSnapshot::AttributeSnapshot(
    eATTRIBUTE_ID id, double base,
    std::unordered_set<AttributeModifier*>* modifiers) {
    this->id = id;
    this->base = base;

    for (auto it = modifiers->begin(); it != modifiers->end(); ++it) {
        this->modifiers.insert(new AttributeModifier(
            (*it)->getId(), (*it)->getAmount(), (*it)->getOperation()));
    }
}

UpdateAttributesPacket::AttributeSnapshot::~AttributeSnapshot() {
    for (auto it = modifiers.begin(); it != modifiers.end(); ++it) {
        delete (*it);
    }
}

eATTRIBUTE_ID UpdateAttributesPacket::AttributeSnapshot::getId() { return id; }

double UpdateAttributesPacket::AttributeSnapshot::getBase() { return base; }

std::unordered_set<AttributeModifier*>*
UpdateAttributesPacket::AttributeSnapshot::getModifiers() {
    return &modifiers;
}