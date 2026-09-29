#include "BlazeModel.h"

#include <math.h>

#include <memory>
#include <numbers>

#include "minecraft/client/model/geom/Model.h"
#include "minecraft/client/model/geom/ModelPart.h"

BlazeModel::BlazeModel() : Model() {
    upperBodyParts = std::vector<ModelPart*>(12);

    for (unsigned int i = 0; i < upperBodyParts.size(); i++) {
        upperBodyParts[i] = new ModelPart(this, 0, 16);
        upperBodyParts[i]->addBox(0, 0, 0, 2, 8, 2);
    }

    head = new ModelPart(this, 0, 0);
    head->addBox(-4, -4, -4, 8, 8, 8);

    // 4J added - compile now to avoid random performance hit first time cubes
    // are rendered 4J Stu - Not just performance, but alpha+depth tests don't
    // work right unless we compile here
    for (unsigned int i = 0; i < upperBodyParts.size(); i++) {
        upperBodyParts[i]->compile(1.0f / 16.0f);
    }
    head->compile(1.0f / 16.0f);
}

int BlazeModel::modelVersion() { return 8; }

void BlazeModel::render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled) {
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    head->render(scale, usecompiled);
    for (unsigned int i = 0; i < upperBodyParts.size(); i++) {
        upperBodyParts[i]->render(scale, usecompiled);
    }
}

void BlazeModel::setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity,
                           unsigned int uiBitmaskOverrideAnim) {
    float angle = bob * std::numbers::pi * -.1f;
    for (int i = 0; i < 4; i++) {
        upperBodyParts[i]->y = -2 + cosf((i * 2 + bob) * .25f);
        upperBodyParts[i]->x = cosf(angle) * 9.0f;
        upperBodyParts[i]->z = sinf(angle) * 9.0f;
        angle += std::numbers::pi * 0.5f;
    }
    angle = .25f * std::numbers::pi + bob * std::numbers::pi * .03f;
    for (int i = 4; i < 8; i++) {
        upperBodyParts[i]->y = 2 + cosf((i * 2 + bob) * .25f);
        upperBodyParts[i]->x = cosf(angle) * 7.0f;
        upperBodyParts[i]->z = sinf(angle) * 7.0f;
        angle += std::numbers::pi * 0.5f;
    }

    angle = .15f * std::numbers::pi + bob * std::numbers::pi * -.05f;
    for (int i = 8; i < 12; i++) {
        upperBodyParts[i]->y = 11 + cosf((i * 1.5f + bob) * .5f);
        upperBodyParts[i]->x = cosf(angle) * 5.0f;
        upperBodyParts[i]->z = sinf(angle) * 5.0f;
        angle += std::numbers::pi * 0.5f;
    }

    head->yRot = yRot / (float)(180 / std::numbers::pi);
    head->xRot = xRot / (float)(180 / std::numbers::pi);
}
