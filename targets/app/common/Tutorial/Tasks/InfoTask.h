#pragma once
// using namespace std;

#include <unordered_map>

#include "TutorialTask.h"

class Tutorial;

// Information messages
class InfoTask : public TutorialTask {
private:
    std::unordered_map<int, bool> completedMappings;

    bool CompletionMaskIsValid();

public:
    InfoTask(Tutorial* tutorial, int descriptionId, int promptId = -1,
             bool requiresUserInput = false, int iMapping = 0);
    virtual bool isCompleted();
    virtual int getPromptId();
    virtual void setAsCurrentTask(bool active = true);
    virtual void handleUIInput(int iAction);
};