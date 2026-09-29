#pragma once

#include <cstdint>
#include <mutex>

#include "util/Timer.h"

class SaveManager {
public:
    SaveManager() : m_uiAutosaveTimer{}, m_saveNotificationDepth(0) {}

    void setAutosaveTimerTime(int settingValue);
    bool autosaveDue() const;
    int64_t secondsToAutosave() const;

    void lock();
    void unlock();

private:
    time_util::time_point m_uiAutosaveTimer;
    std::mutex m_saveNotificationMutex;
    int m_saveNotificationDepth;
};
