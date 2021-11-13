#include "TestSubStruct.hxx"

#include <string.h>

namespace MyNamespace {
    TestSubStruct::TestSubStruct() {

    }

    TestSubStruct::~TestSubStruct() {

    }

    void TestSubStruct::serialise(char* data, uint64_t& offset) const {
        // Serialise bigUnsigned
        memcpy(data + offset, &bigUnsigned, sizeof(bigUnsigned));
        offset += sizeof(bigUnsigned);
    }

    void TestSubStruct::deserialise(const char* data, uint64_t& offset) {
        // Deserialise bigUnsigned
        memcpy(&bigUnsigned, data + offset, sizeof(bigUnsigned));
        offset += sizeof(bigUnsigned);
    }

    uint64_t TestSubStruct::getSizeInBytes() const {
        uint64_t size = 0;

        // Add on size of bigUnsigned
        size += sizeof(bigUnsigned);

        return size;
    }

    uint64_t TestSubStruct::getBigUnsigned() const {
        return bigUnsigned;
    }

    void TestSubStruct::setBigUnsigned(const uint64_t value) {
        bigUnsigned = value;
    }

}
