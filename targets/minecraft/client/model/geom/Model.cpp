#include "Model.h"

#include "TexOffs.h"

Model::Model() {
    riding = false;
    young = true;
    texWidth = 64;
    texHeight = 32;
}

void Model::setMapTex(std::string id, int x, int y) {
    mappedTexOffs[id] = new TexOffs(x, y);
}

TexOffs* Model::getMapTex(std::string id) {
    // 4J-PB - assuming there will always be this one
    return mappedTexOffs[id];
}