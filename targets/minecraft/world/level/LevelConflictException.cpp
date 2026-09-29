#include "LevelConflictException.h"

LevelConflictException::LevelConflictException(const std::string& msg)
    : RuntimeException(msg) {}