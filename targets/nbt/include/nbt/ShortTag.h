#pragma once
#include "Tag.h"

class ShortTag : public Tag {
public:
    short data;
    ShortTag(const std::string& name) : Tag(name) {}
    ShortTag(const std::string& name, int data) : Tag(name) {
        this->data = data;
    }

    void write(DataOutput* dos) { dos->writeShort(data); }
    void load(DataInput* dis, int tagDepth) { data = dis->readShort(); }

    uint8_t getId() { return TAG_Short; }
    std::string toString() {
        static char buf[32];
        snprintf(buf, 32, "%d", data);
        return std::string(buf);
    }

    Tag* copy() { return new ShortTag(getName(), data); }

    bool equals(Tag* obj) {
        if (Tag::equals(obj)) {
            ShortTag* o = (ShortTag*)obj;
            return data == o->data;
        }
        return false;
    }
};