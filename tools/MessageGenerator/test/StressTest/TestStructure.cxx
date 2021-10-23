#include "TestStructure.hxx"

#include <string.h>

namespace arse {
    TestStructure::TestStructure() {

    }

    TestStructure::~TestStructure() {

    }

    void TestStructure::serialise(char* data, uint64_t& offset) const {
        // Serialise int8Member
        memcpy(data + offset, &int8Member, sizeof (int8_t));
        offset += sizeof (int8_t);

        // Serialise int16Member
        memcpy(data + offset, &int16Member, sizeof (int16_t));
        offset += sizeof (int16_t);

        // Serialise int32Member
        memcpy(data + offset, &int32Member, sizeof (int32_t));
        offset += sizeof (int32_t);

        // Serialise int64Member
        memcpy(data + offset, &int64Member, sizeof (int64_t));
        offset += sizeof (int64_t);
    }

    void TestStructure::deserialise(const char* data, uint64_t& offset) {
        // Deserialise int8Member
        memcpy(&int8Member, data + offset, sizeof (int8_t));
        offset += sizeof (int8_t);

        // Deserialise int16Member
        memcpy(&int16Member, data + offset, sizeof (int16_t));
        offset += sizeof (int16_t);

        // Deserialise int32Member
        memcpy(&int32Member, data + offset, sizeof (int32_t));
        offset += sizeof (int32_t);

        // Deserialise int64Member
        memcpy(&int64Member, data + offset, sizeof (int64_t));
        offset += sizeof (int64_t);
    }

    uint64_t TestStructure::getSizeInBytes() const {
        // Size of primitive types in this structure
        uint64_t size = 15;

        return size;
    }

    int8_t TestStructure::getInt8Member() const {
        return int8Member;
    }

    int16_t TestStructure::getInt16Member() const {
        return int16Member;
    }

    int32_t TestStructure::getInt32Member() const {
        return int32Member;
    }

    int64_t TestStructure::getInt64Member() const {
        return int64Member;
    }
    void TestStructure::setInt8Member(const int8_t value) {
        int8Member = value;
    }

    void TestStructure::setInt16Member(const int16_t value) {
        int16Member = value;
    }

    void TestStructure::setInt32Member(const int32_t value) {
        int32Member = value;
    }

    void TestStructure::setInt64Member(const int64_t value) {
        int64Member = value;
    }
}
