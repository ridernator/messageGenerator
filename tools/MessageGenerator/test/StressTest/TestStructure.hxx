#pragma once

#include <cstdint>

#include "BaseMessage.hpp"
#include "Colour.hxx"
#include "Colour.hxx"
#include "Colour.hxx"

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
             * Getter for colour1
             *
             * @return colour1
             */
            Colour getColour1() const;

            /**
             * Getter for colour2
             *
             * colour2 defined as : Colour 2 is wicked!
             *
             * @return colour2
             */
            Colour getColour2() const;

            /**
             * Getter for colour3
             *
             * @return colour3
             */
            Colour getColour3() const;

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

            /**
             * Setter for colour1
             *
             * @param value The new value to set
             */
            void setColour1(const Colour value);

            /**
             * Setter for colour2
             *
             * colour2 defined as : Colour 2 is wicked!
             *
             * @param value The new value to set
             */
            void setColour2(const Colour value);

            /**
             * Setter for colour3
             *
             * @param value The new value to set
             */
            void setColour3(const Colour value);

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

            Colour colour1;

            /**
             * Colour 2 is wicked!
             */
            Colour colour2;

            Colour colour3;
    };
}
