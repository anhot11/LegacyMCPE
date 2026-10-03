#pragma once

#include <format>
#include <string>
#include <vector>

#include "DirectoryLevelStorageSource.h"
#include "java/File.h"
#include "java/FileFilter.h"
#include "java/FilenameFilter.h"

class ProgressListener;
class LevelStorage;

class McRegionLevelStorageSource : public DirectoryLevelStorageSource {
public:
    class ChunkFile;

    McRegionLevelStorageSource(File dir);
    virtual std::string getName() override;
    virtual std::vector<LevelSummary*>* getLevelList() override;
    virtual void clearAll() override;
    virtual std::shared_ptr<LevelStorage> selectLevel(
        ConsoleSaveFile* saveFile, const std::string& levelId,
        bool createPlayerDir) override;
    virtual bool isConvertible(ConsoleSaveFile* saveFile,
                               const std::string& levelId) override;
    virtual bool requiresConversion(ConsoleSaveFile* saveFile,
                                    const std::string& levelId) override;
    virtual bool convertLevel(ConsoleSaveFile* saveFile,
                              const std::string& levelId,
                              ProgressListener* progress) override;
    virtual void deleteLevel(const std::string& levelId) override;
    virtual void renameLevel(const std::string& levelId,
                             const std::string& newLevelName) override;

private:
    void convertRegions(File& baseFolder, std::vector<ChunkFile*>* chunkFiles,
                        int currentCount, int totalCount,
                        ProgressListener* progress);
    void eraseFolders(std::vector<File*>* folders, int currentCount,
                      int totalCount, ProgressListener* progress);

public:
};
