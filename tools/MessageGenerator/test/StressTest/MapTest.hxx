#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <map>

#include "BaseMessage.hpp"

#include "Colour.hxx"
#include "TestSubStruct.hxx"

namespace MyNamespace {
    class MapTest : public Messaging::BaseMessage {
        public :
            /**
             * Constructor for this class
             */
            MapTest();

            /**
             * Destructor for this class
             */
            ~MapTest();

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
             * Getter for lastElement
             *
             * @return lastElement
             */
            uint64_t getLastElement() const;

            /**
             * Getter for colour
             *
             * @return colour
             */
            Colour getColour() const;

            /**
             * Getter for tss
             *
             * @return tss
             */
            TestSubStruct& getTss();

            /**
             * Getter for primitiveToPrimitive
             *
             * @return primitiveToPrimitive
             */
            std::map<uint8_t, float>& getPrimitiveToPrimitive();

            /**
             * Getter for primitiveToEnum
             *
             * @return primitiveToEnum
             */
            std::map<uint8_t, Colour>& getPrimitiveToEnum();

            /**
             * Getter for primitiveToStruct
             *
             * @return primitiveToStruct
             */
            std::map<uint8_t, TestSubStruct>& getPrimitiveToStruct();

            /**
             * Getter for primitiveToArray
             *
             * @return primitiveToArray
             */
            std::map<uint8_t, std::array<std::array<uint8_t, 2>, 2>>& getPrimitiveToArray();

            /**
             * Getter for primitiveToSequence
             *
             * @return primitiveToSequence
             */
            std::map<uint8_t, std::vector<std::vector<uint8_t>>>& getPrimitiveToSequence();

            /**
             * Getter for primitiveToMap
             *
             * @return primitiveToMap
             */
            std::map<uint8_t, std::map<uint16_t, std::map<uint16_t, Colour>>>& getPrimitiveToMap();

            /**
             * Setter for lastElement
             *
             * @param value The new value to set
             */
            void setLastElement(const uint64_t value);

            /**
             * Setter for colour
             *
             * @param value The new value to set
             */
            void setColour(const Colour value);

        private :
            uint64_t lastElement;

            Colour colour;

            TestSubStruct tss;

            std::map<uint8_t, float> primitiveToPrimitive;

            std::map<uint8_t, Colour> primitiveToEnum;

            std::map<uint8_t, TestSubStruct> primitiveToStruct;

            std::map<uint8_t, std::array<std::array<uint8_t, 2>, 2>> primitiveToArray;

            std::map<uint8_t, std::vector<std::vector<uint8_t>>> primitiveToSequence;

            std::map<uint8_t, std::map<uint16_t, std::map<uint16_t, Colour>>> primitiveToMap;
    };
}
