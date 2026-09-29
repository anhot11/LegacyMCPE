#include "minecraft/world/level/levelgen/structure/StructureFeature.h"

#include <float.h>
#include <stdint.h>

#include <format>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

#include "StructureStart.h"
#include "java/Random.h"
#include "minecraft/world/level/ChunkPos.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/LevelType.h"
#include "minecraft/world/level/TilePos.h"
#include "minecraft/world/level/levelgen/structure/BoundingBox.h"
#include "minecraft/world/level/levelgen/structure/StructurePiece.h"
#include "minecraft/world/level/storage/LevelData.h"

StructureFeature::StructureFeature() {
#ifdef ENABLE_STRUCTURE_SAVING
    savedData = nullptr;
#endif
}

StructureFeature::~StructureFeature() {
    for (auto it = cachedStructures.begin(); it != cachedStructures.end();
         it++) {
        delete it->second;
    }
}

void StructureFeature::addFeature(Level* level, int x, int z, int xOffs,
                                  int zOffs, std::vector<uint8_t>& blocks) {
    // this method is called for each chunk within 8 chunk's distance from
    // the chunk being generated, but not all chunks are the sources of
    // structures

    restoreSavedData(level);

    if (cachedStructures.find(ChunkPos::hashCode(x, z)) !=
        cachedStructures.end()) {
        return;
    }

    // clear random key
    random->nextInt();
    // 4J-PB - want to know if it's a superflat land, so we don't generate so
    // many villages - we've changed the distance required between villages on
    // the xbox
    if (isFeatureChunk(
            x, z,
            level->getLevelData()->getGenerator() == LevelType::lvl_flat)) {
        StructureStart* start = createStructureStart(x, z);
        cachedStructures[ChunkPos::hashCode(x, z)] = start;
        saveFeature(x, z, start);
    }
}

bool StructureFeature::postProcess(Level* level, Random* random, int chunkX,
                                   int chunkZ) {
    restoreSavedData(level);

    // 4J Stu - The x and z used to be offset by (+8) here, but that means we
    // can miss out half structures on the edge of the world Normal feature
    // generation offsets generation by half a chunk to ensure that it can
    // generate the entire feature in chunks already created Structure features
    // don't need this, as the PlaceBlock function only places blocks inside the
    // BoundingBox specified, and parts of a struture piece can be added in more
    // than one post-process call
    int cx = ((unsigned)chunkX << 4);  // + 8;
    int cz = ((unsigned)chunkZ << 4);  // + 8;

    bool intersection = false;
    for (auto it = cachedStructures.begin(); it != cachedStructures.end();
         it++) {
        StructureStart* structureStart = it->second;

        if (structureStart->isValid()) {
            if (structureStart->getBoundingBox()->intersects(cx, cz, cx + 15,
                                                             cz + 15)) {
                BoundingBox* bb = new BoundingBox(cx, cz, cx + 15, cz + 15);
                structureStart->postProcess(level, random, bb);
                delete bb;
                intersection = true;

                // because some feature pieces are modified in the postProcess
                // step, we need to save them again
                saveFeature(structureStart->getChunkX(),
                            structureStart->getChunkZ(), structureStart);
            }
        }
    }

    return intersection;
}

bool StructureFeature::isIntersection(int cellX, int cellZ) {
    restoreSavedData(level);

    for (auto it = cachedStructures.begin(); it != cachedStructures.end();
         it++) {
        StructureStart* structureStart = it->second;
        if (structureStart->isValid()) {
            if (structureStart->getBoundingBox()->intersects(cellX, cellZ,
                                                             cellX, cellZ)) {
                auto it2 = structureStart->getPieces()->begin();
                while (it2 != structureStart->getPieces()->end()) {
                    StructurePiece* next = *it2++;
                    if (next->getBoundingBox()->intersects(cellX, cellZ, cellX,
                                                           cellZ)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool StructureFeature::isInsideFeature(int cellX, int cellY, int cellZ) {
    restoreSavedData(level);
    return getStructureAt(cellX, cellY, cellZ) != nullptr;
}

StructureStart* StructureFeature::getStructureAt(int cellX, int cellY,
                                                 int cellZ) {
    // for (StructureStart structureStart : cachedStructures.values())
    for (auto it = cachedStructures.begin(); it != cachedStructures.end();
         ++it) {
        StructureStart* pStructureStart = it->second;

        if (pStructureStart->isValid()) {
            if (pStructureStart->getBoundingBox()->intersects(cellX, cellZ,
                                                              cellX, cellZ)) {
                /*
                Iterator<StructurePiece> it =
                structureStart.getPieces().iterator(); while (it.hasNext())
                { StructurePiece next = it.next(); if
                (next.getBoundingBox().isInside(cellX, cellY, cellZ)) { return
                true;
                }
                */
                std::list<StructurePiece*>* pieces =
                    pStructureStart->getPieces();

                for (auto it2 = pieces->begin(); it2 != pieces->end(); it2++) {
                    StructurePiece* piece = *it2;
                    if (piece->getBoundingBox()->isInside(cellX, cellY,
                                                          cellZ)) {
                        return pStructureStart;
                    }
                }
            }
        }
    }
    return nullptr;
}

bool StructureFeature::isInsideBoundingFeature(int cellX, int cellY,
                                               int cellZ) {
    restoreSavedData(level);

    for (auto it = cachedStructures.begin(); it != cachedStructures.end();
         ++it) {
        StructureStart* structureStart = it->second;
        if (structureStart->isValid()) {
            return (structureStart->getBoundingBox()->intersects(cellX, cellZ,
                                                                 cellX, cellZ));
        }
    }
    return false;
}

TilePos* StructureFeature::getNearestGeneratedFeature(Level* level, int cellX,
                                                      int cellY, int cellZ) {
    // this is a hack that will "force" the feature to generate positions
    // even if the player hasn't generated new chunks yet
    this->level = level;

    restoreSavedData(level);

    random->setSeed(level->getSeed());
    int64_t xScale = random->nextLong();
    int64_t zScale = random->nextLong();
    int64_t xx = (cellX >> 4) * xScale;
    int64_t zz = (cellZ >> 4) * zScale;
    random->setSeed(xx ^ zz ^ level->getSeed());

    std::vector<uint8_t> emptyBlocks;
    addFeature(level, cellX >> 4, cellZ >> 4, 0, 0, emptyBlocks);

    double minDistance = DBL_MAX;
    TilePos* selected = nullptr;

    for (auto it = cachedStructures.begin(); it != cachedStructures.end();
         ++it) {
        StructureStart* pStructureStart = it->second;

        if (pStructureStart->isValid()) {
            // StructurePiece *pStructurePiece =
            // pStructureStart->getPieces().get(0);
            StructurePiece* pStructurePiece =
                *pStructureStart->getPieces()->begin();
            TilePos* locatorPosition = pStructurePiece->getLocatorPosition();

            int dx = locatorPosition->x - cellX;
            int dy = locatorPosition->y - cellY;
            int dz = locatorPosition->z - cellZ;
            double dist = dx * dx + dy * dy + dz * dz;

            if (dist < minDistance) {
                minDistance = dist;
                selected = locatorPosition;
            }
        }
    }
    if (selected != nullptr) {
        return selected;
    } else {
        std::vector<TilePos>* guesstimatedFeaturePositions =
            getGuesstimatedFeaturePositions();
        if (guesstimatedFeaturePositions != nullptr) {
            TilePos* pSelectedPos = new TilePos(0, 0, 0);

            for (auto it = guesstimatedFeaturePositions->begin();
                 it != guesstimatedFeaturePositions->end(); ++it) {
                int dx = (*it).x - cellX;
                int dy = (*it).y - cellY;
                int dz = (*it).z - cellZ;
                double dist = dx * dx + dy * dy + dz * dz;

                if (dist < minDistance) {
                    minDistance = dist;
                    pSelectedPos->x = (*it).x;
                    pSelectedPos->y = (*it).y;
                    pSelectedPos->z = (*it).z;
                }
            }
            delete guesstimatedFeaturePositions;
            return pSelectedPos;
        }
    }
    return nullptr;
}

std::vector<TilePos>* StructureFeature::getGuesstimatedFeaturePositions() {
    return nullptr;
}

void StructureFeature::restoreSavedData(Level* level) {
#ifdef ENABLE_STRUCTURE_SAVING
    if (savedData == nullptr) {
        savedData = std::dynamic_pointer_cast<StructureFeatureSavedData>(
            level->getSavedData(typeid(StructureFeatureSavedData),
                                getFeatureName()));

        if (savedData == nullptr) {
            savedData = std::shared_ptr<StructureFeatureSavedData>(
                new StructureFeatureSavedData(getFeatureName()));
            level->setSavedData(getFeatureName(), savedData);
        } else {
            CompoundTag* fullTag = savedData->getFullTag();

            std::vector<Tag*> allTags = fullTag->getAllTags();
            for (auto it = allTags.begin(); it != allTags.end(); ++it) {
                Tag* featureTag = *it;
                if (featureTag->getId() == Tag::TAG_Compound) {
                    CompoundTag* ct = (CompoundTag*)featureTag;

                    if (ct->contains("ChunkX") && ct->contains("ChunkZ")) {
                        int cx = ct->getInt("ChunkX");
                        int cz = ct->getInt("ChunkZ");

                        StructureStart* start =
                            StructureFeatureIO::loadStaticStart(ct, level);
                        // System.out.println("Loaded " +
                        // start.getClass().getSimpleName() + " from file");
                        cachedStructures[ChunkPos::hashCode(cx, cz)] = start;
                    }
                }
            }
            delete allTags;
        }
    }
#endif
}

void StructureFeature::saveFeature(int chunkX, int chunkZ,
                                   StructureStart* feature) {
#ifdef ENABLE_STRUCTURE_SAVING
    savedData->putFeatureTag(feature->createTag(chunkX, chunkZ), chunkX,
                             chunkZ);
    savedData->setDirty();
#endif
}
