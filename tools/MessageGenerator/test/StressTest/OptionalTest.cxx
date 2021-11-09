#include "OptionalTest.hxx"

#include <string.h>

namespace MyNamespace {
    OptionalTest::OptionalTest() {

    }

    OptionalTest::~OptionalTest() {

    }

    void OptionalTest::serialise(char* data, uint64_t& offset) const {
        // Serialise primitive
        // Serialise primitive presence flag
        *(data + offset++) = primitive.has_value() ? 1 : 0;

        // Serialise primitive data
        if (primitive.has_value()) {
            memcpy(data + offset, &primitive.value(), sizeof(primitive.value()));
            offset += sizeof(primitive.value());
        }

        // Serialise lastElement
        memcpy(data + offset, &lastElement, sizeof(lastElement));
        offset += sizeof(lastElement);

        // Serialise enumeration
        // Serialise enumeration presence flag
        *(data + offset++) = enumeration.has_value() ? 1 : 0;

        // Serialise enumeration data
        if (enumeration.has_value()) {
            memcpy(data + offset, &enumeration.value(), sizeof(enumeration.value()));
            offset += sizeof(enumeration.value());
        }

        // Serialise structure
        // Serialise structure presence flag
        *(data + offset++) = structure.has_value() ? 1 : 0;

        // Serialise structure data
        if (structure.has_value()) {
            structure.value().serialise(data + offset, offset);
        }

        // Serialise array
        // Serialise array presence flag
        *(data + offset++) = array.has_value() ? 1 : 0;

        // Serialise array data
        if (array.has_value()) {
            memcpy(data + offset, &array.value()[0], sizeof(uint16_t) * array.value().size());
            offset += sizeof(uint16_t) * array.value().size();
        }

        // Serialise sequence
        // Serialise sequence presence flag
        *(data + offset++) = sequence.has_value() ? 1 : 0;

        // Serialise sequence data
        if (sequence.has_value()) {
            // Serialise size of sequence.value()
            uint64_t sequencevalueSize = sequence.value().size();
            memcpy(data + offset, &sequencevalueSize, sizeof(sequencevalueSize));
            offset += sizeof(sequencevalueSize);

            // Serialise sequence.value() data
            memcpy(data + offset, &sequence.value()[0], sizeof(Colour) * sequencevalueSize);
            offset += sizeof(Colour) * sequencevalueSize;
        }

        // Serialise map
        // Serialise map presence flag
        *(data + offset++) = map.has_value() ? 1 : 0;

        // Serialise map data
        if (map.has_value()) {
            // Serialise size of map.value()
            uint64_t mapvalueSize = map.value().size();
            memcpy(data + offset, &mapvalueSize, sizeof(mapvalueSize));
            offset += sizeof(mapvalueSize);

            for (const auto& e1 : map.value()) {
                // Serialise mapvalue key data
                memcpy(data + offset, &e1.first, sizeof(uint8_t));
                offset += sizeof(uint8_t);

                // Serialise mapvalue value data
                memcpy(data + offset, &e1.second, sizeof(Colour));
                offset += sizeof(Colour);
            }
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

    std::optional<uint64_t>& OptionalTest::getPrimitive() {
        return primitive;
    }

    uint64_t OptionalTest::getLastElement() const {
        return lastElement;
    }

    std::optional<Colour>& OptionalTest::getEnumeration() {
        return enumeration;
    }

    std::optional<TestSubStruct>& OptionalTest::getStructure() {
        return structure;
    }

    std::optional<std::array<uint16_t, 3>>& OptionalTest::getArray() {
        return array;
    }

    std::optional<std::vector<Colour>>& OptionalTest::getSequence() {
        return sequence;
    }

    std::optional<std::map<uint8_t, Colour>>& OptionalTest::getMap() {
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
