#include "minecraft/world/level/storage/ConsoleSaveFileIO/ConsoleSaveFileInputStream.h"

#include <assert.h>

#include <cstdint>
#include <vector>

#include "minecraft/world/level/storage/ConsoleSaveFileIO/ConsoleSaveFile.h"

class ConsoleSavePath;

ConsoleSaveFileInputStream::ConsoleSaveFileInputStream(
    ConsoleSaveFile* saveFile, const ConsoleSavePath& file) {
    m_saveFile = saveFile;
    m_file = m_saveFile->createFile(file);

    m_saveFile->setFilePointer(m_file, 0, SaveFileSeekOrigin::Begin);
}

ConsoleSaveFileInputStream::ConsoleSaveFileInputStream(
    ConsoleSaveFile* saveFile, FileEntry* file) {
    m_saveFile = saveFile;
    m_file = file;

    m_saveFile->setFilePointer(m_file, 0, SaveFileSeekOrigin::Begin);
}

// Reads a byte of data from this input stream. This method blocks if no input
// is yet available. Returns: the next byte of data, or -1 if the end of the
// file is reached.
int ConsoleSaveFileInputStream::read() {
    std::uint8_t byteRead = static_cast<std::uint8_t>(0);
    unsigned int numberOfBytesRead;

    bool result =
        m_saveFile->readFile(m_file,
                             &byteRead,          // data buffer
                             1,                  // number of bytes to read
                             &numberOfBytesRead  // number of bytes read
        );

    if (!result) {
        // TODO 4J Stu - Some kind of error handling
        return -1;
    } else if (numberOfBytesRead == 0) {
        // File pointer is past the end of the file
        return -1;
    }

    return static_cast<int>(byteRead);
}

// Reads up to b.size() bytes of data from this input stream into an array of
// bytes. This method blocks until some input is available. Parameters: b - the
// buffer into which the data is read. Returns: the total number of bytes read
// into the buffer, or -1 if there is no more data because the end of the file
// has been reached.
int ConsoleSaveFileInputStream::read(std::vector<uint8_t>& b) {
    unsigned int numberOfBytesRead;

    bool result =
        m_saveFile->readFile(m_file,
                             b.data(),           // data buffer
                             b.size(),           // number of bytes to read
                             &numberOfBytesRead  // number of bytes read
        );

    if (!result) {
        // TODO 4J Stu - Some kind of error handling
        return -1;
    } else if (numberOfBytesRead == 0) {
        // File pointer is past the end of the file
        return -1;
    }

    return numberOfBytesRead;
}

// Reads up to len bytes of data from this input stream into an array of bytes.
// If len is not zero, the method blocks until some input is available;
// otherwise, no bytes are read and 0 is returned. Parameters: b - the buffer
// into which the data is read. off - the start offset in the destination array
// b len - the maximum number of bytes read. Returns: the total number of bytes
// read into the buffer, or -1 if there is no more data because the end of the
// file has been reached.
int ConsoleSaveFileInputStream::read(std::vector<uint8_t>& b,
                                     unsigned int offset, unsigned int length) {
    // 4J Stu - We don't want to read any more than the array buffer can hold
    assert(length <= (b.size() - offset));

    unsigned int numberOfBytesRead;

    bool result =
        m_saveFile->readFile(m_file,
                             &b[offset],         // data buffer
                             length,             // number of bytes to read
                             &numberOfBytesRead  // number of bytes read
        );

    if (!result) {
        // TODO 4J Stu - Some kind of error handling
        return -1;
    } else if (numberOfBytesRead == 0) {
        // File pointer is past the end of the file
        return -1;
    }

    return numberOfBytesRead;
}

// Closes this file input stream and releases any system resources associated
// with the stream. If this stream has an associated channel then the channel is
// closed as well.
void ConsoleSaveFileInputStream::close() {
    if (m_saveFile != nullptr) {
        bool result = m_saveFile->closeHandle(m_file);

        if (!result) {
            // TODO 4J Stu - Some kind of error handling
        }

        // Stop the dtor from trying to close it again
        m_saveFile = nullptr;
    }
}
