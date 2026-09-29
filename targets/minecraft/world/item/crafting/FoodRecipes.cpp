#include "FoodRecipes.h"

#include "Recipes.h"
#include "minecraft/world/item/DyePowderItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"

void FoodRecipies::addRecipes(Recipes* r) {
    // 4J-JEV: Bumped up in the list to avoid a colision with the title.
    r->addShapedRecipy(new ItemInstance(Item::apple_gold, 1, 0),  //
                       "ssscicig",
                       "###",  //
                       "#X#",  //
                       "###",  //
                       '#', Item::goldIngot, 'X', Item::apple, 'F');

    r->addShapedRecipy(new ItemInstance(Item::apple_gold, 1, 1),  //
                       "sssctcig",
                       "###",  //
                       "#X#",  //
                       "###",  //
                       '#', Tile::goldBlock, 'X', Item::apple, 'F');

    r->addShapedRecipy(new ItemInstance(Item::speckledMelon, 1),  //
                       "ssscicig",
                       "###",  //
                       "#X#",  //
                       "###",  //

                       '#', Item::goldNugget, 'X', Item::melon, 'F');

    r->addShapelessRecipy(new ItemInstance(Item::mushroomStew), "ttig",
                          Tile::mushroom_brown, Tile::mushroom_red, Item::bowl,
                          'F');

    r->addShapedRecipy(
        new ItemInstance(Item::cookie, 8),  //
        "sczcig",
        "#X#",  //

        'X', new ItemInstance(Item::dye_powder, 1, DyePowderItem::BROWN), '#',
        Item::wheat, 'F');

    r->addShapedRecipy(new ItemInstance(Tile::melon),  //
                       "ssscig",
                       "MMM",  //
                       "MMM",  //
                       "MMM",  //

                       'M', Item::melon, 'F');

    r->addShapedRecipy(new ItemInstance(Item::seeds_melon),  //
                       "scig",
                       "M",  //

                       'M', Item::melon, 'F');

    r->addShapedRecipy(new ItemInstance(Item::seeds_pumpkin, 4),  //
                       "sctg",
                       "M",  //

                       'M', Tile::pumpkin, 'F');

    r->addShapelessRecipy(new ItemInstance(Item::pumpkinPie),  //
                          "tiig", Tile::pumpkin, Item::sugar, Item::egg, 'F');

    r->addShapedRecipy(new ItemInstance(Item::carrotGolden, 1, 0),  //
                       "ssscicig",
                       "###",  //
                       "#X#",  //
                       "###",  //

                       '#', Item::goldNugget, 'X', Item::carrots, 'F');

    r->addShapelessRecipy(new ItemInstance(Item::fermentedSpiderEye),  //
                          "itig", Item::spiderEye, Tile::mushroom_brown,
                          Item::sugar, 'F');

    r->addShapelessRecipy(new ItemInstance(Item::blazePowder, 2),  //
                          "ig", Item::blazeRod, 'F');

    r->addShapelessRecipy(new ItemInstance(Item::magmaCream),  //
                          "iig", Item::blazePowder, Item::slimeBall, 'F');
}
