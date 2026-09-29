#include "nbt/Tag.h"

#include <cstdio>

#include "java/InputOutputStream/DataInput.h"
#include "java/InputOutputStream/DataOutput.h"
#include "nbt/ByteArrayTag.h"
#include "nbt/ByteTag.h"
#include "nbt/CompoundTag.h"
#include "nbt/DoubleTag.h"
#include "nbt/EndTag.h"
#include "nbt/FloatTag.h"
#include "nbt/IntArrayTag.h"
#include "nbt/IntTag.h"
#include "nbt/ListTag.h"
#include "nbt/LongTag.h"
#include "nbt/ShortTag.h"
#include "nbt/StringTag.h"

Tag::Tag(const std::string& name) {
    if (name.empty()) {
        this->name = "";
    } else {
        this->name = name;
    }
}

// 4J - Was Object obj
bool Tag::equals(Tag* obj) {
    if (obj == nullptr)  // || !(obj instanceof Tag))
    {
        return false;
    }
    Tag* o = (Tag*)obj;
    if (getId() != o->getId()) {
        return false;
    }
    if ((name.empty() && !o->name.empty()) ||
        (!name.empty() && o->name.empty())) {
        return false;
    }
    if (!name.empty() && name.compare(o->name) != 0) {
        return false;
    }
    return true;
}

void Tag::print(std::ostream& out) { out << ""; }

void Tag::print(char* prefix, std::ostream& out) {
    std::string name = getName();

    out << prefix;
    out << getTagName(getId());
    if (name.length() > 0) {
        out << "(\"" << name << "\")";
    }
    out << ": ";
    out << toString() << std::endl;
}

std::string Tag::getName() { return name; }

Tag* Tag::setName(const std::string& name) {
    this->name = name;
    return this;
}

Tag* Tag::readNamedTag(DataInput* dis) { return readNamedTag(dis, 0); }

Tag* Tag::readNamedTag(DataInput* dis, int tagDepth) {
    uint8_t type = dis->readByte();
    if (static_cast<int>(type) == 0) return new EndTag();

    // 4J Stu - readByte can return -1, so if it's that then also mark as the
    // end tag
    if (static_cast<int>(type) == 255) {
        fprintf(stderr, "readNamedTag read a type of 255\n");
        return new EndTag();
    }

    std::string name = dis->readUTF();  // new String(bytes, "UTF-8");

    Tag* tag = newTag(type, name);
    //        short length = dis.readShort();
    //        byte[] bytes = new uint8_t[length];
    //        dis.readFully(bytes);

    tag->load(dis, tagDepth);
    return tag;
}

void Tag::writeNamedTag(Tag* tag, DataOutput* dos) {
    dos->writeByte(tag->getId());
    if (tag->getId() == Tag::TAG_End) return;

    //        byte[] bytes = tag.getName().getBytes("UTF-8");
    //        dos.writeShort(bytes.size());
    //        dos.write(bytes);
    dos->writeUTF(tag->getName());

    tag->write(dos);
}

Tag* Tag::newTag(uint8_t type, const std::string& name) {
    switch (type) {
        case TAG_End:
            return new EndTag(name);
        case TAG_Byte:
            return new ByteTag(name);
        case TAG_Short:
            return new ShortTag(name);
        case TAG_Int:
            return new IntTag(name);
        case TAG_Long:
            return new LongTag(name);
        case TAG_Float:
            return new FloatTag(name);
        case TAG_Double:
            return new DoubleTag(name);
        case TAG_Byte_Array:
            return new ByteArrayTag(name);
        case TAG_Int_Array:
            return new IntArrayTag(name);
        case TAG_String:
            return new StringTag(name);
        case TAG_List:
            return new ListTag<Tag>(name);
        case TAG_Compound:
            return new CompoundTag(name);
    }
    return nullptr;
}

const char* Tag::getTagName(uint8_t type) {
    switch (type) {
        case TAG_End:
            return "TAG_End";
        case TAG_Byte:
            return "TAG_Byte";
        case TAG_Short:
            return "TAG_Short";
        case TAG_Int:
            return "TAG_Int";
        case TAG_Long:
            return "TAG_Long";
        case TAG_Float:
            return "TAG_Float";
        case TAG_Double:
            return "TAG_Double";
        case TAG_Byte_Array:
            return "TAG_Byte_Array";
        case TAG_Int_Array:
            return "TAG_Int_Array";
        case TAG_String:
            return "TAG_String";
        case TAG_List:
            return "TAG_List";
        case TAG_Compound:
            return "TAG_Compound";
    }
    return "UNKNOWN";
}