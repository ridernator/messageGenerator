#pragma once

#include <cstdint>

#include "BaseMessage.hpp"
#include "Serialiser.hpp"

namespace arse {
    /**
     * Documentation for test structure
     */
    class TestStructure : public Messaging::BaseMessage {
        public :
            /**
             * Constructor for this class
             */
            TestStructure();

            /**
             * Destructor for this class
             */
            ~TestStructure();

            /**
             * Serialise this class into a block of data
             *
             * @param data The data to serialise to (Ensure it is large enough!)
             * @param offset The offset into the data to start serialising to. Will be updated with the new offset on return
             * @return True if the operation was successful or false if not
             */
            void serialise(char* data,
                           uint64_t& offset) const;

            /**
             * Deserialise a block of data into this class
             *
             * @param data The data to deserialise from
             * @param offset The offset into the data to start deserialising from. Will be updated with the new offset on return
             * @return True if the operation was successful or false if not
             */
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
             * Getter for int8Member
             *
             * @return int8Member
             */
            int8_t getInt8Member() const;

            /**
             * Getter for int16Member
             *
             * int16Member defined as : Doc for this 16
             *
             * @return int16Member
             */
            int16_t getInt16Member() const;

            /**
             * Getter for int32Member
             *
             * int32Member defined as : Doc for this 32
             *
             * @return int32Member
             */
            int32_t getInt32Member() const;

            /**
             * Getter for int64Member
             *
             * @return int64Member
             */
            int64_t getInt64Member() const;

            /**
             * Setter for int8Member
             *
             * @param value The new value to set
             */
            void setInt8Member(const int8_t value);

            /**
             * Setter for int16Member
             *
             * int16Member defined as : Doc for this 16
             *
             * @param value The new value to set
             */
            void setInt16Member(const int16_t value);

            /**
             * Setter for int32Member
             *
             * int32Member defined as : Doc for this 32
             *
             * @param value The new value to set
             */
            void setInt32Member(const int32_t value);

            /**
             * Setter for int64Member
             *
             * @param value The new value to set
             */
            void setInt64Member(const int64_t value);

        private :
            int8_t int8Member;

            /**
             * Doc for this 16
             */
            int16_t int16Member;

            /**
             * Doc for this 32
             */
            int32_t int32Member;

            int64_t int64Member;
    };
}
