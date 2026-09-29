#pragma once

#include <cstdint>

#include "java/Class.h"

// Java doesn't have a default hash value for ints, however, the hashmap itself
// does some "supplemental" hashing, so our ints actually get hashed by code as
// implemented below. std templates *do* have a standard hash for ints, but it
// would appear to be a bit expensive so matching the java one for now anyway.
// This code implements the supplemental hashing that happens in java so we can
// match what their maps are doing with ints.

struct IntKeyHash {
    int operator()(const int& k) const {
        // 4jcraft added h to be unsigned, to not cast it later
        unsigned int h = k;
        h += ~(h << 9);
        h ^= (h >> 14);
        h += (h << 4);
        h ^= (h >> 10);
        return h;
    }
};

struct IntKeyEq {
    bool operator()(const int& x, const int& y) const { return x == y; }
};

// This hash functor is taken from the IntHashMap java class used by the game,
// so that we can use a standard std hashmap with this hash rather than
// implement the class itself
struct IntKeyHash2 {
    int operator()(const int& k) const {
        unsigned int h = (unsigned int)k;
        h ^= (h >> 20) ^ (h >> 12);
        return (int)(h ^ (h >> 7) ^ (h >> 4));
    }
};

// This hash functor is taken from the LongHashMap java class used by the game,
// so that we can use a standard std hashmap with this hash rather than
// implement the class itself
struct LongKeyHash {
    int hash(const int& k) const {
        unsigned int h = (unsigned int)k;
        h ^= (h >> 20) ^ (h >> 12);
        return (int)(h ^ (h >> 7) ^ (h >> 4));
    }

    int operator()(const int64_t& k) const {
        return hash((int)(k ^ (((uint64_t)k) >> 32)));
    }
};

struct LongKeyEq {
    bool operator()(const int64_t& x, const int64_t& y) const { return x == y; }
};

struct eINSTANCEOFKeyHash {
    int operator()(const eINSTANCEOF& k) const {
        unsigned int h = (unsigned int)k;
        h ^= (h >> 20) ^ (h >> 12);
        return (int)(h ^ (h >> 7) ^ (h >> 4));
    }
};

struct eINSTANCEOFKeyEq {
    bool operator()(const eINSTANCEOF& x, const eINSTANCEOF& y) const {
        return x == y;
    }
};
