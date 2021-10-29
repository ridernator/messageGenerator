#include "TestStructure.hxx"

#include <string.h>

namespace arse {
    TestStructure::TestStructure() {

    }

    TestStructure::~TestStructure() {

    }

    void TestStructure::serialise(char* data, uint64_t& offset) const {
        // Serialise int8Member
        memcpy(data + offset, &int8Member, sizeof(int8_t));
        offset += sizeof(int8Member);

        // Serialise int16Member
        memcpy(data + offset, &int16Member, sizeof(int16_t));
        offset += sizeof(int16Member);

        // Serialise int32Member
        memcpy(data + offset, &int32Member, sizeof(int32_t));
        offset += sizeof(int32Member);

        // Serialise int64Member
        memcpy(data + offset, &int64Member, sizeof(int64_t));
        offset += sizeof(int64Member);

        // Serialise colour1
        memcpy(data + offset, &colour1, sizeof(Colour));
        offset += sizeof(Colour);

        // Serialise colour2
        memcpy(data + offset, &colour2, sizeof(Colour));
        offset += sizeof(Colour);

        // Serialise colour3
        memcpy(data + offset, &colour3, sizeof(Colour));
        offset += sizeof(Colour);

        // Serialise TSS
        TSS.serialise(data + offset, offset);

        // Serialise testArray
        for (const auto& e1 : testArray) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    memcpy(data + offset, e3.data(), sizeof(uint16_t) * e3.size());
                    offset += sizeof(uint16_t) * e3.size();
                }
            }
        }

        // Serialise testArrayStruct
        for (const auto& e1 : testArrayStruct) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    for (const auto& e4 : e3) {
                        e4.serialise(data + offset, offset);
                    }
                }
            }
        }

        // Serialise testArrayEnum
        for (const auto& e1 : testArrayEnum) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    memcpy(data + offset, e3.data(), sizeof(Colour) * e3.size());
                    offset += sizeof(Colour) * e3.size();
                }
            }
        }

        // Serialise testArraySeq
        for (const auto& e1 : testArraySeq) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                }
            }
        }
    }

    void TestStructure::deserialise(const char* data, uint64_t& offset) {
        // Deserialise int8Member
        memcpy(&int8Member, data + offset, sizeof(int8_t));
        offset += sizeof(int8Member);

        // Deserialise int16Member
        memcpy(&int16Member, data + offset, sizeof(int16_t));
        offset += sizeof(int16Member);

        // Deserialise int32Member
        memcpy(&int32Member, data + offset, sizeof(int32_t));
        offset += sizeof(int32Member);

        // Deserialise int64Member
        memcpy(&int64Member, data + offset, sizeof(int64_t));
        offset += sizeof(int64Member);

        // Deserialise colour1
        memcpy(&colour1, data + offset, sizeof(Colour));
        offset += sizeof(Colour);

        // Deserialise colour2
        memcpy(&colour2, data + offset, sizeof(Colour));
        offset += sizeof(Colour);

        // Deserialise colour3
        memcpy(&colour3, data + offset, sizeof(Colour));
        offset += sizeof(Colour);

        // Deserialise TSS
        TSS.deserialise(data + offset, offset);

        // Deserialise testArray
        for (auto& e1 : testArray) {
            for (auto& e2 : e1) {
                for (auto& e3 : e2) {
                    memcpy(e3.data(), data + offset, sizeof(uint16_t) * e3.size());
                    offset += sizeof(uint16_t) * e3.size();
                }
            }
        }

        // Deserialise testArrayStruct
        for (auto& e1 : testArrayStruct) {
            for (auto& e2 : e1) {
                for (auto& e3 : e2) {
                    for (auto& e4 : e3) {
                        e4.deserialise(data + offset, offset);
                    }
                }
            }
        }

        // Deserialise testArrayEnum
        for (auto& e1 : testArrayEnum) {
            for (auto& e2 : e1) {
                for (auto& e3 : e2) {
                    memcpy(e3.data(), data + offset, sizeof(Colour) * e3.size());
                    offset += sizeof(Colour) * e3.size();
                }
            }
        }

        // Deserialise testArraySeq
        for (auto& e1 : testArraySeq) {
            for (auto& e2 : e1) {
                for (auto& e3 : e2) {
                }
            }
        }
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

        // Add on size of testArraySeq

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

    std::array<std::array<std::array<std::array<std::vector<Colour>, 2>, 3>, 4>, 5>& TestStructure::getTestArraySeq() {
        return testArraySeq;
    }

    std::vector<float>& TestStructure::getPrimitiveSequence() {
        return primitiveSequence;
    }

    std::vector<Colour>& TestStructure::getEnumerationSequence() {
        return enumerationSequence;
    }

    std::vector<TestSubStruct>& TestStructure::getStructureSequence() {
        return structureSequence;
    }

    std::vector<std::array<std::array<uint8_t, 4>, 3>>& TestStructure::getArraySequence() {
        return arraySequence;
    }

    std::vector<std::vector<double>>& TestStructure::getSequenceSequence() {
        return sequenceSequence;
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
