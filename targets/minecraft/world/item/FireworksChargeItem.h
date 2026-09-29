#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Item.h"
#include "nbt/CompoundTag.h"
#include "nbt/Tag.h"

class CompoundTag;
class HtmlString;
class Icon;
class ItemInstance;
class Tag;

class FireworksChargeItem : public Item {
private:
    Icon* overlay;

public:
    FireworksChargeItem(int id);

    virtual Icon* getLayerIcon(int auxValue, int spriteLayer);
    virtual int getColor(std::shared_ptr<ItemInstance> item, int spriteLayer);
    virtual bool hasMultipleSpriteLayers();

    static Tag* getExplosionTagField(std::shared_ptr<ItemInstance> instance,
                                     const std::string& field);

    virtual void appendHoverText(std::shared_ptr<ItemInstance> itemInstance,
                                 std::shared_ptr<Player> player,
                                 std::vector<HtmlString>* lines, bool advanced);

    static void appendHoverText(CompoundTag* expTag,
                                std::vector<HtmlString>* lines);

    virtual void registerIcons(IconRegister* iconRegister);
};