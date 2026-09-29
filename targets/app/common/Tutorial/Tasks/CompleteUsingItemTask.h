#pragma once
// using namespace std;

#include "TutorialTask.h"

class Level;
class Tutorial;

class CompleteUsingItemTask : public TutorialTask {
private:
    int* m_iValidItemsA;
    int m_iValidItemsCount;
    bool completed;

public:
    CompleteUsingItemTask(Tutorial* tutorial, int descriptionId, int itemIds[],
                          unsigned int itemIdsLength,
                          bool enablePreCompletion = false);
    virtual ~CompleteUsingItemTask();
    virtual bool isCompleted();
    virtual void completeUsingItem(std::shared_ptr<ItemInstance> item);
};