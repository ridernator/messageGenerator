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

        // Serialise lastElement
        memcpy(data + offset, &lastElement, sizeof(int64_t));
        offset += sizeof(lastElement);

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
                memcpy(data + offset, &e2[0], sizeof(uint64_t) * e2.size());
                offset += sizeof(uint64_t) * e2.size();
            }
        }

        // Serialise testArrayStruct
        for (const auto& e1 : testArrayStruct) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    e3.serialise(data + offset, offset);
                }
            }
        }

        // Serialise testArrayEnum
        for (const auto& e1 : testArrayEnum) {
            for (const auto& e2 : e1) {
                memcpy(data + offset, &e2[0], sizeof(Colour) * e2.size());
                offset += sizeof(Colour) * e2.size();
            }
        }

        // Serialise testArraySeq
        for (const auto& e1 : testArraySeq) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    // Serialise size of e3
                    uint64_t e3Size = e3.size();
                    memcpy(data + offset, &e3Size, sizeof(e3Size));
                    offset += sizeof(e3Size);

                    // Serialise e3 data
                    memcpy(data + offset, &e3[0], sizeof(Colour) * e3Size);
                    offset += sizeof(Colour) * e3Size;
                }
            }
        }

        // Serialise testArrayMap
        for (const auto& e1 : testArrayMap) {
            for (const auto& e2 : e1) {
                // Serialise size of e2
                uint64_t e2Size = e2.size();
                memcpy(data + offset, &e2Size, sizeof(e2Size));
                offset += sizeof(e2Size);

                for (const auto& e2 : e2) {
                    // Serialise e2 key data
                    memcpy(data + offset, &e2.first, sizeof(float));
                    offset += sizeof(float);

                    // Serialise e2 value data
                    memcpy(data + offset, &e2.second, sizeof(double));
                    offset += sizeof(double);
                }
            }
        }

        // Serialise primitiveSequence
        // Serialise size of primitiveSequence
        uint64_t primitiveSequenceSize = primitiveSequence.size();
        memcpy(data + offset, &primitiveSequenceSize, sizeof(primitiveSequenceSize));
        offset += sizeof(primitiveSequenceSize);

        // Serialise primitiveSequence data
        memcpy(data + offset, &primitiveSequence[0], sizeof(float) * primitiveSequenceSize);
        offset += sizeof(float) * primitiveSequenceSize;

        // Serialise enumerationSequence
        // Serialise size of enumerationSequence
        uint64_t enumerationSequenceSize = enumerationSequence.size();
        memcpy(data + offset, &enumerationSequenceSize, sizeof(enumerationSequenceSize));
        offset += sizeof(enumerationSequenceSize);

        // Serialise enumerationSequence data
        memcpy(data + offset, &enumerationSequence[0], sizeof(Colour) * enumerationSequenceSize);
        offset += sizeof(Colour) * enumerationSequenceSize;

        // Serialise structureSequence
        // Serialise size of structureSequence
        uint64_t structureSequenceSize = structureSequence.size();
        memcpy(data + offset, &structureSequenceSize, sizeof(structureSequenceSize));
        offset += sizeof(structureSequenceSize);

        // Serialise structureSequence data
        for (const auto& e1 : structureSequence) {
            e1.serialise(data + offset, offset);
        }

        // Serialise arraySequence
        // Serialise size of arraySequence
        uint64_t arraySequenceSize = arraySequence.size();
        memcpy(data + offset, &arraySequenceSize, sizeof(arraySequenceSize));
        offset += sizeof(arraySequenceSize);

        // Serialise arraySequence data
        for (const auto& e1 : arraySequence) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    memcpy(data + offset, &e3[0], sizeof(uint16_t) * e3.size());
                    offset += sizeof(uint16_t) * e3.size();
                }
            }
        }

        // Serialise sequenceSequence
        // Serialise size of sequenceSequence
        uint64_t sequenceSequenceSize = sequenceSequence.size();
        memcpy(data + offset, &sequenceSequenceSize, sizeof(sequenceSequenceSize));
        offset += sizeof(sequenceSequenceSize);

        // Serialise sequenceSequence data
        for (const auto& e1 : sequenceSequence) {
            // Serialise size of e1
            uint64_t e1Size = e1.size();
            memcpy(data + offset, &e1Size, sizeof(e1Size));
            offset += sizeof(e1Size);

            // Serialise e1 data
            memcpy(data + offset, &e1[0], sizeof(double) * e1Size);
            offset += sizeof(double) * e1Size;
        }

        // Serialise mapSequence
        // Serialise size of mapSequence
        uint64_t mapSequenceSize = mapSequence.size();
        memcpy(data + offset, &mapSequenceSize, sizeof(mapSequenceSize));
        offset += sizeof(mapSequenceSize);

        // Serialise mapSequence data
        for (const auto& e1 : mapSequence) {
            // Serialise size of e1
            uint64_t e1Size = e1.size();
            memcpy(data + offset, &e1Size, sizeof(e1Size));
            offset += sizeof(e1Size);

            for (const auto& e1 : e1) {
                // Serialise e1 key data
                memcpy(data + offset, &e1.first, sizeof(float));
                offset += sizeof(float);

                // Serialise e1 value data
                memcpy(data + offset, &e1.second, sizeof(double));
                offset += sizeof(double);
            }
        }

        // Serialise mapPrimitiveToEnum
        // Serialise size of mapPrimitiveToEnum
        uint64_t mapPrimitiveToEnumSize = mapPrimitiveToEnum.size();
        memcpy(data + offset, &mapPrimitiveToEnumSize, sizeof(mapPrimitiveToEnumSize));
        offset += sizeof(mapPrimitiveToEnumSize);

        for (const auto& e0 : mapPrimitiveToEnum) {
            // Serialise mapPrimitiveToEnum key data
            memcpy(data + offset, &e0.first, sizeof(uint16_t));
            offset += sizeof(uint16_t);

            // Serialise mapPrimitiveToEnum value data
            memcpy(data + offset, &e0.second, sizeof(Colour));
            offset += sizeof(Colour);
        }

        // Serialise mapEnumToEnum
        // Serialise size of mapEnumToEnum
        uint64_t mapEnumToEnumSize = mapEnumToEnum.size();
        memcpy(data + offset, &mapEnumToEnumSize, sizeof(mapEnumToEnumSize));
        offset += sizeof(mapEnumToEnumSize);

        for (const auto& e0 : mapEnumToEnum) {
            // Serialise mapEnumToEnum key data
            memcpy(data + offset, &e0.first, sizeof(Colour));
            offset += sizeof(Colour);

            // Serialise mapEnumToEnum value data
            memcpy(data + offset, &e0.second, sizeof(Colour));
            offset += sizeof(Colour);
        }

        // Serialise mapEnumToArray
        // Serialise size of mapEnumToArray
        uint64_t mapEnumToArraySize = mapEnumToArray.size();
        memcpy(data + offset, &mapEnumToArraySize, sizeof(mapEnumToArraySize));
        offset += sizeof(mapEnumToArraySize);

        for (const auto& e0 : mapEnumToArray) {
            // Serialise mapEnumToArray key data
            memcpy(data + offset, &e0.first, sizeof(Colour));
            offset += sizeof(Colour);

            // Serialise mapEnumToArray value data
            for (const auto& e2 : e0.second) {
                memcpy(data + offset, &e2[0], sizeof(Colour) * e2.size());
                offset += sizeof(Colour) * e2.size();
            }
        }

        // Serialise mapEnumToSequence
        // Serialise size of mapEnumToSequence
        uint64_t mapEnumToSequenceSize = mapEnumToSequence.size();
        memcpy(data + offset, &mapEnumToSequenceSize, sizeof(mapEnumToSequenceSize));
        offset += sizeof(mapEnumToSequenceSize);

        for (const auto& e0 : mapEnumToSequence) {
            // Serialise mapEnumToSequence key data
            memcpy(data + offset, &e0.first, sizeof(Colour));
            offset += sizeof(Colour);

            // Serialise mapEnumToSequence value data
            // Serialise size of e0.second
            uint64_t e0_secondSize = e0.second.size();
            memcpy(data + offset, &e0_secondSize, sizeof(e0_secondSize));
            offset += sizeof(e0_secondSize);

            // Serialise e0.second data
            memcpy(data + offset, &e0.second[0], sizeof(uint64_t) * e0_secondSize);
            offset += sizeof(uint64_t) * e0_secondSize;
        }

        // Serialise mapEnumToMap
        // Serialise size of mapEnumToMap
        uint64_t mapEnumToMapSize = mapEnumToMap.size();
        memcpy(data + offset, &mapEnumToMapSize, sizeof(mapEnumToMapSize));
        offset += sizeof(mapEnumToMapSize);

        for (const auto& e0 : mapEnumToMap) {
            // Serialise mapEnumToMap key data
            memcpy(data + offset, &e0.first, sizeof(Colour));
            offset += sizeof(Colour);

            // Serialise mapEnumToMap value data
            // Serialise size of e0.second
            uint64_t e0_secondSize = e0.second.size();
            memcpy(data + offset, &e0_secondSize, sizeof(e0_secondSize));
            offset += sizeof(e0_secondSize);

            for (const auto& e1 : e0.second) {
                // Serialise e0_second key data
                memcpy(data + offset, &e1.first, sizeof(uint8_t));
                offset += sizeof(uint8_t);

                // Serialise e0_second value data
                memcpy(data + offset, &e1.second, sizeof(int16_t));
                offset += sizeof(int16_t);
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

        // Deserialise lastElement
        memcpy(&lastElement, data + offset, sizeof(int64_t));
        offset += sizeof(lastElement);

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
                memcpy(&e2[0], data + offset, sizeof(uint64_t) * e2.size());
                offset += sizeof(uint64_t) * e2.size();
            }
        }

        // Deserialise testArrayStruct
        for (auto& e1 : testArrayStruct) {
            for (auto& e2 : e1) {
                for (auto& e3 : e2) {
                    e3.deserialise(data + offset, offset);
                }
            }
        }

        // Deserialise testArrayEnum
        for (auto& e1 : testArrayEnum) {
            for (auto& e2 : e1) {
                memcpy(&e2[0], data + offset, sizeof(Colour) * e2.size());
                offset += sizeof(Colour) * e2.size();
            }
        }

        // Deserialise testArraySeq
        for (auto& e1 : testArraySeq) {
            for (auto& e2 : e1) {
                for (auto& e3 : e2) {
                    // Deserialise size of e3
                    uint64_t e3Size;
                    memcpy(&e3Size, data + offset, sizeof(e3Size));
                    offset += sizeof(e3Size);

                    // Deserialise e3 data
                    e3.resize(e3Size);
                    memcpy(&e3[0], data + offset, sizeof(Colour) * e3Size);
                    offset += sizeof(Colour) * e3Size;
                }
            }
        }

        // Deserialise testArrayMap
        for (auto& e1 : testArrayMap) {
            for (auto& e2 : e1) {
                // Deserialise size of e2
                uint64_t e2Size;
                memcpy(&e2Size, data + offset, sizeof(e2Size));
                offset += sizeof(e2Size);

                // Deserialise e2 data
                for (uint64_t index = 0; index < e2Size; ++index) {
                    // Deserialise e2 key data
                    float first2;
                    memcpy(&first2, data + offset, sizeof(first2));
                    offset += sizeof(first2);

                    // Deserialise e2 value data
                    double second2;
                    memcpy(&second2, data + offset, sizeof(second2));
                    offset += sizeof(second2);

                    e2.insert({first2, second2});
                }
            }
        }

        // Deserialise primitiveSequence
        // Deserialise size of primitiveSequence
        uint64_t primitiveSequenceSize;
        memcpy(&primitiveSequenceSize, data + offset, sizeof(primitiveSequenceSize));
        offset += sizeof(primitiveSequenceSize);

        // Deserialise primitiveSequence data
        primitiveSequence.resize(primitiveSequenceSize);
        memcpy(&primitiveSequence[0], data + offset, sizeof(float) * primitiveSequenceSize);
        offset += sizeof(float) * primitiveSequenceSize;

        // Deserialise enumerationSequence
        // Deserialise size of enumerationSequence
        uint64_t enumerationSequenceSize;
        memcpy(&enumerationSequenceSize, data + offset, sizeof(enumerationSequenceSize));
        offset += sizeof(enumerationSequenceSize);

        // Deserialise enumerationSequence data
        enumerationSequence.resize(enumerationSequenceSize);
        memcpy(&enumerationSequence[0], data + offset, sizeof(Colour) * enumerationSequenceSize);
        offset += sizeof(Colour) * enumerationSequenceSize;

        // Deserialise structureSequence
        // Deserialise size of structureSequence
        uint64_t structureSequenceSize;
        memcpy(&structureSequenceSize, data + offset, sizeof(structureSequenceSize));
        offset += sizeof(structureSequenceSize);

        // Deserialise structureSequence data
        structureSequence.resize(structureSequenceSize);
        for (auto& e1 : structureSequence) {
            e1.deserialise(data + offset, offset);
        }

        // Deserialise arraySequence
        // Deserialise size of arraySequence
        uint64_t arraySequenceSize;
        memcpy(&arraySequenceSize, data + offset, sizeof(arraySequenceSize));
        offset += sizeof(arraySequenceSize);

        // Deserialise arraySequence data
        arraySequence.resize(arraySequenceSize);
        for (auto& e1 : arraySequence) {
            for (auto& e2 : e1) {
                for (auto& e3 : e2) {
                    memcpy(&e3[0], data + offset, sizeof(uint16_t) * e3.size());
                    offset += sizeof(uint16_t) * e3.size();
                }
            }
        }

        // Deserialise sequenceSequence
        // Deserialise size of sequenceSequence
        uint64_t sequenceSequenceSize;
        memcpy(&sequenceSequenceSize, data + offset, sizeof(sequenceSequenceSize));
        offset += sizeof(sequenceSequenceSize);

        // Deserialise sequenceSequence data
        sequenceSequence.resize(sequenceSequenceSize);
        for (auto& e1 : sequenceSequence) {
            // Deserialise size of e1
            uint64_t e1Size;
            memcpy(&e1Size, data + offset, sizeof(e1Size));
            offset += sizeof(e1Size);

            // Deserialise e1 data
            e1.resize(e1Size);
            memcpy(&e1[0], data + offset, sizeof(double) * e1Size);
            offset += sizeof(double) * e1Size;
        }

        // Deserialise mapSequence
        // Deserialise size of mapSequence
        uint64_t mapSequenceSize;
        memcpy(&mapSequenceSize, data + offset, sizeof(mapSequenceSize));
        offset += sizeof(mapSequenceSize);

        // Deserialise mapSequence data
        mapSequence.resize(mapSequenceSize);
        for (auto& e1 : mapSequence) {
            // Deserialise size of e1
            uint64_t e1Size;
            memcpy(&e1Size, data + offset, sizeof(e1Size));
            offset += sizeof(e1Size);

            // Deserialise e1 data
            for (uint64_t index = 0; index < e1Size; ++index) {
                // Deserialise e1 key data
                float first1;
                memcpy(&first1, data + offset, sizeof(first1));
                offset += sizeof(first1);

                // Deserialise e1 value data
                double second1;
                memcpy(&second1, data + offset, sizeof(second1));
                offset += sizeof(second1);

                e1.insert({first1, second1});
            }
        }

        // Deserialise mapPrimitiveToEnum
        // Deserialise size of mapPrimitiveToEnum
        uint64_t mapPrimitiveToEnumSize;
        memcpy(&mapPrimitiveToEnumSize, data + offset, sizeof(mapPrimitiveToEnumSize));
        offset += sizeof(mapPrimitiveToEnumSize);

        // Deserialise mapPrimitiveToEnum data
        for (uint64_t index = 0; index < mapPrimitiveToEnumSize; ++index) {
            // Deserialise mapPrimitiveToEnum key data
            uint16_t first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise mapPrimitiveToEnum value data
            Colour second0;
            memcpy(&second0, data + offset, sizeof(second0));
            offset += sizeof(second0);

            mapPrimitiveToEnum.insert({first0, second0});
        }

        // Deserialise mapEnumToEnum
        // Deserialise size of mapEnumToEnum
        uint64_t mapEnumToEnumSize;
        memcpy(&mapEnumToEnumSize, data + offset, sizeof(mapEnumToEnumSize));
        offset += sizeof(mapEnumToEnumSize);

        // Deserialise mapEnumToEnum data
        for (uint64_t index = 0; index < mapEnumToEnumSize; ++index) {
            // Deserialise mapEnumToEnum key data
            Colour first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise mapEnumToEnum value data
            Colour second0;
            memcpy(&second0, data + offset, sizeof(second0));
            offset += sizeof(second0);

            mapEnumToEnum.insert({first0, second0});
        }

        // Deserialise mapEnumToArray
        // Deserialise size of mapEnumToArray
        uint64_t mapEnumToArraySize;
        memcpy(&mapEnumToArraySize, data + offset, sizeof(mapEnumToArraySize));
        offset += sizeof(mapEnumToArraySize);

        // Deserialise mapEnumToArray data
        for (uint64_t index = 0; index < mapEnumToArraySize; ++index) {
            // Deserialise mapEnumToArray key data
            Colour first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise mapEnumToArray value data
            std::array<std::array<Colour, 4>, 5> second0;
            for (auto& e2 : second0) {
                memcpy(&e2[0], data + offset, sizeof(Colour) * e2.size());
                offset += sizeof(Colour) * e2.size();
            }

            mapEnumToArray.insert({first0, second0});
        }

        // Deserialise mapEnumToSequence
        // Deserialise size of mapEnumToSequence
        uint64_t mapEnumToSequenceSize;
        memcpy(&mapEnumToSequenceSize, data + offset, sizeof(mapEnumToSequenceSize));
        offset += sizeof(mapEnumToSequenceSize);

        // Deserialise mapEnumToSequence data
        for (uint64_t index = 0; index < mapEnumToSequenceSize; ++index) {
            // Deserialise mapEnumToSequence key data
            Colour first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise mapEnumToSequence value data
            std::vector<uint64_t> second0;
            // Deserialise size of second0
            uint64_t second0Size;
            memcpy(&second0Size, data + offset, sizeof(second0Size));
            offset += sizeof(second0Size);

            // Deserialise second0 data
            second0.resize(second0Size);
            memcpy(&second0[0], data + offset, sizeof(uint64_t) * second0Size);
            offset += sizeof(uint64_t) * second0Size;

            mapEnumToSequence.insert({first0, second0});
        }

        // Deserialise mapEnumToMap
        // Deserialise size of mapEnumToMap
        uint64_t mapEnumToMapSize;
        memcpy(&mapEnumToMapSize, data + offset, sizeof(mapEnumToMapSize));
        offset += sizeof(mapEnumToMapSize);

        // Deserialise mapEnumToMap data
        for (uint64_t index = 0; index < mapEnumToMapSize; ++index) {
            // Deserialise mapEnumToMap key data
            Colour first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise mapEnumToMap value data
            std::map<uint8_t, int16_t> second0;
            // Deserialise size of second0
            uint64_t second0Size;
            memcpy(&second0Size, data + offset, sizeof(second0Size));
            offset += sizeof(second0Size);

            // Deserialise second0 data
            for (uint64_t index = 0; index < second0Size; ++index) {
                // Deserialise second0 key data
                uint8_t first1;
                memcpy(&first1, data + offset, sizeof(first1));
                offset += sizeof(first1);

                // Deserialise second0 value data
                int16_t second1;
                memcpy(&second1, data + offset, sizeof(second1));
                offset += sizeof(second1);

                second0.insert({first1, second1});
            }

            mapEnumToMap.insert({first0, second0});
        }
    }

    uint64_t TestStructure::getSizeInBytes() const {
        // int8Member = 1 bytes (int8_t)
        // int16Member = 2 bytes (int16_t)
        // int32Member = 4 bytes (int32_t)
        // int64Member = 8 bytes (int64_t)
        // lastElement = 8 bytes (int64_t)
        // colour1 = 1 bytes (Colour)
        // colour2 = 1 bytes (Colour)
        // colour3 = 1 bytes (Colour)
        // Size of primitive types in this structure
        uint64_t size = 26;

        // Add on size of TSS
        size += TSS.getSizeInBytes();

        // Add on size of testArray
        for (const auto& e1 : testArray) {
            for (const auto& e2 : e1) {
                size += sizeof(uint64_t) * e2.size();
            }
        }

        // Add on size of testArrayStruct
        for (const auto& e1 : testArrayStruct) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    // Add on size of each individual TestSubStruct
                    size += e3.getSizeInBytes();
                }
            }
        }

        // Add on size of testArrayEnum
        for (const auto& e1 : testArrayEnum) {
            for (const auto& e2 : e1) {
                size += sizeof(Colour) * e2.size();
            }
        }

        // Add on size of testArraySeq
        for (const auto& e1 : testArraySeq) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    // Add on size of e3 length field
                    size += sizeof(uint64_t);

                    // Add on size of e3 data
                    size += sizeof(Colour) * e3.size();
                }
            }
        }

        // Add on size of testArrayMap
        for (const auto& e1 : testArrayMap) {
        }

        // Add on size of primitiveSequence
        // Add on size of primitiveSequence length field
        size += sizeof(uint64_t);

        // Add on size of primitiveSequence data
        size += sizeof(float) * primitiveSequence.size();

        // Add on size of enumerationSequence
        // Add on size of enumerationSequence length field
        size += sizeof(uint64_t);

        // Add on size of enumerationSequence data
        size += sizeof(Colour) * enumerationSequence.size();

        // Add on size of structureSequence
        // Add on size of structureSequence length field
        size += sizeof(uint64_t);

        // Add on size of structureSequence data
        for (const auto& e1 : structureSequence) {
            // Add on size of each individual TestSubStruct
            size += e1.getSizeInBytes();
        }

        // Add on size of arraySequence
        // Add on size of arraySequence length field
        size += sizeof(uint64_t);

        // Add on size of arraySequence data
        for (const auto& e1 : arraySequence) {
            for (const auto& e2 : e1) {
                for (const auto& e3 : e2) {
                    size += sizeof(uint16_t) * e3.size();
                }
            }
        }

        // Add on size of sequenceSequence
        // Add on size of sequenceSequence length field
        size += sizeof(uint64_t);

        // Add on size of sequenceSequence data
        for (const auto& e1 : sequenceSequence) {
            // Add on size of e1 length field
            size += sizeof(uint64_t);

            // Add on size of e1 data
            size += sizeof(double) * e1.size();
        }

        // Add on size of mapSequence
        // Add on size of mapSequence length field
        size += sizeof(uint64_t);

        // Add on size of mapSequence data
        for (const auto& e1 : mapSequence) {
            // Add on size of e1 length field
            size += sizeof(uint64_t);

            // Add on size of e1 key data
            size += sizeof(float) * e1.size();

            // Add on size of e1 value data
            size += sizeof(double) * e1.size();
        }

        // Add on size of mapPrimitiveToEnum
        // Add on size of mapPrimitiveToEnum length field
        size += sizeof(uint64_t);

        // Add on size of mapPrimitiveToEnum key data
        size += sizeof(uint16_t) * mapPrimitiveToEnum.size();

        // Add on size of mapPrimitiveToEnum value data
        size += sizeof(Colour) * mapPrimitiveToEnum.size();

        // Add on size of mapEnumToEnum
        // Add on size of mapEnumToEnum length field
        size += sizeof(uint64_t);

        // Add on size of mapEnumToEnum key data
        size += sizeof(Colour) * mapEnumToEnum.size();

        // Add on size of mapEnumToEnum value data
        size += sizeof(Colour) * mapEnumToEnum.size();

        // Add on size of mapEnumToArray
        // Add on size of mapEnumToArray length field
        size += sizeof(uint64_t);

        // Add on size of mapEnumToArray key data
        size += sizeof(Colour) * mapEnumToArray.size();

        // Add on size of mapEnumToArray value data
        for (const auto& e1 : mapEnumToArray) {
            for (const auto& e2 : e1.second) {
                size += sizeof(Colour) * e2.size();
            }
        }

        // Add on size of mapEnumToSequence
        // Add on size of mapEnumToSequence length field
        size += sizeof(uint64_t);

        // Add on size of mapEnumToSequence key data
        size += sizeof(Colour) * mapEnumToSequence.size();

        // Add on size of mapEnumToSequence value data
        for (const auto& e1 : mapEnumToSequence) {
            // Add on size of e1.second length field
            size += sizeof(uint64_t);

            // Add on size of e1.second data
            size += sizeof(uint64_t) * e1.second.size();
        }

        // Add on size of mapEnumToMap
        // Add on size of mapEnumToMap length field
        size += sizeof(uint64_t);

        // Add on size of mapEnumToMap key data
        size += sizeof(Colour) * mapEnumToMap.size();

        // Add on size of mapEnumToMap value data
        for (const auto& e1 : mapEnumToMap) {
            // Add on size of e1.second length field
            size += sizeof(uint64_t);

            // Add on size of e1.second key data
            size += sizeof(uint8_t) * e1.second.size();

            // Add on size of e1.second value data
            size += sizeof(int16_t) * e1.second.size();
        }

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

    int64_t TestStructure::getLastElement() const {
        return lastElement;
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

    std::array<std::array<std::array<uint64_t, 3>, 4>, 5>& TestStructure::getTestArray() {
        return testArray;
    }

    std::array<std::array<std::array<TestSubStruct, 3>, 4>, 5>& TestStructure::getTestArrayStruct() {
        return testArrayStruct;
    }

    std::array<std::array<std::array<Colour, 3>, 4>, 5>& TestStructure::getTestArrayEnum() {
        return testArrayEnum;
    }

    std::array<std::array<std::array<std::vector<Colour>, 3>, 4>, 5>& TestStructure::getTestArraySeq() {
        return testArraySeq;
    }

    std::array<std::array<std::map<float, double>, 4>, 5>& TestStructure::getTestArrayMap() {
        return testArrayMap;
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

    std::vector<std::array<std::array<std::array<uint16_t, 3>, 4>, 5>>& TestStructure::getArraySequence() {
        return arraySequence;
    }

    std::vector<std::vector<double>>& TestStructure::getSequenceSequence() {
        return sequenceSequence;
    }

    std::vector<std::map<float, double>>& TestStructure::getMapSequence() {
        return mapSequence;
    }

    std::map<uint16_t, Colour>& TestStructure::getMapPrimitiveToEnum() {
        return mapPrimitiveToEnum;
    }

    std::map<Colour, Colour>& TestStructure::getMapEnumToEnum() {
        return mapEnumToEnum;
    }

    std::map<Colour, std::array<std::array<Colour, 4>, 5>>& TestStructure::getMapEnumToArray() {
        return mapEnumToArray;
    }

    std::map<Colour, std::vector<uint64_t>>& TestStructure::getMapEnumToSequence() {
        return mapEnumToSequence;
    }

    std::map<Colour, std::map<uint8_t, int16_t>>& TestStructure::getMapEnumToMap() {
        return mapEnumToMap;
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

    void TestStructure::setLastElement(const int64_t value) {
        lastElement = value;
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
