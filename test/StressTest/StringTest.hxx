#pragma once

#include <array>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "BaseMessage.hpp"

namespace MyNamespace {
    class StringTest : public Messaging::BaseMessage {
        public :
            /**
             * Constructor for this class
             */
            StringTest();

            /**
             * Destructor for this class
             */
            ~StringTest();

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
             * Getter for a8StringOpt
             *
             * @return a8StringOpt
             */
            std::optional<std::string>& getA8StringOpt();

            /**
             * Getter for b8StringOpt
             *
             * @return b8StringOpt
             */
            std::optional<std::string>& getB8StringOpt();

            /**
             * Getter for a16StringOpt
             *
             * @return a16StringOpt
             */
            std::optional<std::u16string>& getA16StringOpt();

            /**
             * Getter for a32StringOpt
             *
             * @return a32StringOpt
             */
            std::optional<std::u32string>& getA32StringOpt();

            /**
             * Getter for array
             *
             * @return array
             */
            std::array<std::u16string, 7>& getArray();

            /**
             * Getter for arrayOpt
             *
             * @return arrayOpt
             */
            std::optional<std::array<std::u16string, 7>>& getArrayOpt();

            /**
             * Getter for sequence
             *
             * @return sequence
             */
            std::vector<std::u32string>& getSequence();

            /**
             * Getter for sequenceOpt
             *
             * @return sequenceOpt
             */
            std::optional<std::vector<std::u32string>>& getSequenceOpt();

            /**
             * Getter for map
             *
             * @return map
             */
            std::map<std::string, std::u32string>& getMap();

            /**
             * Getter for mapOpt
             *
             * @return mapOpt
             */
            std::optional<std::map<std::string, std::u32string>>& getMapOpt();

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

            /**
             * Setter for a8StringOpt
             *
             * @param value The new value to set
             */
            void setA8StringOpt(const std::string& value);

            /**
             * Setter for b8StringOpt
             *
             * @param value The new value to set
             */
            void setB8StringOpt(const std::string& value);

            /**
             * Setter for a16StringOpt
             *
             * @param value The new value to set
             */
            void setA16StringOpt(const std::u16string& value);

            /**
             * Setter for a32StringOpt
             *
             * @param value The new value to set
             */
            void setA32StringOpt(const std::u32string& value);

        private :
            std::string a8String;

            std::string b8String;

            std::u16string a16String;

            std::u32string a32String;

            std::optional<std::string> a8StringOpt;

            std::optional<std::string> b8StringOpt;

            std::optional<std::u16string> a16StringOpt;

            std::optional<std::u32string> a32StringOpt;

            std::array<std::u16string, 7> array;

            std::optional<std::array<std::u16string, 7>> arrayOpt;

            std::vector<std::u32string> sequence;

            std::optional<std::vector<std::u32string>> sequenceOpt;

            std::map<std::string, std::u32string> map;

            std::optional<std::map<std::string, std::u32string>> mapOpt;
    };
}
