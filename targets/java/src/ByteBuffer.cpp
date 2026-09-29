
#include "java/ByteBuffer.h"

#include <assert.h>
#include <string.h>

#include <algorithm>
#include <vector>

#include "java/Buffer.h"
#include "java/FloatBuffer.h"
#include "java/IntBuffer.h"

ByteBuffer::ByteBuffer(unsigned int capacity) : Buffer(capacity) {
    hasBackingArray = false;
    buffer = new uint8_t[capacity];
    memset(buffer, 0, sizeof(uint8_t) * capacity);
    byteOrder = std::endian::big;
}

// Allocates a new direct byte buffer.
// The new buffer's position will be zero, its limit will be its capacity, and
// its mark will be undefined. Whether or not it has a backing array is
// unspecified.
//
// Parameters:
// capacity - The new buffer's capacity, in bytes
// Returns:
// The new byte buffer
ByteBuffer* ByteBuffer::allocateDirect(int capacity) {
    return new ByteBuffer(capacity);
}

ByteBuffer::ByteBuffer(unsigned int capacity, uint8_t* backingArray)
    : Buffer(capacity) {
    hasBackingArray = true;
    buffer = backingArray;
}

ByteBuffer::~ByteBuffer() {
    if (!hasBackingArray) delete[] buffer;
}

// Wraps a byte array into a buffer.
// The new buffer will be backed by the given uint8_t array; that is,
// modifications to the buffer will cause the array to be modified and vice
// versa. The new buffer's capacity and limit will be array.size(), its position
// will be zero, and its mark will be undefined. Its backing array will be the
// given array, and its array offset will be zero.
//
// Parameters:
// array - The array that will back this buffer
// Returns:
// The new byte buffer
ByteBuffer* ByteBuffer::wrap(std::vector<uint8_t>& b) {
    return new ByteBuffer(b.size(), b.data());
}

// Allocates a new byte buffer.
// The new buffer's position will be zero, its limit will be its capacity, and
// its mark will be undefined. It will have a backing array, and its array
// offset will be zero.
//
// Parameters:
// capacity - The new buffer's capacity, in bytes
// Returns:
// The new byte buffer
ByteBuffer* ByteBuffer::allocate(unsigned int capacity) {
    return new ByteBuffer(capacity);
}

// Modifies this buffer's byte order.
// Parameters:
// bo - The new byte order, either std::endian::big or std::endian::little
void ByteBuffer::order(std::endian bo) { byteOrder = bo; }

// Flips this buffer. The limit is set to the current position and then the
// position is set to zero. If the mark is defined then it is discarded.
//
// Returns:
// This buffer
ByteBuffer* ByteBuffer::flip() {
    m_limit = m_position;
    m_position = 0;
    return this;
}

// 4J Added so we can write this to a file
uint8_t* ByteBuffer::getBuffer() { return buffer; }

int ByteBuffer::getSize() {
    // TODO 4J Stu - Should this be the capcity and not the limit?
    return m_limit;
}
// End 4J

// Absolute get method. Reads the byte at the given index.
// Parameters:
// index - The index from which the byte will be read
// Returns:
// The byte at the given index
// Throws:
// IndexOutOfBoundsException - If index is negative or not smaller than the
// buffer's limit
uint8_t ByteBuffer::get(int index) {
    assert(index < m_limit);
    assert(index >= 0);

    return buffer[index];
}

// Relative get method for reading an int value.
// Reads the next four bytes at this buffer's current position, composing them
// into an int value according to the current byte order, and then increments
// the position by four.
//
// Returns:
// The int value at the buffer's current position
int ByteBuffer::getInt() {
    assert(m_position + 3 < m_limit);

    int value = 0;

    int b1 = static_cast<int>(buffer[m_position]);
    int b2 = static_cast<int>(buffer[m_position + 1]);
    int b3 = static_cast<int>(buffer[m_position + 2]);
    int b4 = static_cast<int>(buffer[m_position + 3]);

    m_position += 4;

    if (byteOrder == std::endian::big) {
        value = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    } else if (byteOrder == std::endian::little) {
        value = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);
    }
    return value;
}

// Absolute get method for reading an int value.
// Reads four bytes at the given index, composing them into a int value
// according to the current byte order.
//
// Parameters:
// index - The index from which the bytes will be read
// Returns:
// The int value at the given index
int ByteBuffer::getInt(unsigned int index) {
    assert(index + 3 < m_limit);
    int value = 0;

    int b1 = static_cast<int>(buffer[index]);
    int b2 = static_cast<int>(buffer[index + 1]);
    int b3 = static_cast<int>(buffer[index + 2]);
    int b4 = static_cast<int>(buffer[index + 3]);

    if (byteOrder == std::endian::big) {
        value = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    } else if (byteOrder == std::endian::little) {
        value = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);
    }
    return value;
}

// Relative get method for reading a long value.
// Reads the next eight bytes at this buffer's current position, composing them
// into a long value according to the current byte order, and then increments
// the position by eight.
//
// Returns:
// The long value at the buffer's current position
int64_t ByteBuffer::getLong() {
    assert(m_position + 8 < m_limit);

    int64_t value = 0;

    int64_t b1 = static_cast<int64_t>(buffer[m_position]);
    int64_t b2 = static_cast<int64_t>(buffer[m_position + 1]);
    int64_t b3 = static_cast<int64_t>(buffer[m_position + 2]);
    int64_t b4 = static_cast<int64_t>(buffer[m_position + 3]);
    int64_t b5 = static_cast<int64_t>(buffer[m_position + 4]);
    int64_t b6 = static_cast<int64_t>(buffer[m_position + 5]);
    int64_t b7 = static_cast<int64_t>(buffer[m_position + 6]);
    int64_t b8 = static_cast<int64_t>(buffer[m_position + 7]);

    m_position += 8;

    if (byteOrder == std::endian::big) {
        value = (b1 << 56) | (b2 << 48) | (b3 << 40) | (b4 << 32) | (b5 << 24) |
                (b6 << 16) | (b7 << 8) | b8;
    } else if (byteOrder == std::endian::little) {
        value = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24) | (b5 << 32) |
                (b6 << 40) | (b7 << 48) | (b8 << 56);
    }
    return value;
}

// Relative get method for reading a short value.
// Reads the next two bytes at this buffer's current position, composing them
// into a short value according to the current byte order, and then increments
// the position by two.
//
// Returns:
// The short value at the buffer's current position
short ByteBuffer::getShort() {
    assert(m_position + 1 < m_limit);

    short value = 0;

    short b1 = static_cast<short>(buffer[m_position]);
    short b2 = static_cast<short>(buffer[m_position + 1]);

    m_position += 2;

    if (byteOrder == std::endian::big) {
        value = (b1 << 8) | b2;
    } else if (byteOrder == std::endian::little) {
        value = b1 | (b2 << 8);
    }
    return value;
}

void ByteBuffer::getShortArray(std::vector<short>& s) {
    // TODO 4J Stu - Should this function be writing from the start of the
    // buffer, or from position? And should it update position?
    assert(s.size() >= m_limit / 2);

    // 4J Stu - Assumes big endian
    memcpy(s.data(), buffer, (m_limit - m_position));
}

// Absolute put method  (optional operation).
// Writes the given byte into this buffer at the given index.
//
// Parameters:
// index - The index at which the byte will be written
// b - The byte value to be written
// Returns:
// This buffer
// Throws:
// IndexOutOfBoundsException - If index is negative or not smaller than the
// buffer's limit ReadOnlyBufferException - If this buffer is read-only
ByteBuffer* ByteBuffer::put(int index, uint8_t b) {
    assert(index < m_limit);
    assert(index >= 0);

    buffer[index] = b;
    return this;
}

// Relative put method for writing an int value  (optional operation).
// Writes four bytes containing the given int value, in the current byte order,
// into this buffer at the current position, and then increments the position by
// four.
//
// Parameters:
// value - The int value to be written
// Returns:
// This buffer
ByteBuffer* ByteBuffer::putInt(int value) {
    assert(m_position + 3 < m_limit);

    if (byteOrder == std::endian::big) {
        buffer[m_position] = static_cast<uint8_t>((value >> 24) & 0xFF);
        buffer[m_position + 1] = static_cast<uint8_t>((value >> 16) & 0xFF);
        buffer[m_position + 2] = static_cast<uint8_t>((value >> 8) & 0xFF);
        buffer[m_position + 3] = static_cast<uint8_t>(value & 0xFF);
    } else if (byteOrder == std::endian::little) {
        buffer[m_position] = static_cast<uint8_t>(value & 0xFF);
        buffer[m_position + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
        buffer[m_position + 2] = static_cast<uint8_t>((value >> 16) & 0xFF);
        buffer[m_position + 3] = static_cast<uint8_t>((value >> 24) & 0xFF);
    }

    m_position += 4;

    return this;
}

// Absolute put method for writing an int value  (optional operation).
// Writes four bytes containing the given int value, in the current byte order,
// into this buffer at the given index.
//
// Parameters:
// index - The index at which the bytes will be written
// value - The int value to be written
// Returns:
// This buffer
ByteBuffer* ByteBuffer::putInt(unsigned int index, int value) {
    assert(index + 3 < m_limit);

    if (byteOrder == std::endian::big) {
        buffer[index] = static_cast<uint8_t>((value >> 24) & 0xFF);
        buffer[index + 1] = static_cast<uint8_t>((value >> 16) & 0xFF);
        buffer[index + 2] = static_cast<uint8_t>((value >> 8) & 0xFF);
        buffer[index + 3] = static_cast<uint8_t>(value & 0xFF);
    } else if (byteOrder == std::endian::little) {
        buffer[index] = static_cast<uint8_t>(value & 0xFF);
        buffer[index + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
        buffer[index + 2] = static_cast<uint8_t>((value >> 16) & 0xFF);
        buffer[index + 3] = static_cast<uint8_t>((value >> 24) & 0xFF);
    }

    return this;
}

// Relative put method for writing a short value  (optional operation).
// Writes two bytes containing the given short value, in the current byte order,
// into this buffer at the current position, and then increments the position by
// two.
//
// Parameters:
// value - The short value to be written
// Returns:
// This buffer
ByteBuffer* ByteBuffer::putShort(short value) {
    assert(m_position + 1 < m_limit);

    if (byteOrder == std::endian::big) {
        buffer[m_position] = static_cast<uint8_t>((value >> 8) & 0xFF);
        buffer[m_position + 1] = static_cast<uint8_t>(value & 0xFF);
    } else if (byteOrder == std::endian::little) {
        buffer[m_position] = static_cast<uint8_t>(value & 0xFF);
        buffer[m_position + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
    }

    m_position += 2;

    return this;
}

ByteBuffer* ByteBuffer::putShortArray(std::vector<short>& s) {
    // TODO 4J Stu - Should this function be writing from the start of the
    // buffer, or from position? And should it update position?
    assert(s.size() * 2 <= m_limit);

    // 4J Stu - Assumes big endian
    memcpy(buffer, s.data(), s.size() * 2);

    return this;
}

// Relative put method for writing a long value  (optional operation).
// Writes eight bytes containing the given long value, in the current byte
// order, into this buffer at the current position, and then increments the
// position by eight.
//
// Parameters:
// value - The long value to be written
// Returns:
// This buffer
ByteBuffer* ByteBuffer::putLong(int64_t value) {
    assert(m_position + 7 < m_limit);

    if (byteOrder == std::endian::big) {
        buffer[m_position] = static_cast<uint8_t>((value >> 56) & 0xFF);
        buffer[m_position + 1] = static_cast<uint8_t>((value >> 48) & 0xFF);
        buffer[m_position + 2] = static_cast<uint8_t>((value >> 40) & 0xFF);
        buffer[m_position + 3] = static_cast<uint8_t>((value >> 32) & 0xFF);
        buffer[m_position + 4] = static_cast<uint8_t>((value >> 24) & 0xFF);
        buffer[m_position + 5] = static_cast<uint8_t>((value >> 16) & 0xFF);
        buffer[m_position + 6] = static_cast<uint8_t>((value >> 8) & 0xFF);
        buffer[m_position + 7] = static_cast<uint8_t>(value & 0xFF);
    } else if (byteOrder == std::endian::little) {
        buffer[m_position] = static_cast<uint8_t>((value & 0xFF));
        buffer[m_position + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
        buffer[m_position + 2] = static_cast<uint8_t>((value >> 16) & 0xFF);
        buffer[m_position + 3] = static_cast<uint8_t>((value >> 24) & 0xFF);
        buffer[m_position + 4] = static_cast<uint8_t>((value >> 32) & 0xFF);
        buffer[m_position + 5] = static_cast<uint8_t>((value >> 40) & 0xFF);
        buffer[m_position + 6] = static_cast<uint8_t>((value >> 48) & 0xFF);
        buffer[m_position + 7] = static_cast<uint8_t>((value >> 56) & 0xFF);
    }

    return this;
}

// Relative bulk put method  (optional operation).
// This method transfers the entire content of the given source byte array into
// this buffer. An invocation of this method of the form dst.put(a) behaves in
// exactly the same way as the invocation
//
//      dst.put(a, 0, a.size())
// Returns:
// This buffer
ByteBuffer* ByteBuffer::put(std::vector<uint8_t>& inputArray) {
    if (inputArray.size() > remaining())
        assert(false);  // TODO 4J Stu - Some kind of exception?

    std::copy(inputArray.data(), inputArray.data() + inputArray.size(),
              buffer + m_position);

    m_position += inputArray.size();

    return this;
}

std::vector<uint8_t> ByteBuffer::array() {
    return std::vector<uint8_t>(buffer, buffer + m_capacity);
}

// Creates a view of this byte buffer as an int buffer.
// The content of the new buffer will start at this buffer's current position.
// Changes to this buffer's content will be visible in the new buffer, and vice
// versa; the two buffers' position, limit, and mark values will be independent.
//
// The new buffer's position will be zero, its capacity and its limit will be
// the number of bytes remaining in this buffer divided by four, and its mark
// will be undefined. The new buffer will be direct if, and only if, this buffer
// is direct, and it will be read-only if, and only if, this buffer is
// read-only.
//
// Returns:
// A new int buffer
IntBuffer* ByteBuffer::asIntBuffer() {
    // TODO 4J Stu - Is it safe to just cast our byte array pointer to another
    // type?
    return new IntBuffer((m_limit - m_position) / 4,
                         (int*)(buffer + m_position));
}

// Creates a view of this byte buffer as a float buffer.
// The content of the new buffer will start at this buffer's current position.
// Changes to this buffer's content will be visible in the new buffer, and vice
// versa; the two buffers' position, limit, and mark values will be independent.
//
// The new buffer's position will be zero, its capacity and its limit will be
// the number of bytes remaining in this buffer divided by four, and its mark
// will be undefined. The new buffer will be direct if, and only if, this buffer
// is direct, and it will be read-only if, and only if, this buffer is
// read-only.
//
// Returns:
// A new float buffer
FloatBuffer* ByteBuffer::asFloatBuffer() {
    // TODO 4J Stu - Is it safe to just cast our byte array pointer to another
    // type?
    return new FloatBuffer((m_limit - m_position) / 4,
                           (float*)(buffer + m_position));
}
