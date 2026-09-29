#include "McRegionLevelStorageSource.h"

#include <assert.h>

#include <memory>

#include "LevelData.h"
#include "McRegionLevelStorage.h"
#include "java/File.h"
#include "java/JavaMath.h"
#include "minecraft/util/ProgressListener.h"
#include "minecraft/world/level/storage/DirectoryLevelStorageSource.h"

McRegionLevelStorageSource::McRegionLevelStorageSource(File dir)
    : DirectoryLevelStorageSource(dir) {}

std::string McRegionLevelStorageSource::getName() {
    return "Scaevolus' McRegion";
}

std::vector<LevelSummary*>* McRegionLevelStorageSource::getLevelList() {
    // 4J Stu - We don't need to do directory lookups with the xbox save files
    std::vector<LevelSummary*>* levels = new std::vector<LevelSummary*>;
    return levels;
}

void McRegionLevelStorageSource::clearAll() {}

std::shared_ptr<LevelStorage> McRegionLevelStorageSource::selectLevel(
    ConsoleSaveFile* saveFile, const std::string& levelId,
    bool createPlayerDir) {
    //        return new LevelStorageProfilerDecorator(new
    //        McRegionLevelStorage(baseDir, levelId, createPlayerDir));
    return std::shared_ptr<LevelStorage>(
        new McRegionLevelStorage(saveFile, baseDir, levelId, createPlayerDir));
}

bool McRegionLevelStorageSource::isConvertible(ConsoleSaveFile* saveFile,
                                               const std::string& levelId) {
    // check if there is old file format level data
    LevelData* levelData = getDataTagFor(saveFile, levelId);
    if (levelData == nullptr || levelData->getVersion() != 0) {
        delete levelData;
        return false;
    }
    delete levelData;

    return true;
}

bool McRegionLevelStorageSource::requiresConversion(
    ConsoleSaveFile* saveFile, const std::string& levelId) {
    LevelData* levelData = getDataTagFor(saveFile, levelId);
    if (levelData == nullptr || levelData->getVersion() != 0) {
        delete levelData;
        return false;
    }
    delete levelData;

    return true;
}

bool McRegionLevelStorageSource::convertLevel(ConsoleSaveFile* saveFile,
                                              const std::string& levelId,
                                              ProgressListener* progress) {
    assert(false);
    // I removed this while updating the saves to use the single save file
    // Will we ever use this convertLevel function anyway? The main issue is the
    // check for the hellFolder.exists() which would require a slight change to
    // the way our save files are structured
    return true;
}

void McRegionLevelStorageSource::convertRegions(
    File& baseFolder, std::vector<ChunkFile*>* chunkFiles, int currentCount,
    int totalCount, ProgressListener* progress) {
    assert(false);

    // 4J Stu - Removed, see comment in convertLevel above
}

void McRegionLevelStorageSource::eraseFolders(std::vector<File*>* folders,
                                              int currentCount, int totalCount,
                                              ProgressListener* progress) {
    File* folder;
    auto itEnd = folders->end();
    for (auto it = folders->begin(); it != itEnd; it++) {
        folder = *it;  // folders->at(i);

        std::vector<File*>* files = folder->listFiles();
        deleteRecursive(files);
        folder->_delete();

        currentCount++;
        int percent =
            (int)Math::round(100.0 * (double)currentCount / (double)totalCount);
        progress->progressStagePercentage(percent);
    }
}
