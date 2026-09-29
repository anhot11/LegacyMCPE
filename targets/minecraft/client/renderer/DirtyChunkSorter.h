#pragma once
#include <memory>

class Chunk;
class Mob;
class LivingEntity;

class DirtyChunkSorter {
private:
    std::shared_ptr<LivingEntity> cameraEntity;
    int playerIndex;  // 4J added

public:
    DirtyChunkSorter(std::shared_ptr<LivingEntity> cameraEntity,
                     int playerIndex);  // 4J - added player index
    bool operator()(const Chunk* a, const Chunk* b) const;
};
