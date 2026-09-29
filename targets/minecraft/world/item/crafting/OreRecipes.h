#pragma once

#include <vector>

class Object;
class Recipes;

#define MAX_ORE_RECIPES 8

class OreRecipies {
public:
    // 4J - added for common ctor code
    void _init();
    OreRecipies() { _init(); }

private:
    std::vector<Object*> map[MAX_ORE_RECIPES];

public:
    void addRecipes(Recipes* r);
};