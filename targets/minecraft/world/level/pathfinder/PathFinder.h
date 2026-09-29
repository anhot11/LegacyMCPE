#pragma once
#include <format>
#include <unordered_map>
#include <vector>

#include "java/JavaIntHash.h"
#include "minecraft/world/level/pathfinder/BinaryHeap.h"

class LevelSource;
class Entity;
class Node;
class Path;

class PathFinder {
private:
    LevelSource* level;

    BinaryHeap openSet;

    // 4J Jev, was a IntHashMap, thought this was close enough.
    std::unordered_map<int, Node*, IntKeyHash, IntKeyEq> nodes;

    std::vector<Node*>* neighbors;

    bool canPassDoors;
    bool canOpenDoors;
    bool avoidWater;
    bool canFloat;

public:
    PathFinder(LevelSource* level, bool canPassDoors, bool canOpenDoors,
               bool avoidWater, bool canFloat);
    ~PathFinder();

    Path* findPath(Entity* from, Entity* to, float maxDist);
    Path* findPath(Entity* from, int x, int y, int z, float maxDist);

private:
    Path* findPath(Entity* e, double xt, double yt, double zt, float maxDist);

    // function A*(start,goal)
    Path* findPath(Entity* e, Node* from, Node* to, Node* size, float maxDist);
    int getNeighbors(Entity* entity, Node* pos, Node* size, Node* target,
                     float maxDist);
    Node* getNode(Entity* entity, int x, int y, int z, Node* size,
                  int jumpSize);
    /*final*/ Node* getNode(int x, int y, int z);

public:
    static const int TYPE_TRAP = -4;
    static const int TYPE_FENCE = -3;
    static const int TYPE_LAVA = -2;
    static const int TYPE_WATER = -1;
    static const int TYPE_BLOCKED = 0;
    static const int TYPE_OPEN = 1;
    static const int TYPE_WALKABLE = 2;

    int isFree(Entity* entity, int x, int y, int z, Node* size);
    static int isFree(Entity* entity, int x, int y, int z, Node* size,
                      bool avoidWater, bool canOpenDoors, bool canPassDoors);

    // function reconstruct_path(came_from,current_node)
    Path* reconstruct_path(Node* from, Node* to);
};