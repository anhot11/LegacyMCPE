#pragma once

class Position {
public:
    virtual ~Position() = default;
    virtual double getX() = 0;
    virtual double getY() = 0;
    virtual double getZ() = 0;
};
