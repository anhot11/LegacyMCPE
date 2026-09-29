#pragma once

#include <memory>
#include <string>
#include <vector>

class LevelSummary;
class ProgressListener;
class LevelData;
class LevelStorage;
class ConsoleSaveFile;

class LevelStorageSource {
public:
    virtual std::string getName() = 0;
    virtual std::shared_ptr<LevelStorage> selectLevel(
        ConsoleSaveFile* saveFile, const std::string& levelId,
        bool createPlayerDir) = 0;
    virtual std::vector<LevelSummary*>* getLevelList() = 0;
    virtual void clearAll() = 0;
    virtual LevelData* getDataTagFor(ConsoleSaveFile* saveFile,
                                     const std::string& levelId) = 0;

    /**
     * Tests if a levelId can be used to store a level. For example, a levelId
     * can't be called COM1 on Windows systems, because that is a reserved file
     * handle.
     * <p>
     * Also, a new levelId may not overwrite an existing one.
     *
     * @param levelId
     * @return
     */
    virtual bool isNewLevelIdAcceptable(const std::string& levelId) = 0;
    virtual void deleteLevel(const std::string& levelId) = 0;
    virtual void renameLevel(const std::string& levelId,
                             const std::string& newLevelName) = 0;
    virtual bool isConvertible(ConsoleSaveFile* saveFile,
                               const std::string& levelId) = 0;
    virtual bool requiresConversion(ConsoleSaveFile* saveFile,
                                    const std::string& levelId) = 0;
    virtual bool convertLevel(ConsoleSaveFile* saveFile,
                              const std::string& levelId,
                              ProgressListener* progress) = 0;
};