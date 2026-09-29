#pragma once

#include <format>
#include <memory>
#include <unordered_map>
#include <vector>

class ItemInstance;
class Inventory;
class DamageSource;
class Enchantment;
class EnchantmentInstance;
class Entity;
class LivingEntity;
class Random;

class EnchantmentHelper {
private:
    static Random random;

public:
    static int getEnchantmentLevel(int enchantmentId,
                                   std::shared_ptr<ItemInstance> piece);
    static std::unordered_map<int, int>* getEnchantments(
        std::shared_ptr<ItemInstance> item);
    static void setEnchantments(std::unordered_map<int, int>* enchantments,
                                std::shared_ptr<ItemInstance> item);

    static int getEnchantmentLevel(
        int enchantmentId,
        std::vector<std::shared_ptr<ItemInstance>> inventory);

private:
    class EnchantmentIterationMethod {
    public:
        virtual void doEnchantment(Enchantment* enchantment, int level) = 0;
    };

    static void runIterationOnItem(EnchantmentIterationMethod& method,
                                   std::shared_ptr<ItemInstance> piece);
    static void runIterationOnInventory(
        EnchantmentIterationMethod& method,
        std::vector<std::shared_ptr<ItemInstance>> inventory);

    class GetDamageProtectionIteration : public EnchantmentIterationMethod {
    public:
        int sum;
        DamageSource* source;

        virtual void doEnchantment(Enchantment* enchantment, int level);
    };

    static GetDamageProtectionIteration getDamageProtectionIteration;

    /**
     * Fetches the protection value for enchanted items.
     *
     * @param inventory
     * @param source
     * @return
     */
public:
    static int getDamageProtection(
        std::vector<std::shared_ptr<ItemInstance>> armor, DamageSource* source);

private:
    class GetDamageBonusIteration : public EnchantmentIterationMethod {
    public:
        float sum;
        std::shared_ptr<LivingEntity> target;

        virtual void doEnchantment(Enchantment* enchantment, int level);
    };

    static GetDamageBonusIteration getDamageBonusIteration;

    /**
     *
     * @param inventory
     * @param target
     * @return
     */
public:
    static float getDamageBonus(std::shared_ptr<LivingEntity> source,
                                std::shared_ptr<LivingEntity> target);
    static int getKnockbackBonus(std::shared_ptr<LivingEntity> source,
                                 std::shared_ptr<LivingEntity> target);
    static int getFireAspect(std::shared_ptr<LivingEntity> source);
    static int getOxygenBonus(std::shared_ptr<LivingEntity> source);
    static int getDiggingBonus(std::shared_ptr<LivingEntity> source);
    static int getDigDurability(std::shared_ptr<LivingEntity> source);
    static bool hasSilkTouch(std::shared_ptr<LivingEntity> source);
    static int getDiggingLootBonus(std::shared_ptr<LivingEntity> source);
    static int getKillingLootBonus(std::shared_ptr<LivingEntity> source);
    static bool hasWaterWorkerBonus(std::shared_ptr<LivingEntity> source);
    static int getArmorThorns(std::shared_ptr<LivingEntity> source);
    static std::shared_ptr<ItemInstance> getRandomItemWith(
        Enchantment* enchantment, std::shared_ptr<LivingEntity> source);

    /**
     *
     * @param random
     * @param slot
     *            The table slot, 0-2
     * @param bookcases
     *            How many book cases that are found around the table.
     * @param itemInstance
     *            Which item that is being enchanted.
     * @return The enchantment cost, 0 means unchantable, 50 is max.
     */
    static int getEnchantmentCost(Random* random, int slot, int bookcases,
                                  std::shared_ptr<ItemInstance> itemInstance);

    static std::shared_ptr<ItemInstance> enchantItem(
        Random* random, std::shared_ptr<ItemInstance> itemInstance,
        int enchantmentCost);

    /**
     *
     * @param random
     * @param itemInstance
     * @param enchantmentCost
     * @return
     */
    static std::vector<EnchantmentInstance*>* selectEnchantment(
        Random* random, std::shared_ptr<ItemInstance> itemInstance,
        int enchantmentCost);
    static std::unordered_map<int, EnchantmentInstance*>*
    getAvailableEnchantmentResults(int value,
                                   std::shared_ptr<ItemInstance> itemInstance);
};