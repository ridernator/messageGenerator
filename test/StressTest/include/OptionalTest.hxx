#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
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
             * Getter for string1
             *
             * @return string1
             */
            std::optional<std::string>& getString1();

            /**
             * Getter for string2
             *
             * @return string2
             */
            std::optional<std::string>& getString2();

            /**
             * Getter for string3
             *
             * @return string3
             */
            std::optional<std::u16string>& getString3();

            /**
             * Getter for string4
             *
             * @return string4
             */
            std::optional<std::u32string>& getString4();

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

            /**
             * Setter for string1
             *
             * @param value The new value to set
             */
            void setString1(const std::string& value);

            /**
             * Setter for string2
             *
             * @param value The new value to set
             */
            void setString2(const std::string& value);

            /**
             * Setter for string3
             *
             * @param value The new value to set
             */
            void setString3(const std::u16string& value);

            /**
             * Setter for string4
             *
             * @param value The new value to set
             */
            void setString4(const std::u32string& value);

        private :
            std::optional<uint64_t> primitive;

            uint64_t lastElement;

            std::optional<Colour> enumeration;

            std::optional<std::string> string1;

            std::optional<std::string> string2;

            std::optional<std::u16string> string3;

            std::optional<std::u32string> string4;

            std::optional<TestSubStruct> structure;

            std::optional<std::array<uint16_t, 3>> array;

            std::optional<std::vector<Colour>> sequence;

            std::optional<std::map<uint8_t, Colour>> map;
    };
}
