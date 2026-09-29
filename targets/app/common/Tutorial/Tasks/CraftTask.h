#pragma once
#include <format>
#include <vector>

#include "TutorialTask.h"

class Tutorial;
class TutorialConstraint;

class CraftTask : public TutorialTask {
public:
    CraftTask(int itemId, int auxValue, int quantity, Tutorial* tutorial,
              int descriptionId, bool enablePreCompletion = true,
              std::vector<TutorialConstraint*>* inConstraints = nullptr,
              bool bShowMinimumTime = false, bool bAllowFade = true,
              bool m_bTaskReminders = true);
    CraftTask(int* items, int* auxValues, int numItems, int quantity,
              Tutorial* tutorial, int descriptionId,
              bool enablePreCompletion = true,
              std::vector<TutorialConstraint*>* inConstraints = nullptr,
              bool bShowMinimumTime = false, bool bAllowFade = true,
              bool m_bTaskReminders = true);

    ~CraftTask();

    virtual bool isCompleted() { return bIsCompleted; }
    virtual void onCrafted(std::shared_ptr<ItemInstance> item);

private:
    int* m_items;
    int* m_auxValues;
    int m_numItems;
    int m_quantity;
    int m_count;
};