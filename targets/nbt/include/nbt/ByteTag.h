#pragma once
#include "Tag.h"

class ByteTag : public Tag {
public:
    uint8_t data;
    ByteTag(const std::string& name) : Tag(name) {}
    ByteTag(const std::string& name, uint8_t data) : Tag(name) {
        this->data = data;
    }

    void write(DataOutput* dos) { dos->writeByte(data); }
    void load(DataInput* dis, int tagDepth) { data = dis->readByte(); }

    uint8_t getId() { return TAG_Byte; }
    std::string toString() {
        static char buf[32];
        snprintf(buf, 32, "%d", data);
        return std::string(buf);
    }

    bool equals(Tag* obj) {
        if (Tag::equals(obj)) {
            ByteTag* o = (ByteTag*)obj;
            return data == o->data;
        }
        return false;
    }

    Tag* copy() { return new ByteTag(getName(), data); }
};