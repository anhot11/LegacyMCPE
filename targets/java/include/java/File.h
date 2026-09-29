#pragma once

#include <cstdint>
#include <string>
#include <vector>

// 4J Stu - Represents java standard library class

class FileFilter;

class File {
public:
    // The system-dependent path-separator character
    static const char pathSeparator;

    // 4J Jev, the start of the file root
    static const std::string pathRoot;

    File() { m_abstractPathName = ""; }

    File(const File& parent, const std::string& child);
    File(const std::string& pathname);
    File(const std::string& parent, const std::string& child);
    bool _delete();
    bool mkdir() const;
    bool mkdirs() const;
    bool exists() const;
    bool isFile() const;
    bool renameTo(File dest);
    std::vector<File*>* listFiles() const;  // Array
    std::vector<File*>* listFiles(FileFilter* filter) const;
    bool isDirectory() const;
    int64_t length();
    int64_t lastModified();
    const std::string getPath() const;  // 4J Jev: TODO
    std::string getName() const;

    static int hash_fnct(const File& k);
    static bool eq_test(const File& x, const File& y);

private:
    void _init();
    std::string m_abstractPathName;

    // 4J Jev, just helper functions, change between paths and
    // std::vector<string>
    // File(std::vector<std::string> *path);
};

struct FileKeyHash {
    int operator()(const File& k) const { return File::hash_fnct(k); }
};

struct FileKeyEq {
    bool operator()(const File& x, const File& y) const {
        return File::eq_test(x, y);
    }
};