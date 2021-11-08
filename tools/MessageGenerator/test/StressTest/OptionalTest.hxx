#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <map>

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
            uint64_t getPrimitive() const;

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
            Colour getEnumeration() const;

            /**
             * Getter for structure
             *
             * @return structure
             */
            TestSubStruct& getStructure();

            /**
             * Getter for array
             *
             * @return array
             */
            std::array<uint16_t, 3>& getArray();

            /**
             * Getter for sequence
             *
             * @return sequence
             */
            std::vector<Colour>& getSequence();

            /**
             * Getter for map
             *
             * @return map
             */
            std::map<uint8_t, Colour>& getMap();

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
            uint64_t primitive;

            uint64_t lastElement;

            Colour enumeration;

            TestSubStruct structure;

            std::array<uint16_t, 3> array;

            std::vector<Colour> sequence;

            std::map<uint8_t, Colour> map;
    };
}
