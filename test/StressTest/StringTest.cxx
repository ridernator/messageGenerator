#include "StringTest.hxx"

#include <string.h>

namespace MyNamespace {
    StringTest::StringTest() {

    }

    StringTest::~StringTest() {

    }

    void StringTest::serialise(char* data, uint64_t& offset) const {
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

        // Serialise array
        for (const auto& e1 : array) {
            // Serialise size of e1
            uint64_t e1Size = e1.size();
            memcpy(data + offset, &e1Size, sizeof(e1Size));
            offset += sizeof(e1Size);

            // Serialise e1 data
            memcpy(data + offset, &e1[0], sizeof(e1[0]) * e1.size());
            offset += sizeof(e1[0]) * e1.size();
        }
    }

    void StringTest::deserialise(const char* data, uint64_t& offset) {
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

        // Deserialise array
        for (auto& e1 : array) {
            // Deserialise size of e1
            uint64_t e1Size;
            memcpy(&e1Size, data + offset, sizeof(e1Size));
            offset += sizeof(e1Size);
            e1.resize(e1Size);

            // Deserialise e1 data
            memcpy(&e1[0], data + offset, sizeof(e1[0]) * e1.size());
            offset += sizeof(e1[0]) * e1.size();
        }
    }

    uint64_t StringTest::getSizeInBytes() const {
        uint64_t size = 0;

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

        // Add on size of array
        for (const auto& e1 : array) {
            // Add on size of each individual std::string
            size += sizeof(std::string::value_type) * e1.size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        return size;
    }

    std::string& StringTest::getA8String() {
        return a8String;
    }

    std::string& StringTest::getB8String() {
        return b8String;
    }

    std::u16string& StringTest::getA16String() {
        return a16String;
    }

    std::u32string& StringTest::getA32String() {
        return a32String;
    }

    std::array<std::string, 7>& StringTest::getArray() {
        return array;
    }

    void StringTest::setA8String(const std::string& value) {
        a8String = value;
    }

    void StringTest::setB8String(const std::string& value) {
        b8String = value;
    }

    void StringTest::setA16String(const std::u16string& value) {
        a16String = value;
    }

    void StringTest::setA32String(const std::u32string& value) {
        a32String = value;
    }

}
