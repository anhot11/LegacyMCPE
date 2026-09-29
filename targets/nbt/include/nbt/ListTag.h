#pragma once
#include <cstring>
#include <memory>
#include <vector>

#include "Tag.h"

template <class T>
class ListTag : public Tag {
private:
    std::vector<std::unique_ptr<Tag>> list;
    uint8_t type;

public:
    ListTag() : Tag("") {}
    ListTag(const std::string& name) : Tag(name) {}

    void write(DataOutput* dos) {
        if (list.size() > 0)
            type = list[0]->getId();
        else
            type = static_cast<uint8_t>(1);

        dos->writeByte(type);
        dos->writeInt((int)list.size());

        for (auto& tag : list) tag->write(dos);
    }

    void load(DataInput* dis, int tagDepth) {
        if (tagDepth > MAX_DEPTH) {
#ifndef _CONTENT_PACKAGE
            printf("Tried to read NBT tag with too high complexity, depth > %d",
                   MAX_DEPTH);
#endif
            return;
        }
        type = dis->readByte();
        int size = dis->readInt();

        list.clear();
        for (int i = 0; i < size; i++) {
            std::unique_ptr<Tag> tag(Tag::newTag(type, ""));
            tag->load(dis, tagDepth);
            list.push_back(std::move(tag));
        }
    }

    uint8_t getId() { return TAG_List; }

    std::string toString() {
        static char buf[64];
        snprintf(buf, 64, "%zu entries of type %s", list.size(),
                 Tag::getTagName(type));
        return std::string(buf);
    }

    void print(char* prefix, std::ostream& out) {
        Tag::print(prefix, out);

        out << prefix << "{" << std::endl;

        char* newPrefix = new char[strlen(prefix) + 4];
        strcpy(newPrefix, prefix);
        strcat(newPrefix, "   ");
        for (auto& tag : list) {
            tag->print(newPrefix, out);
        }
        delete[] newPrefix;
        out << prefix << "}" << std::endl;
    }

    void add(T* tag) {
        type = tag->getId();
        // 4J: List tag write/load doesn't preserve tag names so remove them so
        // we can safely do comparisons There are a few ways I could have fixed
        // this but this seems the least invasive, most complete fix (covers
        // other items that also use list tags and require equality checks to
        // work) considering we can't change the write/load functions.
        tag->setName("");
        list.push_back(std::unique_ptr<Tag>(tag));
    }

    T* get(int index) { return static_cast<T*>(list[index].get()); }

    int size() { return (int)list.size(); }

    virtual ~ListTag() = default;

    virtual Tag* copy() {
        ListTag<T>* res = new ListTag<T>(getName());
        res->type = type;
        for (auto& tag : list) {
            res->list.push_back(std::unique_ptr<Tag>(tag->copy()));
        }
        return res;
    }

    virtual bool equals(Tag* obj) {
        if (Tag::equals(obj)) {
            ListTag* o = (ListTag*)obj;
            if (type == o->type) {
                bool equal = false;
                if (list.size() == o->list.size()) {
                    equal = true;
                    // 4J Stu - Pretty inefficient method, but I think we can
                    // live with it give how often it will happen, and the small
                    // sizes of the data sets
                    for (auto& tag : list) {
                        bool thisMatches = false;
                        for (auto& otherTag : o->list) {
                            if (tag->equals(otherTag.get())) {
                                thisMatches = true;
                                break;
                            }
                        }
                        if (!thisMatches) {
                            equal = false;
                            break;
                        }
                    }
                }

                return equal;
            }
        }
        return false;
    }
};
