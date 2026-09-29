#include "MenuBackup.h"

#include <vector>

#include "AbstractContainerMenu.h"
#include "Slot.h"
#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/item/ItemInstance.h"

MenuBackup::MenuBackup(std::shared_ptr<Inventory> inventory,
                       AbstractContainerMenu* menu) {
    backups =
        new std::unordered_map<short,
                               std::vector<std::shared_ptr<ItemInstance>>*>();

    this->inventory = inventory;
    this->menu = menu;
}

void MenuBackup::save(short changeUid) {
    std::vector<std::shared_ptr<ItemInstance>>* backup =
        new std::vector<std::shared_ptr<ItemInstance>>((int)menu->slots.size() +
                                                       1);
    (*backup)[0] = ItemInstance::clone(inventory->getCarried());
    for (unsigned int i = 0; i < menu->slots.size(); i++) {
        (*backup)[i + 1] = ItemInstance::clone(menu->slots.at(i)->getItem());
    }
    // TODO Is unordered_map use correct?
    // Was backups.put(changeUid, backup);
    (*backups)[changeUid] = backup;
}

// Cannot use delete as function name as it is a reserved keyword
void MenuBackup::deleteBackup(short changeUid) {
    // TODO Is the unordered_map use correct?
    // 4J Was backups.remove(changeUid);
    backups->erase(changeUid);
}

void MenuBackup::rollback(short changeUid) {
    std::vector<std::shared_ptr<ItemInstance>>* backup = backups->at(changeUid);
    backups->clear();
    inventory->setCarried((*backup)[0]);
    for (unsigned int i = 0; i < menu->slots.size(); i++) {
        menu->slots.at(i)->set((*backup)[i + 1]);
    }
}