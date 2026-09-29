#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <unordered_map>
#include <vector>

#include "java/File.h"

class DataInputStream;

class ArchiveFile {
protected:
    File m_sourcefile;
    std::uint8_t* m_cachedData;

    typedef struct _MetaData {
        std::string filename;
        int ptr;
        int filesize;
        bool isCompressed;

    } MetaData, *PMetaData;

    std::unordered_map<std::string, PMetaData> m_index;

public:
    void _readHeader(DataInputStream* dis);

    ArchiveFile(File file);
    ~ArchiveFile();

    std::vector<std::string>* getFileList();
    bool hasFile(const std::string& filename);
    int getFileSize(const std::string& filename);
    std::vector<uint8_t> getFile(const std::string& filename);
};
