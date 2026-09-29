#pragma once

#include "Control.h"

class Mob;

class JumpControl : public Control {
private:
    Mob* mob;
    bool _jump;

public:
    JumpControl(Mob* mob);
    virtual ~JumpControl() {}

    void jump();
    // genuinly, why tf is this VIRTUAL
    virtual void tick();
};
