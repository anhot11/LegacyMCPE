#pragma once

class BossMob {
public:
    virtual float getMaxHealth() = 0;
    virtual float getHealth() = 0;
    virtual std::string getAName() = 0;
};