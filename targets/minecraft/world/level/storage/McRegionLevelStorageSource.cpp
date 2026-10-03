#include "McRegionLevelStorageSource.h"

#include <assert.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <set>

#include "LevelData.h"
#include "LevelSummary.h"
#include "McRegionLevelStorage.h"
#include "java/File.h"
#include "java/JavaMath.h"
#include "minecraft/util/ProgressListener.h"
#include "minecraft/world/level/GameType.h"
#include "minecraft/world/level/storage/DirectoryLevelStorageSource.h"
#include "platform/fs/fs.h"

McRegionLevelStorageSource::McRegionLevelStorageSource(File dir)
    : DirectoryLevelStorageSource(dir) {}

std::string McRegionLevelStorageSource::getName() {
    return "Scaevolus' McRegion";
}

std::vector<LevelSummary*>* McRegionLevelStorageSource::getLevelList() {
    std::vector<LevelSummary*>* levels = new std::vector<LevelSummary*>;
    std::filesystem::path savesDir = PlatformFilesystem.getBasePath() / "saves";
    std::error_code ec;
    if (!std::filesystem::exists(savesDir, ec)) {
        std::filesystem::create_directories(savesDir, ec);
    }

    std::vector<std::filesystem::path> checkDirs = {
        savesDir,
        std::filesystem::path("/sdcard/LegacyMCPE/saves")
    };
    std::set<std::string> seen;

    for (const auto& dir : checkDirs) {
        if (!std::filesystem::exists(dir, ec)) continue;
        for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
            if (entry.is_directory()) {
                std::string folderName = entry.path().filename().string();
                if (seen.count(folderName)) continue;

                std::filesystem::path datFile = entry.path() / "savegame.dat";
                if (std::filesystem::exists(datFile, ec)) {
                    seen.insert(folderName);
                    std::string displayName = folderName;
                    int64_t lastPlayed = 0;
                    int64_t sizeOnDisk = (int64_t)std::filesystem::file_size(datFile, ec);

                    // Check info.txt
                    std::filesystem::path infoPath = entry.path() / "info.txt";
                    if (std::filesystem::exists(infoPath, ec)) {
                        std::ifstream inf(infoPath);
                        if (inf.is_open()) {
                            std::string line;
                            if (std::getline(inf, line) && !line.empty()) {
                                displayName = line;
                            }
                        }
                    }

                    levels->push_back(new LevelSummary(
                        folderName,
                        displayName,
                        lastPlayed,
                        sizeOnDisk,
                        GameType::SURVIVAL,
                        false,
                        false,
                        false
                    ));
                }
            }
        }
    }
    return levels;
}

void McRegionLevelStorageSource::deleteLevel(const std::string& levelId) {
    std::filesystem::path savesDir = PlatformFilesystem.getBasePath() / "saves" / levelId;
    std::error_code ec;
    std::filesystem::remove_all(savesDir, ec);

    std::filesystem::path altDir = std::filesystem::path("/sdcard/LegacyMCPE/saves") / levelId;
    std::filesystem::remove_all(altDir, ec);
}

void McRegionLevelStorageSource::renameLevel(const std::string& levelId,
                                             const std::string& newLevelName) {
    std::filesystem::path savesDir = PlatformFilesystem.getBasePath() / "saves" / levelId;
    std::error_code ec;
    if (std::filesystem::exists(savesDir, ec)) {
        std::ofstream inf(savesDir / "info.txt", std::ios::trunc);
        if (inf.is_open()) {
            inf << newLevelName << "\n";
            inf.close();
        }
    }
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
