#pragma once
#include "Tag.h"

class EndTag : public Tag {
public:
    EndTag() : Tag("") {}
    EndTag(const std::string& name) : Tag(name) {}

    void load(DataInput* dis, int tagDepth) {};
    void write(DataOutput* dos) {};

    uint8_t getId() { return TAG_End; }
    std::string toString() { return std::string("END"); }

    Tag* copy() { return new EndTag(); }

    bool equals(Tag* obj) { return Tag::equals(obj); }
};