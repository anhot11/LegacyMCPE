#include "BookModel.h"

#include <math.h>

#include <memory>
#include <numbers>

#include "minecraft/client/model/geom/ModelPart.h"

BookModel::BookModel() {
    leftLid = (new ModelPart(this))->texOffs(0, 0)->addBox(-6, -5, 0, 6, 10, 0);
    rightLid =
        (new ModelPart(this))->texOffs(16, 0)->addBox(0, -5, 0, 6, 10, 0);

    seam = (new ModelPart(this))->texOffs(12, 0)->addBox(-1, -5, 0, 2, 10, 0);

    // 4J - added faceMasks here to remove sides of these page boxes which end
    // up being nearly coplanar to the cover of the book and flickering when
    // rendering at a distance
    leftPages = (new ModelPart(this))
                    ->texOffs(0, 10)
                    ->addBoxWithMask(0, -4, -1 + 0.01f, 5, 8, 1,
                                     47);  // 4J - faceMask is binary 101111
    rightPages = (new ModelPart(this))
                     ->texOffs(12, 10)
                     ->addBoxWithMask(0, -4, -0.01f, 5, 8, 1,
                                      31);  // 4J - faceMask is binary 011111

    flipPage1 =
        (new ModelPart(this))->texOffs(24, 10)->addBox(0, -4, 0, 5, 8, 0);
    flipPage2 =
        (new ModelPart(this))->texOffs(24, 10)->addBox(0, -4, 0, 5, 8, 0);

    leftLid->setPos(0, 0, -1);
    rightLid->setPos(0, 0, 1);

    seam->yRot = std::numbers::pi / 2;

    // 4J added - compile now to avoid random performance hit first time cubes
    // are rendered
    leftLid->compile(1.0f / 16.0f);
    rightLid->compile(1.0f / 16.0f);
    seam->compile(1.0f / 16.0f);
    leftPages->compile(1.0f / 16.0f);
    rightPages->compile(1.0f / 16.0f);
    flipPage1->compile(1.0f / 16.0f);
    flipPage2->compile(1.0f / 16.0f);
}

void BookModel::render(std::shared_ptr<Entity> entity, float time, float r,
                       float bob, float yRot, float xRot, float scale,
                       bool usecompiled) {
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    leftLid->render(scale, usecompiled);
    rightLid->render(scale, usecompiled);
    seam->render(scale, usecompiled);

    leftPages->render(scale, usecompiled);
    rightPages->render(scale, usecompiled);

    flipPage1->render(scale, usecompiled);
    flipPage2->render(scale, usecompiled);
}

void BookModel::setupAnim(float time, float r, float bob, float yRot,
                          float xRot, float scale,
                          std::shared_ptr<Entity> entity,
                          unsigned int uiBitmaskOverrideAnim) {
    float openness = (sinf(time * 0.02f) * 0.10f + 1.25f) * yRot;

    leftLid->yRot = std::numbers::pi + openness;
    rightLid->yRot = -openness;
    leftPages->yRot = +openness;
    rightPages->yRot = -openness;

    flipPage1->yRot = +openness - openness * 2 * r;
    flipPage2->yRot = +openness - openness * 2 * bob;

    leftPages->x = sinf(openness);
    rightPages->x = sinf(openness);
    flipPage1->x = sinf(openness);
    flipPage2->x = sinf(openness);
}
