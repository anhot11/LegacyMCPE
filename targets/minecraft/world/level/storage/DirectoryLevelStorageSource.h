#pragma once

#include <format>
#include <string>
#include <vector>

#include "LevelStorageSource.h"
#include "java/File.h"

class ProgressListener;
class LevelData;
class ConsoleSaveFile;

class DirectoryLevelStorageSource : public LevelStorageSource {
protected:
    const File baseDir;

public:
    DirectoryLevelStorageSource(const File dir);
    virtual std::string getName();
    virtual std::vector<LevelSummary*>* getLevelList();
    virtual void clearAll();
    virtual LevelData* getDataTagFor(ConsoleSaveFile* saveFile,
                                     const std::string& levelId);
    virtual void renameLevel(const std::string& levelId,
                             const std::string& newLevelName);
    virtual bool isNewLevelIdAcceptable(const std::string& levelId);
    virtual void deleteLevel(const std::string& levelId);

protected:
    static void deleteRecursive(std::vector<File*>* files);

public:
    virtual std::shared_ptr<LevelStorage> selectLevel(
        ConsoleSaveFile* saveFile, const std::string& levelId,
        bool createPlayerDir);
    virtual bool isConvertible(ConsoleSaveFile* saveFile,
                               const std::string& levelId);
    virtual bool requiresConversion(ConsoleSaveFile* saveFile,
                                    const std::string& levelId);
    virtual bool convertLevel(ConsoleSaveFile* saveFile,
                              const std::string& levelId,
                              ProgressListener* progress);
};