#pragma once
#include <stdint.h>

#include <ostream>
#include <string>

#include "java/InputOutputStream/InputOutputStream.h"

class DataInput;
class DataOutput;

class Tag {
public:
    static const uint8_t TAG_End = static_cast<uint8_t>(0);
    static const uint8_t TAG_Byte = static_cast<uint8_t>(1);
    static const uint8_t TAG_Short = static_cast<uint8_t>(2);
    static const uint8_t TAG_Int = static_cast<uint8_t>(3);
    static const uint8_t TAG_Long = static_cast<uint8_t>(4);
    static const uint8_t TAG_Float = static_cast<uint8_t>(5);
    static const uint8_t TAG_Double = static_cast<uint8_t>(6);
    static const uint8_t TAG_Byte_Array = static_cast<uint8_t>(7);
    static const uint8_t TAG_String = static_cast<uint8_t>(8);
    static const uint8_t TAG_List = static_cast<uint8_t>(9);
    static const uint8_t TAG_Compound = static_cast<uint8_t>(10);
    static const uint8_t TAG_Int_Array = static_cast<uint8_t>(11);
    static const int MAX_DEPTH = static_cast<uint8_t>(512);

private:
    std::string name;

protected:
    Tag(const std::string& name);

public:
    virtual void write(DataOutput* dos) = 0;
    virtual void load(DataInput* dis, int tagDepth) = 0;
    virtual std::string toString() = 0;
    virtual uint8_t getId() = 0;
    void print(std::ostream& out);
    void print(char* prefix, std::ostream& out);
    std::string getName();
    Tag* setName(const std::string& name);
    static Tag* readNamedTag(DataInput* dis);
    static Tag* readNamedTag(DataInput* dis, int tagDepth);
    static void writeNamedTag(Tag* tag, DataOutput* dos);
    static Tag* newTag(uint8_t type, const std::string& name);
    static const char* getTagName(uint8_t type);
    virtual ~Tag() {}
    virtual bool equals(Tag* obj);  // 4J Brought forward from 1.2
    virtual Tag* copy() = 0;        // 4J Brought foward from 1.2
};
