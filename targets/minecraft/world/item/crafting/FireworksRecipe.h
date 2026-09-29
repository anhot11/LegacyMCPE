#pragma once

#include <memory>

#include "Recipy.h"

class CraftingContainer;
class ItemInstance;

class FireworksRecipe : public Recipy {
private:
    // std::shared_ptr<ItemInstance> resultItem;

    // 4J added so we can have separate contexts and rleBuf for different
    // threads
    class ThreadStorage {
    public:
        std::shared_ptr<ItemInstance> resultItem;
        ThreadStorage();
    };
    static thread_local ThreadStorage* m_tlsStorage;
    static ThreadStorage* m_defaultThreadStorage;

    void setResultItem(std::shared_ptr<ItemInstance> item);

public:
    // Each new thread that needs to use Compression will need to call one of
    // the following 2 functions, to either create its own local storage, or
    // share the default storage already allocated by the main thread
    static void CreateNewThreadStorage();
    static void UseDefaultThreadStorage();
    static void ReleaseThreadStorage();

public:
    FireworksRecipe();

    bool matches(std::shared_ptr<CraftingContainer> craftSlots, Level* level);
    std::shared_ptr<ItemInstance> assemble(
        std::shared_ptr<CraftingContainer> craftSlots);
    int size();
    const ItemInstance* getResultItem();

    virtual const int getGroup() { return 0; }

    // 4J-PB
    virtual bool requiresRecipe(int iRecipe) { return false; };
    virtual void collectRequirements(INGREDIENTS_REQUIRED* pIngReq) {};

    // 4J Added
    static void updatePossibleRecipes(
        std::shared_ptr<CraftingContainer> craftSlots, bool* firework,
        bool* charge, bool* fade);
    static bool isValidIngredient(std::shared_ptr<ItemInstance> item,
                                  bool firework, bool charge, bool fade);
};