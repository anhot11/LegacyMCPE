#pragma once

#include <memory>

#include "Throwable.h"
#include "java/Class.h"

class HitResult;
class Level;
class LivingEntity;

class ThrownEgg : public Throwable {
public:
    eINSTANCEOF GetType() { return eTYPE_THROWNEGG; }

private:
    void _init();

public:
    ThrownEgg(Level* level);
    ThrownEgg(Level* level, std::shared_ptr<LivingEntity> mob);
    ThrownEgg(Level* level, double x, double y, double z);

protected:
    virtual void onHit(HitResult* res);
};
