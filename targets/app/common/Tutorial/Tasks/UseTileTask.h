#pragma once
// using namespace std;

#include <format>
#include <vector>

#include "TutorialTask.h"

class Level;
class Tutorial;
class TutorialConstraint;

// 4J Stu - Tasks that involve using a tile, with or without an item. e.g.
// Opening a chest
class UseTileTask : public TutorialTask {
private:
    int x, y, z;
    const int tileId;
    bool useLocation;
    bool completed;

public:
    UseTileTask(const int tileId, int x, int y, int z, Tutorial* tutorial,
                int descriptionId, bool enablePreCompletion = false,
                std::vector<TutorialConstraint*>* inConstraints = nullptr,
                bool bShowMinimumTime = false, bool bAllowFade = true,
                bool bTaskReminders = true);
    UseTileTask(const int tileId, Tutorial* tutorial, int descriptionId,
                bool enablePreCompletion = false,
                std::vector<TutorialConstraint*>* inConstraints = nullptr,
                bool bShowMinimumTime = false, bool bAllowFade = true,
                bool bTaskReminders = true);
    virtual bool isCompleted();
    virtual void useItemOn(Level* level, std::shared_ptr<ItemInstance> item,
                           int x, int y, int z, bool bTestUseOnly = false);
};