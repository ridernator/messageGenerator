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

        // Serialise a8String
        // Serialise size of a8String
        uint64_t a8StringSize = a8String.size();
        memcpy(data + offset, &a8StringSize, sizeof(a8StringSize));
        offset += sizeof(a8StringSize);

        // Serialise a8String data
        memcpy(data + offset, &a8String[0], sizeof(a8String[0]) * a8String.size());
        offset += sizeof(a8String[0]) * a8String.size();

        // Serialise b8String
        // Serialise size of b8String
        uint64_t b8StringSize = b8String.size();
        memcpy(data + offset, &b8StringSize, sizeof(b8StringSize));
        offset += sizeof(b8StringSize);

        // Serialise b8String data
        memcpy(data + offset, &b8String[0], sizeof(b8String[0]) * b8String.size());
        offset += sizeof(b8String[0]) * b8String.size();

        // Serialise a16String
        // Serialise size of a16String
        uint64_t a16StringSize = a16String.size();
        memcpy(data + offset, &a16StringSize, sizeof(a16StringSize));
        offset += sizeof(a16StringSize);

        // Serialise a16String data
        memcpy(data + offset, &a16String[0], sizeof(a16String[0]) * a16String.size());
        offset += sizeof(a16String[0]) * a16String.size();

        // Serialise a32String
        // Serialise size of a32String
        uint64_t a32StringSize = a32String.size();
        memcpy(data + offset, &a32StringSize, sizeof(a32StringSize));
        offset += sizeof(a32StringSize);

        // Serialise a32String data
        memcpy(data + offset, &a32String[0], sizeof(a32String[0]) * a32String.size());
        offset += sizeof(a32String[0]) * a32String.size();
    }

    void TestSubStruct::deserialise(const char* data, uint64_t& offset) {
        // Deserialise bigUnsigned
        memcpy(&bigUnsigned, data + offset, sizeof(bigUnsigned));
        offset += sizeof(bigUnsigned);

        // Deserialise a8String
        // Deserialise size of a8String
        uint64_t a8StringSize;
        memcpy(&a8StringSize, data + offset, sizeof(a8StringSize));
        offset += sizeof(a8StringSize);
        a8String.resize(a8StringSize);

        // Deserialise a8String data
        memcpy(&a8String[0], data + offset, sizeof(a8String[0]) * a8String.size());
        offset += sizeof(a8String[0]) * a8String.size();

        // Deserialise b8String
        // Deserialise size of b8String
        uint64_t b8StringSize;
        memcpy(&b8StringSize, data + offset, sizeof(b8StringSize));
        offset += sizeof(b8StringSize);
        b8String.resize(b8StringSize);

        // Deserialise b8String data
        memcpy(&b8String[0], data + offset, sizeof(b8String[0]) * b8String.size());
        offset += sizeof(b8String[0]) * b8String.size();

        // Deserialise a16String
        // Deserialise size of a16String
        uint64_t a16StringSize;
        memcpy(&a16StringSize, data + offset, sizeof(a16StringSize));
        offset += sizeof(a16StringSize);
        a16String.resize(a16StringSize);

        // Deserialise a16String data
        memcpy(&a16String[0], data + offset, sizeof(a16String[0]) * a16String.size());
        offset += sizeof(a16String[0]) * a16String.size();

        // Deserialise a32String
        // Deserialise size of a32String
        uint64_t a32StringSize;
        memcpy(&a32StringSize, data + offset, sizeof(a32StringSize));
        offset += sizeof(a32StringSize);
        a32String.resize(a32StringSize);

        // Deserialise a32String data
        memcpy(&a32String[0], data + offset, sizeof(a32String[0]) * a32String.size());
        offset += sizeof(a32String[0]) * a32String.size();
    }

    uint64_t TestSubStruct::getSizeInBytes() const {
        uint64_t size = 0;

        // Add on size of bigUnsigned
        size += sizeof(bigUnsigned);

        // Add on size of a8String
        size += sizeof(std::string::value_type) * a8String.size();

        // Add on size of string size field
        size += sizeof(uint64_t);

        // Add on size of b8String
        size += sizeof(std::string::value_type) * b8String.size();

        // Add on size of string size field
        size += sizeof(uint64_t);

        // Add on size of a16String
        size += sizeof(std::u16string::value_type) * a16String.size();

        // Add on size of string size field
        size += sizeof(uint64_t);

        // Add on size of a32String
        size += sizeof(std::u32string::value_type) * a32String.size();

        // Add on size of string size field
        size += sizeof(uint64_t);

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
