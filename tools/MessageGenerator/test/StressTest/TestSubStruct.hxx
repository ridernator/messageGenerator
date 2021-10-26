#pragma once

#include <cstdint>

#include "BaseMessage.hpp"

namespace arse {
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
             * Setter for bigUnsigned
             *
             * @param value The new value to set
             */
            void setBigUnsigned(const uint64_t value);

        private :
            uint64_t bigUnsigned;
    };
}