#include "TutorialTask.h"

#include <compare>

#include "app/common/Tutorial/Constraints/TutorialConstraint.h"
#include "app/common/Tutorial/Tutorial.h"

TutorialTask::TutorialTask(Tutorial* tutorial, int descriptionId,
                           bool enablePreCompletion,
                           std::vector<TutorialConstraint*>* inConstraints,
                           bool bShowMinimumTime, bool bAllowFade,
                           bool bTaskReminders)
    : tutorial(tutorial),
      descriptionId(descriptionId),
      m_promptId(-1),
      enablePreCompletion(enablePreCompletion),
      areConstraintsEnabled(false),
      bIsCompleted(false),
      bHasBeenActivated(false),
      m_bAllowFade(bAllowFade),
      m_bTaskReminders(bTaskReminders),
      m_bShowMinimumTime(bShowMinimumTime),
      m_bShownForMinimumTime(false) {
    if (inConstraints != nullptr) {
        for (auto it = inConstraints->begin(); it < inConstraints->end();
             ++it) {
            TutorialConstraint* constraint = *it;
            constraints.push_back(constraint);
        }
        delete inConstraints;
    }

    tutorial->addMessage(descriptionId);
}

TutorialTask::~TutorialTask() {
    enableConstraints(false);

    for (auto it = constraints.begin(); it < constraints.end(); ++it) {
        TutorialConstraint* constraint = *it;

        if (constraint->getQueuedForRemoval()) {
            constraint->setDeleteOnDeactivate(true);
        } else {
            delete constraint;
        }
    }
}

void TutorialTask::taskCompleted() {
    if (areConstraintsEnabled == true) enableConstraints(false);
}

void TutorialTask::enableConstraints(bool enable,
                                     bool delayRemove /*= false*/) {
    if (!enable && (areConstraintsEnabled || !delayRemove)) {
        // Remove
        for (auto it = constraints.begin(); it != constraints.end(); ++it) {
            TutorialConstraint* constraint = *it;
            // app.DebugPrintf(">>>>>>>> %i\n", constraints.size());
            tutorial->RemoveConstraint(constraint, delayRemove);
        }
        areConstraintsEnabled = false;
    } else if (!areConstraintsEnabled && enable) {
        // Add
        for (auto it = constraints.begin(); it != constraints.end(); ++it) {
            TutorialConstraint* constraint = *it;
            tutorial->AddConstraint(constraint);
        }
        areConstraintsEnabled = true;
    }
}

void TutorialTask::setAsCurrentTask(bool active /*= true*/) {
    bHasBeenActivated = active;
}