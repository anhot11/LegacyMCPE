#pragma once

#include <format>
#include <vector>

#include "TutorialTask.h"

class Level;
class Tutorial;
class TutorialConstraint;

// 4J-JEV: Tasks that involve riding an entity.
class RideEntityTask : public TutorialTask {
protected:
    const int m_eType;

public:
    RideEntityTask(const int eTYPE, Tutorial* tutorial, int descriptionId,
                   bool enablePreCompletion = false,
                   std::vector<TutorialConstraint*>* inConstraints = nullptr,
                   bool bShowMinimumTime = false, bool bAllowFade = true,
                   bool bTaskReminders = true);

    virtual bool isCompleted();

    virtual void onRideEntity(std::shared_ptr<Entity> entity);
};