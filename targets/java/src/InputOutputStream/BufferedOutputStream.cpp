#include "java/InputOutputStream/BufferedOutputStream.h"

#include <stdio.h>

#include "java/InputOutputStream/OutputStream.h"

// Creates a new buffered output stream to write data to the specified
// underlying output stream with the specified buffer size. Parameters: out -
// the underlying output stream. size - the buffer size.
BufferedOutputStream::BufferedOutputStream(OutputStream* out, int size) {
    stream = out;
    buf = std::vector<uint8_t>(size);
    count = 0;
}

BufferedOutputStream::~BufferedOutputStream() {
    // 4jcraft, changed to [], deallocates internal buffer
    // TODO: ArrayWithLength.h doesnt have a destructor.
    // this wouldnt need to be done manually.
    // but for some reason the destructor is commented out in the source code?
}

// Flushes this buffered output stream. This forces any buffered output bytes to
// be written out to the underlying output stream.
void BufferedOutputStream::flush() {
    if (stream == nullptr) {
        fprintf(stderr,
                "BufferedOutputStream::flush() called but underlying stream is "
                "nullptr\n");
        return;
    }

    if (count > 0) {
        stream->write(buf, 0, count);
        count = 0;
    }
}

// Closes this output stream and releases any system resources associated with
// the stream. The close method of FilterOutputStream calls its flush method,
// and then calls the close method of its underlying output stream.
void BufferedOutputStream::close() {
    flush();
    if (stream == nullptr) {
        fprintf(stderr,
                "BufferedOutputStream::close() called but underlying stream is "
                "nullptr\n");
        return;
    }
    stream->close();
}

// Writes len bytes from the specified byte array starting at offset off to this
// buffered output stream. Ordinarily this method stores bytes from the given
// array into this stream's buffer, flushing the buffer to the underlying output
// stream as needed. If the requested length is at least as large as this
// stream's buffer, however, then this method will flush the buffer and write
// the bytes directly to the underlying output stream. Thus redundant
// BufferedOutputStreams will not copy data unnecessarily.
//
// Overrides:
// write in class FilterOutputStream
// Parameters:
// b - the data.
// off - the start offset in the data.
// len - the number of bytes to write.
void BufferedOutputStream::write(const std::vector<uint8_t>& b,
                                 unsigned int offset, unsigned int length) {
    // Over the length of what we can store in our buffer - just flush the
    // buffer and output directly
    if (length >= buf.size()) {
        flush();
        stream->write(b, offset, length);
    } else {
        for (unsigned int i = 0; i < length; i++) {
            write(static_cast<unsigned int>(b[offset + i]));
        }
    }
}

// Writes b.size() bytes to this output stream.
// The write method of FilterOutputStream calls its write method of three
// arguments with the arguments b, 0, and b.size().
//
// Note that this method does not call the one-argument write method of its
// underlying stream with the single argument b.
void BufferedOutputStream::write(const std::vector<uint8_t>& b) {
    write(b, 0, b.size());
}

// Writes the specified byte to this buffered output stream.
// Overrides:
// write in class FilterOutputStream
// Parameters:
// b - the byte to be written.
void BufferedOutputStream::write(unsigned int b) {
    buf[count++] = (uint8_t)b;
    if (count == buf.size()) {
        flush();
    }
}
