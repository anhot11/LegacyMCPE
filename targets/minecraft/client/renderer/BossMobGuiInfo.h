#pragma once

#include <memory>
#include <string>

class BossMob;

class BossMobGuiInfo {
public:
    static float healthProgress;
    static int displayTicks;
    static std::string name;
    static bool darkenWorld;

    static void setBossHealth(std::shared_ptr<BossMob> boss, bool darkenWorld);
};