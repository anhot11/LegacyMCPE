#include "java/InputOutputStream/InputStream.h"

#include <string>

#include "java/File.h"
#include "java/InputOutputStream/FileInputStream.h"

InputStream* InputStream::getResourceAsStream(const std::string& fileName) {
    File file(fileName);
    return file.exists() ? new FileInputStream(file) : nullptr;
}