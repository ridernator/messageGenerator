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
            structure.value().serialise(data, offset);
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
        if (*(data + offset++) == 1) {
            primitive = typename std::decay_t<decltype(primitive)>::value_type{};
            memcpy(&primitive.value(), data + offset, sizeof(uint64_t));
            offset += sizeof(primitive.value());
        } else {
            primitive.reset();
        }

        // Deserialise lastElement
        memcpy(&lastElement, data + offset, sizeof(uint64_t));
        offset += sizeof(lastElement);

        // Deserialise enumeration
        if (*(data + offset++) == 1) {
            enumeration = typename std::decay_t<decltype(enumeration)>::value_type{};
            memcpy(&enumeration.value(), data + offset, sizeof(enumeration.value()));
            offset += sizeof(enumeration.value());
        } else {
            enumeration.reset();
        }

        // Deserialise structure
        if (*(data + offset++) == 1) {
            structure = typename std::decay_t<decltype(structure)>::value_type{};
            structure.value().deserialise(data, offset);
        } else {
            structure.reset();
        }

        // Deserialise array
        if (*(data + offset++) == 1) {
            array = typename std::decay_t<decltype(array)>::value_type{};
            memcpy(&array.value()[0], data + offset, sizeof(uint16_t) * array.value().size());
            offset += sizeof(uint16_t) * array.value().size();
        } else {
            array.reset();
        }

        // Deserialise sequence
        if (*(data + offset++) == 1) {
            sequence = typename std::decay_t<decltype(sequence)>::value_type{};
            // Deserialise size of sequence.value()
            uint64_t sequencevalueSize;
            memcpy(&sequencevalueSize, data + offset, sizeof(sequencevalueSize));
            offset += sizeof(sequencevalueSize);

            // Deserialise sequence.value() data
            sequence.value().resize(sequencevalueSize);
            memcpy(&sequence.value()[0], data + offset, sizeof(Colour) * sequencevalueSize);
            offset += sizeof(Colour) * sequencevalueSize;
        } else {
            sequence.reset();
        }

        // Deserialise map
        if (*(data + offset++) == 1) {
            map = typename std::decay_t<decltype(map)>::value_type{};
            // Deserialise size of map.value()
            uint64_t mapvalueSize;
            memcpy(&mapvalueSize, data + offset, sizeof(mapvalueSize));
            offset += sizeof(mapvalueSize);

            // Deserialise map.value() data
            for (uint64_t index = 0; index < mapvalueSize; ++index) {
                // Deserialise mapvalue key data
                uint8_t first1;
                memcpy(&first1, data + offset, sizeof(first1));
                offset += sizeof(first1);

                // Deserialise mapvalue value data
                Colour second1;
                memcpy(&second1, data + offset, sizeof(second1));
                offset += sizeof(second1);

                map.value().insert({first1, second1});
            }
        } else {
            map.reset();
        }
    }

    uint64_t OptionalTest::getSizeInBytes() const {
        // Optionality flag for primitive = 1 byte
        // lastElement = 8 bytes (uint64_t)
        // Optionality flag for enumeration = 1 byte
        // Size of primitive types in this structure
        uint64_t size = 10;

        // If primitive is present then add on size of it
        if (primitive.has_value()) {
            size += sizeof(uint64_t);
        }
        // If enumeration is present then add on size of it
        if (enumeration.has_value()) {
            size += sizeof(Colour);
        }
        // Add on size of structure
        ++size;
        if (structure.has_value()) {
            size += structure.value().getSizeInBytes();
        }

        // Add on size of array
        ++size;
        if (array.has_value()) {
            size += sizeof(uint16_t) * array.value().size();

        }
        // Add on size of sequence
        ++size;
        if (sequence.has_value()) {
            // Add on size of sequence.value() length field
            size += sizeof(uint64_t);

            // Add on size of sequence.value() data
            size += sizeof(Colour) * sequence.value().size();

        }
        // Add on size of map
        ++size;
        if (map.has_value()) {
            // Add on size of map.value() length field
            size += sizeof(uint64_t);

            // Add on size of map.value() key data
            size += sizeof(uint8_t) * map.value().size();

            // Add on size of map.value() value data
            size += sizeof(Colour) * map.value().size();

        }
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
