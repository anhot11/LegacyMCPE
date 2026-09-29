// package net.minecraft.world.item.crafting;

// import net.minecraft.world.item.*;
// import net.minecraft.world.level.tile.Tile;

#pragma once

#include <format>
#include <string>
#include <vector>

class Object;
class Recipes;

#define MAX_WEAPON_RECIPES 2
class WeaponRecipies {
public:
    // 4J - added for common ctor code
    void _init();
    WeaponRecipies() { _init(); }

private:
    static std::string shapes[][4];
    std::vector<Object*>* map;

public:
    void addRecipes(Recipes* r);
};
