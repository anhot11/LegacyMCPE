#pragma once
#include "Tag.h"
#include "java/InputOutputStream/InputOutputStream.h"

class DoubleTag : public Tag {
public:
    double data;
    DoubleTag(const std::string& name) : Tag(name) {}
    DoubleTag(const std::string& name, double data) : Tag(name) {
        this->data = data;
    }

    void write(DataOutput* dos) { dos->writeDouble(data); }
    void load(DataInput* dis, int tagDepth) { data = dis->readDouble(); }

    uint8_t getId() { return TAG_Double; }
    std::string toString() {
        static char buf[32];
        snprintf(buf, 32, "%f", data);
        return std::string(buf);
    }

    Tag* copy() { return new DoubleTag(getName(), data); }

    bool equals(Tag* obj) {
        if (Tag::equals(obj)) {
            DoubleTag* o = (DoubleTag*)obj;
            return data == o->data;
        }
        return false;
    }
};