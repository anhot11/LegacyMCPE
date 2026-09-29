#include "DispenseItemBehavior.h"

DispenseItemBehavior* DispenseItemBehavior::NOOP =
    new NoOpDispenseItemBehavior();

std::shared_ptr<ItemInstance> NoOpDispenseItemBehavior::dispense(
    BlockSource* source, std::shared_ptr<ItemInstance> dispensed) {
    return dispensed;
}