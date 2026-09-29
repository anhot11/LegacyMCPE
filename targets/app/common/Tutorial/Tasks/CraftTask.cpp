#include "CraftTask.h"

#include <wchar.h>

#include <memory>
#include <string>

#include "app/common/Tutorial/Tasks/TutorialTask.h"
#include "minecraft/world/item/ItemInstance.h"

class Tutorial;
class TutorialConstraint;

CraftTask::CraftTask(
    int itemId, int auxValue, int quantity, Tutorial* tutorial,
    int descriptionId, bool enablePreCompletion /*= true*/,
    std::vector<TutorialConstraint*>* inConstraints /*= nullptr*/,
    bool bShowMinimumTime /*=false*/, bool bAllowFade /*=true*/,
    bool m_bTaskReminders /*=true*/)
    : TutorialTask(tutorial, descriptionId, enablePreCompletion, inConstraints,
                   bShowMinimumTime, bAllowFade, m_bTaskReminders),
      m_quantity(quantity),
      m_count(0) {
    m_numItems = 1;
    m_items = new int[1];
    m_items[0] = itemId;
    m_auxValues = new int[1];
    m_auxValues[0] = auxValue;
}

CraftTask::CraftTask(
    int* items, int* auxValues, int numItems, int quantity, Tutorial* tutorial,
    int descriptionId, bool enablePreCompletion /*= true*/,
    std::vector<TutorialConstraint*>* inConstraints /*= nullptr*/,
    bool bShowMinimumTime /*=false*/, bool bAllowFade /*=true*/,
    bool m_bTaskReminders /*=true*/)
    : TutorialTask(tutorial, descriptionId, enablePreCompletion, inConstraints,
                   bShowMinimumTime, bAllowFade, m_bTaskReminders),
      m_quantity(quantity),
      m_count(0) {
    m_numItems = numItems;
    m_items = new int[m_numItems];
    m_auxValues = new int[m_numItems];

    for (int i = 0; i < m_numItems; ++i) {
        m_items[i] = items[i];
        m_auxValues[i] = auxValues[i];
    }
}

CraftTask::~CraftTask() {
    delete[] m_items;
    delete[] m_auxValues;
}

void CraftTask::onCrafted(std::shared_ptr<ItemInstance> item) {
#ifndef _CONTENT_PACKAGE
    printf("CraftTask::onCrafted - %s\n", item->toString().c_str());
#endif
    bool itemFound = false;
    for (int i = 0; i < m_numItems; ++i) {
        if (m_items[i] == item->id &&
            (m_auxValues[i] == -1 || m_auxValues[i] == item->getAuxValue())) {
            itemFound = true;
            break;
        }
    }

    if (itemFound) {
        ++m_count;
    }
    if (m_count >= m_quantity) {
        bIsCompleted = true;
    }
}