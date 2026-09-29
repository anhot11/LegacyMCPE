#pragma once

#include <memory>

#include "Control.h"

class Mob;
class Entity;

class LookControl : public Control {
private:
    Mob* mob;
    float yMax, xMax;
    bool hasWanted;

    double wantedX, wantedY, wantedZ;

public:
    LookControl(Mob* mob);
    virtual ~LookControl() {}

    void setLookAt(std::shared_ptr<Entity> target, float yMax, float xMax);
    void setLookAt(double x, double y, double z, float yMax, float xMax);
    virtual void tick();

private:
    float rotlerp(float a, float b, float max);

public:
    bool isHasWanted();
    float getYMax();
    float getXMax();
    double getWantedX();
    double getWantedY();
    double getWantedZ();
};
