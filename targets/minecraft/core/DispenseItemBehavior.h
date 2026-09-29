#pragma once

#include <memory>

#include "Behavior.h"

class ItemInstance;
class BlockSource;

class DispenseItemBehavior : public Behavior {
public:
    /**
     * The 'do nothing' behavior.
     */
    static DispenseItemBehavior* NOOP;

    virtual ~DispenseItemBehavior() = default;

    /**
     *
     * @param source The source of this call (the dispenser that calls it)
     * @param dispensed The ItemInstance which is being dispensed
     * @return The ItemInstance that should is 'left over'
     */
    virtual std::shared_ptr<ItemInstance> dispense(
        BlockSource* source, std::shared_ptr<ItemInstance> dispensed) = 0;
};

class NoOpDispenseItemBehavior : public DispenseItemBehavior {
public:
    std::shared_ptr<ItemInstance> dispense(
        BlockSource* source, std::shared_ptr<ItemInstance> dispensed);
};
