#pragma once

#include <memory>

#include "IUIScene_AbstractContainerMenu.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/entity/animal/EntityHorse.h"
#include "minecraft/world/entity/player/Inventory.h"

class Container;
class EntityHorse;
class Inventory;

class IUIScene_HorseInventoryMenu
    : public virtual IUIScene_AbstractContainerMenu {
protected:
    std::shared_ptr<Inventory> m_inventory;
    std::shared_ptr<Container> m_container;
    std::shared_ptr<EntityHorse> m_horse;

public:
    virtual ESceneSection GetSectionAndSlotInDirection(ESceneSection eSection,
                                                       ETapState eTapDirection,
                                                       int* piTargetX,
                                                       int* piTargetY);
    int getSectionStartOffset(ESceneSection eSection);
    bool IsSectionSlotList(ESceneSection eSection);
    bool IsVisible(ESceneSection eSection);
};
