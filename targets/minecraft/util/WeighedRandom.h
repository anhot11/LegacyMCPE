#pragma once

#include <vector>

#include "java/Random.h"

class Random;

// 4J - this WeighedRandomItem class was a nested static class within
// WeighedRandom, but we need to be able to refer to it externally

class WeighedRandomItem {
    friend class WeighedRandom;

protected:
    int randomWeight;

public:
    WeighedRandomItem(int randomWeight) { this->randomWeight = randomWeight; }
};

class WeighedRandom {
public:
    // 4J - vectors here were Collection<? extends WeighedRandomItem>
    static int getTotalWeight(std::vector<WeighedRandomItem*>* items);
    static WeighedRandomItem* getRandomItem(
        Random* random, std::vector<WeighedRandomItem*>* items,
        int totalWeight);
    static WeighedRandomItem* getRandomItem(
        Random* random, std::vector<WeighedRandomItem*>* items);
    static int getTotalWeight(const std::vector<WeighedRandomItem*>& items);
    static WeighedRandomItem* getRandomItem(
        Random* random, const std::vector<WeighedRandomItem*>& items,
        int totalWeight);
    static WeighedRandomItem* getRandomItem(
        Random* random, const std::vector<WeighedRandomItem*>& items);
};
