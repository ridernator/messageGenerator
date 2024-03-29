#include "MapTest.hxx"

#include <string.h>

namespace MyNamespace {
    MapTest::MapTest() {

    }

    MapTest::~MapTest() {

    }

    void MapTest::serialise(char* data, uint64_t& offset) const {
        // Serialise primitiveToPrimitive
        // Serialise size of primitiveToPrimitive
        uint64_t primitiveToPrimitiveSize = primitiveToPrimitive.size();
        memcpy(data + offset, &primitiveToPrimitiveSize, sizeof(primitiveToPrimitiveSize));
        offset += sizeof(primitiveToPrimitiveSize);

        for (const auto& e0 : primitiveToPrimitive) {
            // Serialise primitiveToPrimitive key data
            memcpy(data + offset, &e0.first, sizeof(e0.first));
            offset += sizeof(e0.first);

            // Serialise primitiveToPrimitive value data
            memcpy(data + offset, &e0.second, sizeof(e0.second));
            offset += sizeof(e0.second);
        }

        // Serialise primitiveToEnum
        // Serialise size of primitiveToEnum
        uint64_t primitiveToEnumSize = primitiveToEnum.size();
        memcpy(data + offset, &primitiveToEnumSize, sizeof(primitiveToEnumSize));
        offset += sizeof(primitiveToEnumSize);

        for (const auto& e0 : primitiveToEnum) {
            // Serialise primitiveToEnum key data
            memcpy(data + offset, &e0.first, sizeof(e0.first));
            offset += sizeof(e0.first);

            // Serialise primitiveToEnum value data
            memcpy(data + offset, &e0.second, sizeof(e0.second));
            offset += sizeof(e0.second);
        }

        // Serialise primitiveToStruct
        // Serialise size of primitiveToStruct
        uint64_t primitiveToStructSize = primitiveToStruct.size();
        memcpy(data + offset, &primitiveToStructSize, sizeof(primitiveToStructSize));
        offset += sizeof(primitiveToStructSize);

        for (const auto& e0 : primitiveToStruct) {
            // Serialise primitiveToStruct key data
            memcpy(data + offset, &e0.first, sizeof(e0.first));
            offset += sizeof(e0.first);

            // Serialise primitiveToStruct value data
            e0.second.serialise(data, offset);
        }

        // Serialise primitiveToArray
        // Serialise size of primitiveToArray
        uint64_t primitiveToArraySize = primitiveToArray.size();
        memcpy(data + offset, &primitiveToArraySize, sizeof(primitiveToArraySize));
        offset += sizeof(primitiveToArraySize);

        for (const auto& e0 : primitiveToArray) {
            // Serialise primitiveToArray key data
            memcpy(data + offset, &e0.first, sizeof(e0.first));
            offset += sizeof(e0.first);

            // Serialise primitiveToArray value data
            for (const auto& e2 : e0.second) {
                memcpy(data + offset, &e2[0], sizeof(e2[0]) * e2.size());
                offset += sizeof(e2[0]) * e2.size();
            }
        }

        // Serialise primitiveToSequence
        // Serialise size of primitiveToSequence
        uint64_t primitiveToSequenceSize = primitiveToSequence.size();
        memcpy(data + offset, &primitiveToSequenceSize, sizeof(primitiveToSequenceSize));
        offset += sizeof(primitiveToSequenceSize);

        for (const auto& e0 : primitiveToSequence) {
            // Serialise primitiveToSequence key data
            memcpy(data + offset, &e0.first, sizeof(e0.first));
            offset += sizeof(e0.first);

            // Serialise primitiveToSequence value data
            // Serialise size of e0.second
            uint64_t e0secondSize = e0.second.size();
            memcpy(data + offset, &e0secondSize, sizeof(e0secondSize));
            offset += sizeof(e0secondSize);

            // Serialise e0.second data
            for (const auto& e2 : e0.second) {
                // Serialise size of e2
                uint64_t e2Size = e2.size();
                memcpy(data + offset, &e2Size, sizeof(e2Size));
                offset += sizeof(e2Size);

                // Serialise e2 data
                memcpy(data + offset, &e2[0], sizeof(e2[0]) * e2Size);
                offset += sizeof(e2[0]) * e2Size;
            }
        }

        // Serialise primitiveToMap
        // Serialise size of primitiveToMap
        uint64_t primitiveToMapSize = primitiveToMap.size();
        memcpy(data + offset, &primitiveToMapSize, sizeof(primitiveToMapSize));
        offset += sizeof(primitiveToMapSize);

        for (const auto& e0 : primitiveToMap) {
            // Serialise primitiveToMap key data
            memcpy(data + offset, &e0.first, sizeof(e0.first));
            offset += sizeof(e0.first);

            // Serialise primitiveToMap value data
            // Serialise size of e0.second
            uint64_t e0secondSize = e0.second.size();
            memcpy(data + offset, &e0secondSize, sizeof(e0secondSize));
            offset += sizeof(e0secondSize);

            for (const auto& e1 : e0.second) {
                // Serialise e0second key data
                memcpy(data + offset, &e1.first, sizeof(e1.first));
                offset += sizeof(e1.first);

                // Serialise e0second value data
                // Serialise size of e1.second
                uint64_t e1secondSize = e1.second.size();
                memcpy(data + offset, &e1secondSize, sizeof(e1secondSize));
                offset += sizeof(e1secondSize);

                for (const auto& e2 : e1.second) {
                    // Serialise e1second key data
                    memcpy(data + offset, &e2.first, sizeof(e2.first));
                    offset += sizeof(e2.first);

                    // Serialise e1second value data
                    memcpy(data + offset, &e2.second, sizeof(e2.second));
                    offset += sizeof(e2.second);
                }
            }
        }

        // Serialise stringTo32String
        // Serialise size of stringTo32String
        uint64_t stringTo32StringSize = stringTo32String.size();
        memcpy(data + offset, &stringTo32StringSize, sizeof(stringTo32StringSize));
        offset += sizeof(stringTo32StringSize);

        for (const auto& e0 : stringTo32String) {
            // Serialise stringTo32String key data
            // Serialise size of e0.first
            uint64_t e0firstSize = e0.first.size();
            memcpy(data + offset, &e0firstSize, sizeof(e0firstSize));
            offset += sizeof(e0firstSize);

            // Serialise e0.first data
            memcpy(data + offset, &e0.first[0], sizeof(e0.first[0]) * e0.first.size());
            offset += sizeof(e0.first[0]) * e0.first.size();

            // Serialise stringTo32String value data
            // Serialise size of e0.second
            uint64_t e0secondSize = e0.second.size();
            memcpy(data + offset, &e0secondSize, sizeof(e0secondSize));
            offset += sizeof(e0secondSize);

            // Serialise e0.second data
            memcpy(data + offset, &e0.second[0], sizeof(e0.second[0]) * e0.second.size());
            offset += sizeof(e0.second[0]) * e0.second.size();
        }
    }

    void MapTest::deserialise(const char* data, uint64_t& offset) {
        // Deserialise primitiveToPrimitive
        // Deserialise size of primitiveToPrimitive
        uint64_t primitiveToPrimitiveSize;
        memcpy(&primitiveToPrimitiveSize, data + offset, sizeof(primitiveToPrimitiveSize));
        offset += sizeof(primitiveToPrimitiveSize);

        // Deserialise primitiveToPrimitive data
        for (uint64_t index = 0; index < primitiveToPrimitiveSize; ++index) {
            // Deserialise primitiveToPrimitive key data
            uint8_t first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise primitiveToPrimitive value data
            float second0;
            memcpy(&second0, data + offset, sizeof(second0));
            offset += sizeof(second0);

            primitiveToPrimitive.insert({first0, second0});
        }

        // Deserialise primitiveToEnum
        // Deserialise size of primitiveToEnum
        uint64_t primitiveToEnumSize;
        memcpy(&primitiveToEnumSize, data + offset, sizeof(primitiveToEnumSize));
        offset += sizeof(primitiveToEnumSize);

        // Deserialise primitiveToEnum data
        for (uint64_t index = 0; index < primitiveToEnumSize; ++index) {
            // Deserialise primitiveToEnum key data
            uint8_t first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise primitiveToEnum value data
            Colour second0;
            memcpy(&second0, data + offset, sizeof(second0));
            offset += sizeof(second0);

            primitiveToEnum.insert({first0, second0});
        }

        // Deserialise primitiveToStruct
        // Deserialise size of primitiveToStruct
        uint64_t primitiveToStructSize;
        memcpy(&primitiveToStructSize, data + offset, sizeof(primitiveToStructSize));
        offset += sizeof(primitiveToStructSize);

        // Deserialise primitiveToStruct data
        for (uint64_t index = 0; index < primitiveToStructSize; ++index) {
            // Deserialise primitiveToStruct key data
            uint8_t first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise primitiveToStruct value data
            TestSubStruct second0;
            second0.deserialise(data, offset);

            primitiveToStruct.insert({first0, second0});
        }

        // Deserialise primitiveToArray
        // Deserialise size of primitiveToArray
        uint64_t primitiveToArraySize;
        memcpy(&primitiveToArraySize, data + offset, sizeof(primitiveToArraySize));
        offset += sizeof(primitiveToArraySize);

        // Deserialise primitiveToArray data
        for (uint64_t index = 0; index < primitiveToArraySize; ++index) {
            // Deserialise primitiveToArray key data
            uint8_t first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise primitiveToArray value data
            std::array<std::array<uint8_t, 2>, 2> second0;
            for (auto& e2 : second0) {
                memcpy(&e2[0], data + offset, sizeof(e2[0]) * e2.size());
                offset += sizeof(e2[0]) * e2.size();
            }

            primitiveToArray.insert({first0, second0});
        }

        // Deserialise primitiveToSequence
        // Deserialise size of primitiveToSequence
        uint64_t primitiveToSequenceSize;
        memcpy(&primitiveToSequenceSize, data + offset, sizeof(primitiveToSequenceSize));
        offset += sizeof(primitiveToSequenceSize);

        // Deserialise primitiveToSequence data
        for (uint64_t index = 0; index < primitiveToSequenceSize; ++index) {
            // Deserialise primitiveToSequence key data
            uint8_t first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise primitiveToSequence value data
            std::vector<std::vector<uint8_t>> second0;
            // Deserialise size of second0
            uint64_t second0Size;
            memcpy(&second0Size, data + offset, sizeof(second0Size));
            offset += sizeof(second0Size);

            // Deserialise second0 data
            second0.resize(second0Size);
            for (auto& e2 : second0) {
                // Deserialise size of e2
                uint64_t e2Size;
                memcpy(&e2Size, data + offset, sizeof(e2Size));
                offset += sizeof(e2Size);

                // Deserialise e2 data
                e2.resize(e2Size);
                memcpy(&e2[0], data + offset, sizeof(e2[0]) * e2Size);
                offset += sizeof(e2[0]) * e2Size;
            }

            primitiveToSequence.insert({first0, second0});
        }

        // Deserialise primitiveToMap
        // Deserialise size of primitiveToMap
        uint64_t primitiveToMapSize;
        memcpy(&primitiveToMapSize, data + offset, sizeof(primitiveToMapSize));
        offset += sizeof(primitiveToMapSize);

        // Deserialise primitiveToMap data
        for (uint64_t index = 0; index < primitiveToMapSize; ++index) {
            // Deserialise primitiveToMap key data
            uint8_t first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise primitiveToMap value data
            std::map<uint16_t, std::map<uint16_t, Colour>> second0;
            // Deserialise size of second0
            uint64_t second0Size;
            memcpy(&second0Size, data + offset, sizeof(second0Size));
            offset += sizeof(second0Size);

            // Deserialise second0 data
            for (uint64_t index = 0; index < second0Size; ++index) {
                // Deserialise second0 key data
                uint16_t first1;
                memcpy(&first1, data + offset, sizeof(first1));
                offset += sizeof(first1);

                // Deserialise second0 value data
                std::map<uint16_t, Colour> second1;
                // Deserialise size of second1
                uint64_t second1Size;
                memcpy(&second1Size, data + offset, sizeof(second1Size));
                offset += sizeof(second1Size);

                // Deserialise second1 data
                for (uint64_t index = 0; index < second1Size; ++index) {
                    // Deserialise second1 key data
                    uint16_t first2;
                    memcpy(&first2, data + offset, sizeof(first2));
                    offset += sizeof(first2);

                    // Deserialise second1 value data
                    Colour second2;
                    memcpy(&second2, data + offset, sizeof(second2));
                    offset += sizeof(second2);

                    second1.insert({first2, second2});
                }

                second0.insert({first1, second1});
            }

            primitiveToMap.insert({first0, second0});
        }

        // Deserialise stringTo32String
        // Deserialise size of stringTo32String
        uint64_t stringTo32StringSize;
        memcpy(&stringTo32StringSize, data + offset, sizeof(stringTo32StringSize));
        offset += sizeof(stringTo32StringSize);

        // Deserialise stringTo32String data
        for (uint64_t index = 0; index < stringTo32StringSize; ++index) {
            // Deserialise stringTo32String key data
            std::string first0;
            // Deserialise size of first0
            uint64_t first0Size;
            memcpy(&first0Size, data + offset, sizeof(first0Size));
            offset += sizeof(first0Size);
            first0.resize(first0Size);

            // Deserialise first0 data
            memcpy(&first0[0], data + offset, sizeof(first0[0]) * first0.size());
            offset += sizeof(first0[0]) * first0.size();

            // Deserialise stringTo32String value data
            std::u32string second0;
            // Deserialise size of second0
            uint64_t second0Size;
            memcpy(&second0Size, data + offset, sizeof(second0Size));
            offset += sizeof(second0Size);
            second0.resize(second0Size);

            // Deserialise second0 data
            memcpy(&second0[0], data + offset, sizeof(second0[0]) * second0.size());
            offset += sizeof(second0[0]) * second0.size();

            stringTo32String.insert({first0, second0});
        }
    }

    uint64_t MapTest::getSizeInBytes() const {
        uint64_t size = 0;

        // Add on size of primitiveToPrimitive
        // Add on size of primitiveToPrimitive length field
        size += sizeof(uint64_t);

        // Add on size of primitiveToPrimitive key data
        size += sizeof(uint8_t) * primitiveToPrimitive.size();

        // Add on size of primitiveToPrimitive value data
        size += sizeof(float) * primitiveToPrimitive.size();

        // Add on size of primitiveToEnum
        // Add on size of primitiveToEnum length field
        size += sizeof(uint64_t);

        // Add on size of primitiveToEnum key data
        size += sizeof(uint8_t) * primitiveToEnum.size();

        // Add on size of primitiveToEnum value data
        size += sizeof(Colour) * primitiveToEnum.size();

        // Add on size of primitiveToStruct
        // Add on size of primitiveToStruct length field
        size += sizeof(uint64_t);

        // Add on size of primitiveToStruct key data
        size += sizeof(uint8_t) * primitiveToStruct.size();

        // Add on size of primitiveToStruct value data
        for (const auto& e1 : primitiveToStruct) {
            // Add on size of each individual TestSubStruct
            size += e1.second.getSizeInBytes();
        }

        // Add on size of primitiveToArray
        // Add on size of primitiveToArray length field
        size += sizeof(uint64_t);

        // Add on size of primitiveToArray key data
        size += sizeof(uint8_t) * primitiveToArray.size();

        // Add on size of primitiveToArray value data
        for (const auto& e1 : primitiveToArray) {
            for (const auto& e2 : e1.second) {
                size += sizeof(uint8_t) * e2.size();
            }
        }

        // Add on size of primitiveToSequence
        // Add on size of primitiveToSequence length field
        size += sizeof(uint64_t);

        // Add on size of primitiveToSequence key data
        size += sizeof(uint8_t) * primitiveToSequence.size();

        // Add on size of primitiveToSequence value data
        for (const auto& e1 : primitiveToSequence) {
            // Add on size of e1.second length field
            size += sizeof(uint64_t);

            // Add on size of e1.second data
            for (const auto& e2 : e1.second) {
                // Add on size of e2 length field
                size += sizeof(uint64_t);

                // Add on size of e2 data
                size += sizeof(uint8_t) * e2.size();
            }
        }

        // Add on size of primitiveToMap
        // Add on size of primitiveToMap length field
        size += sizeof(uint64_t);

        // Add on size of primitiveToMap key data
        size += sizeof(uint8_t) * primitiveToMap.size();

        // Add on size of primitiveToMap value data
        for (const auto& e1 : primitiveToMap) {
            // Add on size of e1.second length field
            size += sizeof(uint64_t);

            // Add on size of e1.second key data
            size += sizeof(uint16_t) * e1.second.size();

            // Add on size of e1.second value data
            for (const auto& e2 : e1.second) {
                // Add on size of e2.second length field
                size += sizeof(uint64_t);

                // Add on size of e2.second key data
                size += sizeof(uint16_t) * e2.second.size();

                // Add on size of e2.second value data
                size += sizeof(Colour) * e2.second.size();
            }
        }

        // Add on size of stringTo32String
        // Add on size of stringTo32String length field
        size += sizeof(uint64_t);

        // Add on size of stringTo32String key data
        for (const auto& e1 : stringTo32String) {
            // Add on size of each individual std::string
            size += sizeof(std::string::value_type) * e1.first.size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        // Add on size of stringTo32String value data
        for (const auto& e1 : stringTo32String) {
            // Add on size of each individual std::u32string
            size += sizeof(std::u32string::value_type) * e1.second.size();

            // Add on size of string size field
            size += sizeof(uint64_t);
        }

        return size;
    }

    std::map<uint8_t, float>& MapTest::getPrimitiveToPrimitive() {
        return primitiveToPrimitive;
    }

    std::map<uint8_t, Colour>& MapTest::getPrimitiveToEnum() {
        return primitiveToEnum;
    }

    std::map<uint8_t, TestSubStruct>& MapTest::getPrimitiveToStruct() {
        return primitiveToStruct;
    }

    std::map<uint8_t, std::array<std::array<uint8_t, 2>, 2>>& MapTest::getPrimitiveToArray() {
        return primitiveToArray;
    }

    std::map<uint8_t, std::vector<std::vector<uint8_t>>>& MapTest::getPrimitiveToSequence() {
        return primitiveToSequence;
    }

    std::map<uint8_t, std::map<uint16_t, std::map<uint16_t, Colour>>>& MapTest::getPrimitiveToMap() {
        return primitiveToMap;
    }

    std::map<std::string, std::u32string>& MapTest::getStringTo32String() {
        return stringTo32String;
    }

}
