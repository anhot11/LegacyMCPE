#include "StructureStart.h"

#include <string>

#include "java/Random.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/levelgen/structure/BoundingBox.h"
#include "minecraft/world/level/levelgen/structure/StructureFeatureIO.h"
#include "minecraft/world/level/levelgen/structure/StructurePiece.h"
#include "nbt/CompoundTag.h"
#include "nbt/IntArrayTag.h"
#include "nbt/ListTag.h"

StructureStart::StructureStart() {
    chunkX = chunkZ = 0;
    boundingBox = nullptr;  // 4J added initialiser
}

StructureStart::StructureStart(int x, int z) {
    this->chunkX = x;
    this->chunkZ = z;
    boundingBox = nullptr;
}

StructureStart::~StructureStart() {
    for (auto it = pieces.begin(); it != pieces.end(); it++) {
        delete (*it);
    }
    delete boundingBox;
}

BoundingBox* StructureStart::getBoundingBox() { return boundingBox; }

std::list<StructurePiece*>* StructureStart::getPieces() { return &pieces; }

void StructureStart::postProcess(Level* level, Random* random,
                                 BoundingBox* chunkBB) {
    auto it = pieces.begin();

    while (it != pieces.end()) {
        if ((*it)->getBoundingBox()->intersects(chunkBB) &&
            !(*it)->postProcess(level, random, chunkBB)) {
            // this piece can't be placed, so remove it to avoid future
            // attempts
            it = pieces.erase(it);
        } else {
            it++;
        }
    }
}

void StructureStart::calculateBoundingBox() {
    boundingBox = BoundingBox::getUnknownBox();

    for (auto it = pieces.begin(); it != pieces.end(); it++) {
        StructurePiece* piece = *it;
        boundingBox->expand(piece->getBoundingBox());
    }
}

CompoundTag* StructureStart::createTag(int chunkX, int chunkZ) {
    CompoundTag* tag = new CompoundTag();

    tag->putString("id", StructureFeatureIO::getEncodeId(this));
    tag->putInt("ChunkX", chunkX);
    tag->putInt("ChunkZ", chunkZ);
    tag->put("BB", boundingBox->createTag("BB"));

    ListTag<CompoundTag>* childrenTags = new ListTag<CompoundTag>("Children");
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        StructurePiece* piece = *it;
        childrenTags->add(piece->createTag());
    }
    tag->put("Children", childrenTags);

    addAdditonalSaveData(tag);

    return tag;
}

void StructureStart::addAdditonalSaveData(CompoundTag* tag) {}

void StructureStart::load(Level* level, CompoundTag* tag) {
    chunkX = tag->getInt("ChunkX");
    chunkZ = tag->getInt("ChunkZ");
    if (tag->contains("BB")) {
        boundingBox = new BoundingBox(tag->getIntArray("BB"));
    }

    ListTag<CompoundTag>* children =
        (ListTag<CompoundTag>*)tag->getList("Children");
    for (int i = 0; i < children->size(); i++) {
        pieces.push_back(
            StructureFeatureIO::loadStaticPiece(children->get(i), level));
    }

    readAdditonalSaveData(tag);
}

void StructureStart::readAdditonalSaveData(CompoundTag* tag) {}

void StructureStart::moveBelowSeaLevel(Level* level, Random* random,
                                       int offset) {
    const int MAX_Y = level->seaLevel - offset;

    // set lowest possible position (at bedrock)
    int y1Pos = boundingBox->getYSpan() + 1;
    // move up randomly within the available span
    if (y1Pos < MAX_Y) {
        y1Pos += random->nextInt(MAX_Y - y1Pos);
    }

    // move all bounding boxes
    int dy = y1Pos - boundingBox->y1;
    boundingBox->move(0, dy, 0);
    for (auto it = pieces.begin(); it != pieces.end(); it++) {
        StructurePiece* piece = *it;
        piece->getBoundingBox()->move(0, dy, 0);
    }
}

void StructureStart::moveInsideHeights(Level* level, Random* random,
                                       int lowestAllowed, int highestAllowed) {
    int heightSpan =
        highestAllowed - lowestAllowed + 1 - boundingBox->getYSpan();
    int y0Pos = 1;

    if (heightSpan > 1) {
        y0Pos = lowestAllowed + random->nextInt(heightSpan);
    } else {
        y0Pos = lowestAllowed;
    }

    // move all bounding boxes
    int dy = y0Pos - boundingBox->y0;
    boundingBox->move(0, dy, 0);
    for (auto it = pieces.begin(); it != pieces.end(); it++) {
        StructurePiece* piece = *it;
        piece->getBoundingBox()->move(0, dy, 0);
    }
}

bool StructureStart::isValid() { return true; }

int StructureStart::getChunkX() { return chunkX; }

int StructureStart::getChunkZ() { return chunkZ; }
