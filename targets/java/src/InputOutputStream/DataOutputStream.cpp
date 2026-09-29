#include "java/InputOutputStream/DataOutputStream.h"

#include <stdio.h>

#include <bit>
#include <string>
#include <vector>

#include "java/InputOutputStream/OutputStream.h"

// Creates a new data output stream to write data to the specified underlying
// output stream. The counter written is set to zero. Parameters: out - the
// underlying output stream, to be saved for later use.
DataOutputStream::DataOutputStream(OutputStream* out)
    : stream(out), written(0) {}

// 4J Stu - We cannot always delete the stream when we are destroyed, but we
// want to clear it up as there are occasions when we don't have a handle to the
// child stream elsewhere and want to delete it
void DataOutputStream::deleteChildStream() { delete stream; }

// Writes the specified byte (the low eight bits of the argument b) to the
// underlying output stream. If no exception is thrown, the counter written is
// incremented by 1. Implements the write method of OutputStream. Parameters: b
// - the byte to be written.
void DataOutputStream::write(unsigned int b) {
    if (stream == nullptr) {
        fprintf(stderr,
                "DataOutputStream::write(unsigned int) called but underlying "
                "stream is nullptr\n");
        return;
    }
    stream->write(b);
    // TODO 4J Stu - Exception handling?
    written++;
}

void DataOutputStream::flush() {
    if (stream == nullptr) {
        fprintf(stderr,
                "DataOutputStream::flush() called but underlying stream is "
                "nullptr\n");
        return;
    }
    stream->flush();
}

// Writes b.size() bytes from the specified byte array to this output stream.
// The general contract for write(b) is that it should have exactly the same
// effect as the call write(b, 0, b.size()). Parameters: b - the data.
void DataOutputStream::write(const std::vector<uint8_t>& b) {
    write(b, 0, b.size());
}

// Writes len bytes from the specified byte array starting at offset off to the
// underlying output stream. If no exception is thrown, the counter written is
// incremented by len. Parameters: b - the data. off - the start offset in the
// data. len - the number of bytes to write.
void DataOutputStream::write(const std::vector<uint8_t>& b, unsigned int offset,
                             unsigned int length) {
    if (stream == nullptr) {
        fprintf(stderr,
                "DataOutputStream::write(std::vector<uint8_t>,...) called but "
                "underlying "
                "stream is nullptr\n");
        return;
    }
    stream->write(b, offset, length);
    // TODO 4J Stu - Some form of error checking?
    written += length;
}

// Closes this output stream and releases any system resources associated with
// the stream. The close method of FilterOutputStream calls its flush method,
// and then calls the close method of its underlying output stream.
void DataOutputStream::close() {
    if (stream == nullptr) {
        fprintf(stderr,
                "DataOutputStream::close() called but underlying stream is "
                "nullptr\n");
        return;
    }
    stream->close();
}

// Writes out a byte to the underlying output stream as a 1-byte value. If no
// exception is thrown, the counter written is incremented by 1. Parameters: v -
// a byte value to be written.
void DataOutputStream::writeByte(uint8_t a) {
    stream->write(static_cast<unsigned int>(a));
}

// Converts the double argument to a long using the doubleToLongBits method in
// class Double, and then writes that long value to the underlying output stream
// as an 8-byte quantity, high byte first. If no exception is thrown, the
// counter written is incremented by 8. Parameters: v - a double value to be
// written.
void DataOutputStream::writeDouble(double a) {
    int64_t bits = std::bit_cast<int64_t>(a);

    writeLong(bits);
    // TODO 4J Stu - Error handling?
    written += 8;
}

// Converts the float argument to an int using the floatToIntBits method in
// class Float, and then writes that int value to the underlying output stream
// as a 4-byte quantity, high byte first. If no exception is thrown, the counter
// written is incremented by 4. Parameters: v - a float value to be written.
void DataOutputStream::writeFloat(float a) {
    int bits = std::bit_cast<int>(a);

    writeInt(bits);
    // TODO 4J Stu - Error handling?
    written += 4;
}

// Writes an int to the underlying output stream as four bytes, high byte first.
// If no exception is thrown, the counter written is incremented by 4.
// Parameters:
// v - an int to be written.
void DataOutputStream::writeInt(int a) {
    stream->write((a >> 24) & 0xff);
    stream->write((a >> 16) & 0xff);
    stream->write((a >> 8) & 0xff);
    stream->write(a & 0xff);
    // TODO 4J Stu - Error handling?
    written += 4;
}

// Writes a long to the underlying output stream as eight bytes, high byte
// first. In no exception is thrown, the counter written is incremented by 8.
// Parameters:
// v - a long to be written.
void DataOutputStream::writeLong(int64_t a) {
    stream->write((a >> 56) & 0xff);
    stream->write((a >> 48) & 0xff);
    stream->write((a >> 40) & 0xff);
    stream->write((a >> 32) & 0xff);
    stream->write((a >> 24) & 0xff);
    stream->write((a >> 16) & 0xff);
    stream->write((a >> 8) & 0xff);
    stream->write(a & 0xff);
    // TODO 4J Stu - Error handling?
    written += 4;
}

// Writes a short to the underlying output stream as two bytes, high byte first.
// If no exception is thrown, the counter written is incremented by 2.
// Parameters:
// v - a short to be written.
void DataOutputStream::writeShort(short a) {
    stream->write((a >> 8) & 0xff);
    stream->write(a & 0xff);
    // TODO 4J Stu - Error handling?
    written += 2;
}

void DataOutputStream::writeUnsignedShort(unsigned short a) {
    if (stream == nullptr) {
        fprintf(
            stderr,
            "DataOutputStream::writeUnsignedShort() but underlying stream is "
            "nullptr\n");
        return;
    }
    stream->write(static_cast<unsigned int>((a >> 8) & 0xff));
    stream->write(static_cast<unsigned int>(a & 0xff));
    written += 2;
}

// Writes a char to the underlying output stream as a 2-byte value, high byte
// first. If no exception is thrown, the counter written is incremented by 2.
// Parameters:
// v - a char value to be written.
void DataOutputStream::writeChar(char v) {
    stream->write((v >> 8) & 0xff);
    stream->write(v & 0xff);
    // TODO 4J Stu - Error handling?
    written += 2;
}

// Writes a string to the underlying output stream as a sequence of characters.
// Each character is written to the data output stream as if by the writeChar
// method. If no exception is thrown, the counter written is incremented by
// twice the length of s. Parameters: s - a String value to be written.
void DataOutputStream::writeChars(const std::string& str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        writeChar(str.at(i));
        // TODO 4J Stu - Error handling?
    }
    // Incrementing handled by the writeChar function
}

// Writes a bool to the underlying output stream as a 1-byte value.
// The value true is written out as the value (uint8_t)1; the value false is
// written out as the value (uint8_t)0. If no exception is thrown, the counter
// written is incremented by 1. Parameters: v - a bool value to be written.
void DataOutputStream::writeBoolean(bool b) {
    stream->write(b ? 1 : 0);
    // TODO 4J Stu - Error handling?
    written += 1;
}

// Writes a string to the underlying output stream using modified UTF-8 encoding
// in a machine-independent manner. First, two bytes are written to the output
// stream as if by the writeShort method giving the number of bytes to follow.
// This value is the number of bytes actually written out, not the length of the
// string. Following the length, each character of the string is output, in
// sequence, using the modified UTF-8 encoding for the character. If no
// exception is thrown, the counter written is incremented by the total number
// of bytes written to the output stream. This will be at least two plus the
// length of str, and at most two plus thrice the length of str. Parameters: str
// - a string to be written.
void DataOutputStream::writeUTF(const std::string& str) {
    int strlen = (int)str.length();
    int utflen = 0;
    int c, count = 0;

    /* use charAt instead of copying String to char array */
    for (int i = 0; i < strlen; i++) {
        c = str.at(i);
        if ((c >= 0x0001) && (c <= 0x007F)) {
            utflen++;
        } else if (c > 0x07FF) {
            utflen += 3;
        } else {
            utflen += 2;
        }
    }

    // if (utflen > 65535)
    //	throw new UTFDataFormatException(
    //	"encoded string too long: " + utflen + " bytes");

    std::vector<uint8_t> bytearr(utflen + 2);

    bytearr[count++] = (uint8_t)((utflen >> 8) & 0xFF);
    bytearr[count++] = (uint8_t)((utflen >> 0) & 0xFF);

    int i = 0;
    for (i = 0; i < strlen; i++) {
        c = str.at(i);
        if (!((c >= 0x0001) && (c <= 0x007F))) break;
        bytearr[count++] = (uint8_t)c;
    }

    for (; i < strlen; i++) {
        c = str.at(i);
        if ((c >= 0x0001) && (c <= 0x007F)) {
            bytearr[count++] = (uint8_t)c;

        } else if (c > 0x07FF) {
            bytearr[count++] = (uint8_t)(0xE0 | ((c >> 12) & 0x0F));
            bytearr[count++] = (uint8_t)(0x80 | ((c >> 6) & 0x3F));
            bytearr[count++] = (uint8_t)(0x80 | ((c >> 0) & 0x3F));
        } else {
            bytearr[count++] = (uint8_t)(0xC0 | ((c >> 6) & 0x1F));
            bytearr[count++] = (uint8_t)(0x80 | ((c >> 0) & 0x3F));
        }
    }
    write(bytearr, 0, utflen + 2);
}

// 4J Added
void DataOutputStream::writePlayerUID(unsigned long long player) {
    writeLong(player);
}
