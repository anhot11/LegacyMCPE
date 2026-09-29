#pragma once
class Player;
class LocalPlayer;

class Input {
public:
    float xa;
    float ya;

    bool wasJumping;
    bool jumping;
    bool sneaking;
    bool sprintKey;

    Input();  // 4J - added
    virtual ~Input() {}

    virtual void tick(LocalPlayer* player);

private:
    bool lReset;
    bool rReset;
};
