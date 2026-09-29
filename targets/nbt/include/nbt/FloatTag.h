#pragma once
#include "Tag.h"
#include "java/InputOutputStream/InputOutputStream.h"

class FloatTag : public Tag {
public:
    float data;
    FloatTag(const std::string& name) : Tag(name) {}
    FloatTag(const std::string& name, float data) : Tag(name) {
        this->data = data;
    }

    void write(DataOutput* dos) { dos->writeFloat(data); }
    void load(DataInput* dis, int tagDepth) { data = dis->readFloat(); }

    uint8_t getId() { return TAG_Float; }
    std::string toString() {
        static char buf[32];
        snprintf(buf, 32, "%f", data);
        return std::string(buf);
    }

    Tag* copy() { return new FloatTag(getName(), data); }

    bool equals(Tag* obj) {
        if (Tag::equals(obj)) {
            FloatTag* o = (FloatTag*)obj;
            return data == o->data;
        }
        return false;
    }
};