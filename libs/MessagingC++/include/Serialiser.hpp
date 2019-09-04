#pragma once

#include <cstdint>
#include <array>
#include <map>
#include <cstring>
#include <vector>

class Serialiser {
    public:
        template <class PrimitiveType>
        static void serialisePrimitive(char* data,
                                       const PrimitiveType& value,
                                       uint64_t& offset) {
            memcpy(data + offset, &value, sizeof (PrimitiveType));

            offset += sizeof (PrimitiveType);
        }

        template <class PrimitiveType, std::size_t size>
        static void serialisePrimitiveArray(char* data,
                                            const std::array<PrimitiveType, size>& value,
                                            uint64_t& offset) {
            memcpy(data + offset, value.data(), sizeof (PrimitiveType) * size);

            offset += sizeof (PrimitiveType) * size;
        }

        template <class PrimitiveType>
        static void serialisePrimitiveSequence(char* data,
                                               const std::vector<PrimitiveType>& value,
                                               uint64_t& offset) {
            serialisePrimitive(data, value.size(), offset);
            
            memcpy(data + offset, value.data(), sizeof (PrimitiveType) * value.size());

            offset += sizeof (PrimitiveType) * value.size();
        }
        
        static void serialiseString(char* data,
                                    const std::string& value,
                                    uint64_t& offset);
        
        template <class PrimitiveType>
        static void deserialisePrimitive(const char* data,
                                         PrimitiveType& value,
                                         uint64_t& offset) {
            memcpy(&value, data + offset, sizeof (PrimitiveType));

            offset += sizeof (PrimitiveType);
        }

        template <class PrimitiveType, std::size_t size>
        static void deserialisePrimitiveArray(const char* data,
                                              std::array<PrimitiveType, size>& value,
                                              uint64_t& offset) {
            memcpy(value.data(), data + offset, sizeof (PrimitiveType) * size);

            offset += sizeof (PrimitiveType) * size;
        }

        template <class PrimitiveType>
        static void deserialisePrimitiveSequence(const char* data,
                                                 std::vector<PrimitiveType>& value,
                                                 uint64_t& offset) {
            decltype(value.size()) size;
            deserialisePrimitive(data, size, offset);
            
            value.reserve(size);
            
            for (decltype(size) index = 0; index < size; ++index) {
                value.push_back((PrimitiveType) *(data + offset));
                offset += sizeof (PrimitiveType);
            }
        }
        
        static void deserialiseString(const char* data,
                                      std::string& value,
                                      uint64_t& offset);
};