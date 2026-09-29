#include "BatModel.h"

#include <cmath>
#include <memory>
#include <numbers>

#include "minecraft/client/model/geom/Model.h"
#include "minecraft/client/model/geom/ModelPart.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/ambient/Bat.h"

BatModel::BatModel() : Model() {
    texWidth = 64;
    texHeight = 64;

    head = new ModelPart(this, 0, 0);
    head->addBox(-3, -3, -3, 6, 6, 6);

    ModelPart* rightEar = new ModelPart(this, 24, 0);
    rightEar->addBox(-4, -6, -2, 3, 4, 1);
    head->addChild(rightEar);
    ModelPart* leftEar = new ModelPart(this, 24, 0);
    leftEar->bMirror = true;
    leftEar->addBox(1, -6, -2, 3, 4, 1);
    head->addChild(leftEar);

    body = new ModelPart(this, 0, 16);
    body->addBox(-3, 4, -3, 6, 12, 6);
    body->texOffs(0, 34)->addBox(-5, 16, 0, 10, 6, 1);

    rightWing = new ModelPart(this, 42, 0);
    rightWing->addBox(-12, 1, 1.5f, 10, 16, 1);
    rightWingTip = new ModelPart(this, 24, 16);
    rightWingTip->setPos(-12, 1, 1.5f);
    rightWingTip->addBox(-8, 1, 0, 8, 12, 1);

    leftWing = new ModelPart(this, 42, 0);
    leftWing->bMirror = true;
    leftWing->addBox(2, 1, 1.5f, 10, 16, 1);
    leftWingTip = new ModelPart(this, 24, 16);
    leftWingTip->bMirror = true;
    leftWingTip->setPos(12, 1, 1.5f);
    leftWingTip->addBox(0, 1, 0, 8, 12, 1);

    body->addChild(rightWing);
    body->addChild(leftWing);
    rightWing->addChild(rightWingTip);
    leftWing->addChild(leftWingTip);

    // 4J added - compile now to avoid random performance hit first time cubes
    // are rendered 4J Stu - Not just performance, but alpha+depth tests don't
    // work right unless we compile here
    head->compile(1.0f / 16.0f);
    body->compile(1.0f / 16.0f);
    rightWing->compile(1.0f / 16.0f);
    leftWing->compile(1.0f / 16.0f);
    rightWingTip->compile(1.0f / 16.0f);
    leftWingTip->compile(1.0f / 16.0f);
    rightEar->compile(1.0f / 16.0f);
    leftEar->compile(1.0f / 16.0f);
}

int BatModel::modelVersion() { return 36; }

void BatModel::render(std::shared_ptr<Entity> entity, float time, float r,
                      float bob, float yRot, float xRot, float scale,
                      bool usecompiled) {
    std::shared_ptr<Bat> bat = std::dynamic_pointer_cast<Bat>(entity);
    if (bat->isResting()) {
        float rad = 180 / std::numbers::pi;
        head->xRot = xRot / rad;
        head->yRot = std::numbers::pi - yRot / rad;
        head->zRot = std::numbers::pi;

        head->setPos(0, -2, 0);
        rightWing->setPos(-3, 0, 3);
        leftWing->setPos(3, 0, 3);

        body->xRot = std::numbers::pi;

        rightWing->xRot = -std::numbers::pi * .05f;
        rightWing->yRot = -std::numbers::pi * .40f;
        rightWingTip->yRot = -std::numbers::pi * .55f;
        leftWing->xRot = rightWing->xRot;
        leftWing->yRot = -rightWing->yRot;
        leftWingTip->yRot = -rightWingTip->yRot;
    } else {
        float rad = 180 / std::numbers::pi;
        head->xRot = xRot / rad;
        head->yRot = yRot / rad;
        head->zRot = 0;

        head->setPos(0, 0, 0);
        rightWing->setPos(0, 0, 0);
        leftWing->setPos(0, 0, 0);

        body->xRot = std::numbers::pi * .25f + cos(bob * .1f) * .15f;
        body->yRot = 0;

        rightWing->yRot = cos(bob * 1.3f) * std::numbers::pi * .25f;
        leftWing->yRot = -rightWing->yRot;
        rightWingTip->yRot = rightWing->yRot * .5f;
        leftWingTip->yRot = -rightWing->yRot * .5f;
    }

    head->render(scale, usecompiled);
    body->render(scale, usecompiled);
}