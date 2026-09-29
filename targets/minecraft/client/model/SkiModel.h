#pragma once
#include <memory>
#include <vector>

#include "minecraft/client/model/geom/Model.h"
#include "minecraft/client/model/geom/ModelPart.h"

class Entity;
class ModelPart;

class SkiModel : public Model {
public:
    std::vector<ModelPart*> cubes;

private:
    bool leftSki;

public:
    SkiModel();  // 4J added
    SkiModel(bool leftSki);
    void _init(bool leftSki);  // 4J added
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity);
};