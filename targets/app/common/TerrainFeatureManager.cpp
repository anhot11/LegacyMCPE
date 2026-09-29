#include "app/common/TerrainFeatureManager.h"

void TerrainFeatureManager::add(_eTerrainFeatureType eFeatureType, int x,
                                int z) {
    // check we don't already have this in
    for (auto it = m_vTerrainFeatures.begin(); it < m_vTerrainFeatures.end();
         ++it) {
        FEATURE_DATA* pFeatureData = *it;

        if ((pFeatureData->eTerrainFeature == eFeatureType) &&
            (pFeatureData->x == x) && (pFeatureData->z == z))
            return;
    }

    FEATURE_DATA* pFeatureData = new FEATURE_DATA;
    pFeatureData->eTerrainFeature = eFeatureType;
    pFeatureData->x = x;
    pFeatureData->z = z;

    m_vTerrainFeatures.push_back(pFeatureData);
}

_eTerrainFeatureType TerrainFeatureManager::isFeature(int x, int z) const {
    for (auto it = m_vTerrainFeatures.begin(); it < m_vTerrainFeatures.end();
         ++it) {
        FEATURE_DATA* pFeatureData = *it;

        if ((pFeatureData->x == x) && (pFeatureData->z == z))
            return pFeatureData->eTerrainFeature;
    }

    return eTerrainFeature_None;
}

bool TerrainFeatureManager::getPosition(_eTerrainFeatureType eType, int* pX,
                                        int* pZ) const {
    for (auto it = m_vTerrainFeatures.begin(); it < m_vTerrainFeatures.end();
         ++it) {
        FEATURE_DATA* pFeatureData = *it;

        if (pFeatureData->eTerrainFeature == eType) {
            *pX = pFeatureData->x;
            *pZ = pFeatureData->z;
            return true;
        }
    }

    return false;
}

void TerrainFeatureManager::clear() {
    FEATURE_DATA* pFeatureData;
    while (m_vTerrainFeatures.size() > 0) {
        pFeatureData = m_vTerrainFeatures.back();
        m_vTerrainFeatures.pop_back();
        delete pFeatureData;
    }
}
