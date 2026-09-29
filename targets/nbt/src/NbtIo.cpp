#include "nbt/NbtIo.h"

#include "java/InputOutputStream/BufferedOutputStream.h"
#include "java/InputOutputStream/ByteArrayInputStream.h"
#include "java/InputOutputStream/ByteArrayOutputStream.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "java/System.h"
#include "nbt/CompoundTag.h"
#include "nbt/Tag.h"

class DataInput;
class DataOutput;
class OutputStream;

CompoundTag* NbtIo::readCompressed(InputStream* in) {
    // 4J - this was using a try/finally block
    DataInputStream dis =
        DataInputStream(in);  // 4J - was new GZIPInputStream as well
    CompoundTag* ret = NbtIo::read((DataInput*)&dis);
    dis.close();
    return ret;
}

void NbtIo::writeCompressed(CompoundTag* tag, OutputStream* out) {
    // 4J - this was using a try/finally block
    // 4J Stu - Buffer output in 1024 byte chunks so that we can allocate
    // properly in the save file
    BufferedOutputStream bos = BufferedOutputStream(out, 1024);
    DataOutputStream dos =
        DataOutputStream(&bos);  // 4J - was new GZIPOutputStream as well
    NbtIo::write(tag, &dos);
    dos.close();
}

// Reads tags from a stream created from the input buffer. Doesn't free the data
// in the source buffer.
CompoundTag* NbtIo::decompress(std::vector<uint8_t> buffer) {
    ByteArrayInputStream bais = ByteArrayInputStream(buffer);
    // 4J - this was using a try/finally block
    DataInputStream in =
        DataInputStream(&bais);  // 4J - was new GZIPInputStream as well
    CompoundTag* ret = NbtIo::read((DataInput*)&in);
    bais.reset();  // This stops the buffer referenced by the input stream from
                   // being freed when it goes out of context
    in.close();
    return ret;
}

std::vector<uint8_t> NbtIo::compress(CompoundTag* tag) {
    // 4J - this was using a try/finally block
    ByteArrayOutputStream baos = ByteArrayOutputStream();
    DataOutputStream dos =
        DataOutputStream(&baos);  // 4J - was new GZIPOutputStream as well
    NbtIo::write(tag, &dos);

    std::vector<uint8_t> ret(baos.buf.size());
    System::arraycopy(baos.buf, 0, &ret, 0, baos.buf.size());
    dos.close();
    return ret;
}

CompoundTag* NbtIo::read(DataInput* dis) {
    Tag* tag = Tag::readNamedTag(dis);

    if (tag->getId() == Tag::TAG_Compound) return (CompoundTag*)tag;

    if (tag != nullptr) delete tag;
    // Root tag must be a named compound tag
    return nullptr;
}

void NbtIo::write(CompoundTag* tag, DataOutput* dos) {
    Tag::writeNamedTag(tag, dos);
}