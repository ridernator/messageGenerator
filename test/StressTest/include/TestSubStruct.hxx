#pragma once

#include <cstdint>
#include <string>

#include "BaseMessage.hpp"

namespace MyNamespace {
    class TestSubStruct : public Messaging::BaseMessage {
        public :
            /**
             * Constructor for this class
             */
            TestSubStruct();

            /**
             * Destructor for this class
             */
            ~TestSubStruct();

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
             * Getter for bigUnsigned
             *
             * @return bigUnsigned
             */
            uint64_t getBigUnsigned() const;

            /**
             * Getter for a8String
             *
             * @return a8String
             */
            std::string& getA8String();

            /**
             * Getter for b8String
             *
             * @return b8String
             */
            std::string& getB8String();

            /**
             * Getter for a16String
             *
             * @return a16String
             */
            std::u16string& getA16String();

            /**
             * Getter for a32String
             *
             * @return a32String
             */
            std::u32string& getA32String();

            /**
             * Setter for bigUnsigned
             *
             * @param value The new value to set
             */
            void setBigUnsigned(const uint64_t value);

            /**
             * Setter for a8String
             *
             * @param value The new value to set
             */
            void setA8String(const std::string& value);

            /**
             * Setter for b8String
             *
             * @param value The new value to set
             */
            void setB8String(const std::string& value);

            /**
             * Setter for a16String
             *
             * @param value The new value to set
             */
            void setA16String(const std::u16string& value);

            /**
             * Setter for a32String
             *
             * @param value The new value to set
             */
            void setA32String(const std::u32string& value);

        private :
            uint64_t bigUnsigned;

            std::string a8String;

            std::string b8String;

            std::u16string a16String;

            std::u32string a32String;
    };
}
