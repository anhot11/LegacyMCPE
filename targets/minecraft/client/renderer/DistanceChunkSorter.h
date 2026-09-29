#pragma once
#include <memory>

class Entity;
class Chunk;

class DistanceChunkSorter {
private:
    double ix, iy, iz;

public:
    DistanceChunkSorter(std::shared_ptr<Entity> player);
    bool operator()(const Chunk* a, const Chunk* b) const;
};
