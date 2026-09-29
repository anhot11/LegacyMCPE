#include "OreRecipes.h"

#include "Recipes.h"
#include "minecraft/world/item/CoalItem.h"
#include "minecraft/world/item/DyePowderItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"

void OreRecipies::_init() {
    ADD_OBJECT(map[0], Tile::goldBlock);
    ADD_OBJECT(map[0], new ItemInstance(Item::goldIngot, 9));

    ADD_OBJECT(map[1], Tile::ironBlock);
    ADD_OBJECT(map[1], new ItemInstance(Item::ironIngot, 9));

    ADD_OBJECT(map[2], Tile::diamondBlock);
    ADD_OBJECT(map[2], new ItemInstance(Item::diamond, 9));

    ADD_OBJECT(map[3], Tile::emeraldBlock);
    ADD_OBJECT(map[3], new ItemInstance(Item::emerald, 9));

    ADD_OBJECT(map[4], Tile::lapisBlock);
    ADD_OBJECT(map[4],
               new ItemInstance(Item::dye_powder, 9, DyePowderItem::BLUE));

    ADD_OBJECT(map[5], Tile::redstoneBlock);
    ADD_OBJECT(map[5], new ItemInstance(Item::redStone, 9));

    ADD_OBJECT(map[6], Tile::coalBlock);
    ADD_OBJECT(map[6], new ItemInstance(Item::coal, 9, CoalItem::STONE_COAL));

    ADD_OBJECT(map[7], Tile::hayBlock);
    ADD_OBJECT(map[7], new ItemInstance(Item::wheat, 9));
}
void OreRecipies::addRecipes(Recipes* r) {
    for (int i = 0; i < MAX_ORE_RECIPES; i++) {
        Tile* from = (Tile*)map[i].at(0)->tile;
        ItemInstance* to = (ItemInstance*)map[i].at(1)->iteminstance;
        r->addShapedRecipy(new ItemInstance(from),  //
                           "sssczg",
                           "###",  //
                           "###",  //
                           "###",  //

                           '#', to, 'D');

        r->addShapedRecipy(to,  //
                           "sctg",
                           "#",  //

                           '#', from, 'D');
    }
}
