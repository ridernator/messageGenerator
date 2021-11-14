#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <vector>

#include "BaseMessage.hpp"
#include "Colour.hxx"
#include "TestSubStruct.hxx"

namespace MyNamespace {
    class OptionalTest : public Messaging::BaseMessage {
        public :
            /**
             * Constructor for this class
             */
            OptionalTest();

            /**
             * Destructor for this class
             */
            ~OptionalTest();

            void serialise(char* data,
                           uint64_t& offset) const;

            void deserialise(const char* data,
                             uint64_t& offset);

            /**
             * Get the serialised size of this class in bytes.
             * Use to create an array for serialisation.
             *
             * @return The size of the serialised version of this class in bytes
             */
            uint64_t getSizeInBytes() const;

            /**
             * Getter for primitive
             *
             * @return primitive
             */
            std::optional<uint64_t>& getPrimitive();

            /**
             * Getter for lastElement
             *
             * @return lastElement
             */
            uint64_t getLastElement() const;

            /**
             * Getter for enumeration
             *
             * @return enumeration
             */
            std::optional<Colour>& getEnumeration();

            /**
             * Getter for structure
             *
             * @return structure
             */
            std::optional<TestSubStruct>& getStructure();

            /**
             * Getter for array
             *
             * @return array
             */
            std::optional<std::array<uint16_t, 3>>& getArray();

            /**
             * Getter for sequence
             *
             * @return sequence
             */
            std::optional<std::vector<Colour>>& getSequence();

            /**
             * Getter for map
             *
             * @return map
             */
            std::optional<std::map<uint8_t, Colour>>& getMap();

            /**
             * Setter for primitive
             *
             * @param value The new value to set
             */
            void setPrimitive(const uint64_t value);

            /**
             * Setter for lastElement
             *
             * @param value The new value to set
             */
            void setLastElement(const uint64_t value);

            /**
             * Setter for enumeration
             *
             * @param value The new value to set
             */
            void setEnumeration(const Colour value);

        private :
            std::optional<uint64_t> primitive;

            uint64_t lastElement;

            std::optional<Colour> enumeration;

            std::optional<TestSubStruct> structure;

            std::optional<std::array<uint16_t, 3>> array;

            std::optional<std::vector<Colour>> sequence;

            std::optional<std::map<uint8_t, Colour>> map;
    };
}
