#pragma once

#include <string>

class File;

// 4J Jev, java lirary interface.
class FilenameFilter {
public:
    virtual bool accept(File* dir, const std::string& name) = 0;
};