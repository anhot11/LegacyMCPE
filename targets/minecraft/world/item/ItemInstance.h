#pragma once

#include <memory>
#include <string>
#include <vector>

#include "UseAnim.h"
#include "minecraft/world/entity/ai/attributes/Attribute.h"
#include "nbt/NbtIo.h"

class Entity;
class Level;
class Player;
class Mob;
class LivingEntity;
class CompoundTag;
class Enchantment;
class Rarity;
class AttributeModifier;
class Random;
// 4J-PB - added
class MapItem;
class ItemFrame;
class Icon;
class HtmlString;
class Tile;
class Item;
class Tag;
template <class T>
class ListTag;

// 4J Stu - While this is not really an abstract class, we don't want to make
// new instances of it, mainly because there are too many ctors and that doesn't
// fit well into out macroisation setup
class ItemInstance : public std::enable_shared_from_this<ItemInstance> {
public:
    static const std::string ATTRIBUTE_MODIFIER_FORMAT;
    static const char* TAG_ENCH_ID;
    static const char* TAG_ENCH_LEVEL;

    int count;
    int popTime;
    int id;

    // 4J Stu - Brought forward for enchanting/game rules
    CompoundTag* tag;

    /**
     * This was previously the damage value, but is now used for different stuff
     * depending on item / tile. Use the getter methods to make sure the value
     * is interpreted correctly.
     */
private:
    int auxValue;
    // 4J-PB - added for trading menu
    bool m_bForceNumberDisplay;

    void _init(int id, int count, int auxValue);

    // TU9
    std::shared_ptr<ItemFrame> frame;

public:
    ItemInstance(Tile* tile);
    ItemInstance(Tile* tile, int count);
    ItemInstance(Tile* tile, int count, int auxValue);
    ItemInstance(Item* item);
    // 4J-PB - added
    ItemInstance(MapItem* item, int count);

    ItemInstance(Item* item, int count);
    ItemInstance(Item* item, int count, int auxValue);
    ItemInstance(int id, int count, int damage);

    static std::shared_ptr<ItemInstance> fromTag(CompoundTag* itemTag);

private:
    ItemInstance() { _init(-1, 0, 0); }

public:
    virtual ~ItemInstance();
    std::shared_ptr<ItemInstance> remove(int count);

    Item* getItem() const;
    Icon* getIcon();
    int getIconType();
    bool useOn(std::shared_ptr<Player> player, Level* level, int x, int y,
               int z, int face, float clickX, float clickY, float clickZ,
               bool bTestUseOnOnly = false);
    float getDestroySpeed(Tile* tile);
    bool TestUse(std::shared_ptr<ItemInstance> itemInstance, Level* level,
                 std::shared_ptr<Player> player);
    std::shared_ptr<ItemInstance> use(Level* level,
                                      std::shared_ptr<Player> player);
    std::shared_ptr<ItemInstance> useTimeDepleted(
        Level* level, std::shared_ptr<Player> player);
    CompoundTag* save(CompoundTag* compoundTag);
    void load(CompoundTag* compoundTag);
    int getMaxStackSize();
    bool isStackable();
    bool isDamageableItem();
    bool isStackedByData();
    bool isDamaged();
    int getDamageValue();
    int getAuxValue() const;
    void setAuxValue(int value);
    int getMaxDamage();
    bool hurt(int dmg, Random* random);
    void hurtAndBreak(int dmg, std::shared_ptr<LivingEntity> owner);
    void hurtEnemy(std::shared_ptr<LivingEntity> mob,
                   std::shared_ptr<Player> attacker);
    void mineBlock(Level* level, int tile, int x, int y, int z,
                   std::shared_ptr<Player> owner);
    bool canDestroySpecial(Tile* tile);
    bool interactEnemy(std::shared_ptr<Player> player,
                       std::shared_ptr<LivingEntity> mob);
    std::shared_ptr<ItemInstance> copy() const;
    ItemInstance* copy_not_shared() const;  // 4J Stu - Added for use in recipes
    static bool tagMatches(
        std::shared_ptr<ItemInstance> a,
        std::shared_ptr<ItemInstance> b);  // 4J Brought forward from 1.2
    static bool matches(std::shared_ptr<ItemInstance> a,
                        std::shared_ptr<ItemInstance> b);

    // 4J-PB
    int GetCount() { return count; }
    void ForceNumberDisplay(bool bForce) {
        m_bForceNumberDisplay = bForce;
    }  // to force the display of 0 and 1 on the required trading items when you
       // have o or 1 of the item
    bool GetForceNumberDisplay() {
        return m_bForceNumberDisplay;
    }  // to force the display of 0 and 1 on the required trading items when you
       // have o or 1 of the item

private:
    bool matches(std::shared_ptr<ItemInstance> b);

public:
    bool sameItem(std::shared_ptr<ItemInstance> b);
    bool sameItemWithTags(std::shared_ptr<ItemInstance> b);  // 4J Added
    bool sameItem_not_shared(
        ItemInstance* b);  // 4J Stu - Added this for the one time I need it
    virtual unsigned int getUseDescriptionId();  // 4J Added
    virtual unsigned int getDescriptionId(int iData = -1);
    virtual ItemInstance* setDescriptionId(unsigned int id);
    static std::shared_ptr<ItemInstance> clone(
        std::shared_ptr<ItemInstance> item);
    std::string toString();
    void inventoryTick(Level* level, std::shared_ptr<Entity> owner, int slot,
                       bool selected);
    void onCraftedBy(Level* level, std::shared_ptr<Player> player,
                     int craftCount);
    bool equals(std::shared_ptr<ItemInstance> ii);

    int getUseDuration();
    UseAnim getUseAnimation();
    void releaseUsing(Level* level, std::shared_ptr<Player> player,
                      int durationLeft);

    // 4J Stu - Brought forward these functions for enchanting/game rules
    bool hasTag();
    CompoundTag* getTag();
    ListTag<CompoundTag>* getEnchantmentTags();
    void setTag(CompoundTag* tag);
    std::string getHoverName();
    void setHoverName(const std::string& name);
    void resetHoverName();
    bool hasCustomHoverName();
    // 4jcraft: re-added old TU18 overload for java gui
    std::vector<std::string>* getHoverText(
        std::shared_ptr<Player> player, bool advanced,
        std::vector<std::string>& unformattedStrings);
    std::vector<HtmlString>* getHoverText(std::shared_ptr<Player> player,
                                          bool advanced);
    std::vector<HtmlString>* getHoverTextOnly(std::shared_ptr<Player> player,
                                              bool advanced);  // 4J Added
    bool isFoil();
    const Rarity* getRarity();
    bool isEnchantable();
    void enchant(const Enchantment* enchantment, int level);
    bool isEnchanted();
    void addTagElement(std::string name, Tag* tag);
    bool mayBePlacedInAdventureMode();
    bool isFramed();
    void setFramed(std::shared_ptr<ItemFrame> frame);
    std::shared_ptr<ItemFrame> getFrame();
    int getBaseRepairCost();
    void setRepairCost(int cost);
    attrAttrModMap* getAttributeModifiers();

    // 4J Added
    void set4JData(int data);
    int get4JData();
    bool hasPotionStrengthBar();
    int GetPotionStrength();
};