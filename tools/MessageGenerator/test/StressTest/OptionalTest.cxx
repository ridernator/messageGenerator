#include "OptionalTest.hxx"

#include <string.h>

namespace MyNamespace {
    OptionalTest::OptionalTest() {

    }

    OptionalTest::~OptionalTest() {

    }

    void OptionalTest::serialise(char* data, uint64_t& offset) const {
        // Serialise primitive
        memcpy(data + offset, &primitive, sizeof(uint64_t));
        offset += sizeof(primitive);

        // Serialise lastElement
        memcpy(data + offset, &lastElement, sizeof(uint64_t));
        offset += sizeof(lastElement);

        // Serialise enumeration
        memcpy(data + offset, &enumeration, sizeof(Colour));
        offset += sizeof(Colour);

        // Serialise structure
        structure.serialise(data + offset, offset);

        // Serialise array
        memcpy(data + offset, &array[0], sizeof(uint16_t) * array.size());
        offset += sizeof(uint16_t) * array.size();

        // Serialise sequence
        // Serialise size of sequence
        uint64_t sequenceSize = sequence.size();
        memcpy(data + offset, &sequenceSize, sizeof(sequenceSize));
        offset += sizeof(sequenceSize);

        // Serialise sequence data
        memcpy(data + offset, &sequence[0], sizeof(Colour) * sequenceSize);
        offset += sizeof(Colour) * sequenceSize;

        // Serialise map
        // Serialise size of map
        uint64_t mapSize = map.size();
        memcpy(data + offset, &mapSize, sizeof(mapSize));
        offset += sizeof(mapSize);

        for (const auto& e0 : map) {
            // Serialise map key data
            memcpy(data + offset, &e0.first, sizeof(uint8_t));
            offset += sizeof(uint8_t);

            // Serialise map value data
            memcpy(data + offset, &e0.second, sizeof(Colour));
            offset += sizeof(Colour);
        }
    }

    void OptionalTest::deserialise(const char* data, uint64_t& offset) {
        // Deserialise primitive
        memcpy(&primitive, data + offset, sizeof(uint64_t));
        offset += sizeof(primitive);

        // Deserialise lastElement
        memcpy(&lastElement, data + offset, sizeof(uint64_t));
        offset += sizeof(lastElement);

        // Deserialise enumeration
        memcpy(&enumeration, data + offset, sizeof(Colour));
        offset += sizeof(Colour);

        // Deserialise structure
        structure.deserialise(data + offset, offset);

        // Deserialise array
        memcpy(&array[0], data + offset, sizeof(uint16_t) * array.size());
        offset += sizeof(uint16_t) * array.size();

        // Deserialise sequence
        // Deserialise size of sequence
        uint64_t sequenceSize;
        memcpy(&sequenceSize, data + offset, sizeof(sequenceSize));
        offset += sizeof(sequenceSize);

        // Deserialise sequence data
        sequence.resize(sequenceSize);
        memcpy(&sequence[0], data + offset, sizeof(Colour) * sequenceSize);
        offset += sizeof(Colour) * sequenceSize;

        // Deserialise map
        // Deserialise size of map
        uint64_t mapSize;
        memcpy(&mapSize, data + offset, sizeof(mapSize));
        offset += sizeof(mapSize);

        // Deserialise map data
        for (uint64_t index = 0; index < mapSize; ++index) {
            // Deserialise map key data
            uint8_t first0;
            memcpy(&first0, data + offset, sizeof(first0));
            offset += sizeof(first0);

            // Deserialise map value data
            Colour second0;
            memcpy(&second0, data + offset, sizeof(second0));
            offset += sizeof(second0);

            map.insert({first0, second0});
        }
    }

    uint64_t OptionalTest::getSizeInBytes() const {
        // primitive = 8 bytes (uint64_t)
        // lastElement = 8 bytes (uint64_t)
        // enumeration = 1 bytes (Colour)
        // Size of primitive types in this structure
        uint64_t size = 17;

        // Add on size of structure
        size += structure.getSizeInBytes();

        // Add on size of array
        size += sizeof(uint16_t) * array.size();

        // Add on size of sequence
        // Add on size of sequence length field
        size += sizeof(uint64_t);

        // Add on size of sequence data
        size += sizeof(Colour) * sequence.size();

        // Add on size of map
        // Add on size of map length field
        size += sizeof(uint64_t);

        // Add on size of map key data
        size += sizeof(uint8_t) * map.size();

        // Add on size of map value data
        size += sizeof(Colour) * map.size();

        return size;
    }

    uint64_t OptionalTest::getPrimitive() const {
        return primitive;
    }

    uint64_t OptionalTest::getLastElement() const {
        return lastElement;
    }

    Colour OptionalTest::getEnumeration() const {
        return enumeration;
    }

    TestSubStruct& OptionalTest::getStructure() {
        return structure;
    }

    std::array<uint16_t, 3>& OptionalTest::getArray() {
        return array;
    }

    std::vector<Colour>& OptionalTest::getSequence() {
        return sequence;
    }

    std::map<uint8_t, Colour>& OptionalTest::getMap() {
        return map;
    }

    void OptionalTest::setPrimitive(const uint64_t value) {
        primitive = value;
    }

    void OptionalTest::setLastElement(const uint64_t value) {
        lastElement = value;
    }

    void OptionalTest::setEnumeration(const Colour value) {
        enumeration = value;
    }

}
