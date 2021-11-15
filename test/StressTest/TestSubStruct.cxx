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

    std::string& TestSubStruct::getA8String() {
        return a8String;
    }

    std::string& TestSubStruct::getB8String() {
        return b8String;
    }

    std::u16string& TestSubStruct::getA16String() {
        return a16String;
    }

    std::u32string& TestSubStruct::getA32String() {
        return a32String;
    }

    void TestSubStruct::setBigUnsigned(const uint64_t value) {
        bigUnsigned = value;
    }

    void TestSubStruct::setA8String(const std::string& value) {
        a8String = value;
    }

    void TestSubStruct::setB8String(const std::string& value) {
        b8String = value;
    }

    void TestSubStruct::setA16String(const std::u16string& value) {
        a16String = value;
    }

    void TestSubStruct::setA32String(const std::u32string& value) {
        a32String = value;
    }

}
