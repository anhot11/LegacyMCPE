
#include "SavedData.h"

#include <string>

SavedData::SavedData(const std::string& id) : id(id) { dirty = false; }

void SavedData::setDirty() { setDirty(true); }

void SavedData::setDirty(bool dirty) { this->dirty = dirty; }

bool SavedData::isDirty() { return dirty; }