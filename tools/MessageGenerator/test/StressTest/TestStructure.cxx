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

        // Serialise colour1
        memcpy(data + offset, &colour1, sizeof (Colour));
        offset += sizeof (Colour);

        // Serialise colour2
        memcpy(data + offset, &colour2, sizeof (Colour));
        offset += sizeof (Colour);

        // Serialise colour3
        memcpy(data + offset, &colour3, sizeof (Colour));
        offset += sizeof (Colour);

        // Serialise TSS
        TSS.serialise(data, offset);
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

        // Deserialise colour1
        memcpy(&colour1, data + offset, sizeof (Colour));
        offset += sizeof (Colour);

        // Deserialise colour2
        memcpy(&colour2, data + offset, sizeof (Colour));
        offset += sizeof (Colour);

        // Deserialise colour3
        memcpy(&colour3, data + offset, sizeof (Colour));
        offset += sizeof (Colour);

        // Deserialise TSS
        TSS.deserialise(data, offset);
    }

    uint64_t TestStructure::getSizeInBytes() const {
        // Size of primitive types in this structure
        uint64_t size = 18;

        // Add on size of TSS
        size += TSS.getSizeInBytes();

        // Add on size of testArray
        size += 240;

        // Add on size of testArrayStruct
        for (const auto& e1 : testArrayStruct) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    for (const auto& e4 : e3) {
                        size += e4.getSizeInBytes();
                    }
                }
            }
        }

        // Add on size of testArrayEnum
        size += 120;

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

    Colour TestStructure::getColour1() const {
        return colour1;
    }

    Colour TestStructure::getColour2() const {
        return colour2;
    }

    Colour TestStructure::getColour3() const {
        return colour3;
    }

    TestSubStruct& TestStructure::getTSS() {
        return TSS;
    }

    std::array<std::array<std::array<std::array<uint16_t, 2>, 3>, 4>, 5>& TestStructure::getTestArray() {
        return testArray;
    }

    std::array<std::array<std::array<std::array<TestSubStruct, 2>, 3>, 4>, 5>& TestStructure::getTestArrayStruct() {
        return testArrayStruct;
    }

    std::array<std::array<std::array<std::array<Colour, 2>, 3>, 4>, 5>& TestStructure::getTestArrayEnum() {
        return testArrayEnum;
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

    void TestStructure::setColour1(const Colour value) {
        colour1 = value;
    }

    void TestStructure::setColour2(const Colour value) {
        colour2 = value;
    }

    void TestStructure::setColour3(const Colour value) {
        colour3 = value;
    }

}
