#include "TestSubStruct.hxx"

#include <string.h>

namespace MyNamespace {
    TestSubStruct::TestSubStruct() {

    }

    TestSubStruct::~TestSubStruct() {

    }

    void TestSubStruct::serialise(char* data, uint64_t& offset) const {
        // Serialise bigUnsigned
        memcpy(data + offset, &bigUnsigned, sizeof(uint64_t));
        offset += sizeof(bigUnsigned);
    }

    void TestSubStruct::deserialise(const char* data, uint64_t& offset) {
        // Deserialise bigUnsigned
        memcpy(&bigUnsigned, data + offset, sizeof(uint64_t));
        offset += sizeof(bigUnsigned);
    }

    uint64_t TestSubStruct::getSizeInBytes() const {
        // bigUnsigned = 8 bytes (uint64_t)
        // Size of primitive types in this structure
        uint64_t size = 8;

        return size;
    }

    uint64_t TestSubStruct::getBigUnsigned() const {
        return bigUnsigned;
    }

    void TestSubStruct::setBigUnsigned(const uint64_t value) {
        bigUnsigned = value;
    }

}
