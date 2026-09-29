#pragma once
#include <string>

#include "TileEntity.h"
#include "java/Class.h"

class Random;

class EnchantmentTableEntity : public TileEntity {
public:
    eINSTANCEOF GetType() { return eTYPE_ENCHANTMENTTABLEENTITY; }
    static TileEntity* create() { return new EnchantmentTableEntity(); }

public:
    int time;
    float flip, oFlip, flipT, flipA;
    float open, oOpen;
    float rot, oRot, tRot;

private:
    Random* random;
    std::string name;

public:
    EnchantmentTableEntity();
    ~EnchantmentTableEntity();

    virtual void save(CompoundTag* base);
    virtual void load(CompoundTag* base);
    virtual void tick();
    virtual std::string getName();
    virtual std::string getCustomName();
    virtual bool hasCustomName();
    virtual void setCustomName(const std::string& name);

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();
};