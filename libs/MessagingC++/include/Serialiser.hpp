#pragma once

#include <cstdint>
#include <array>
#include <cstring>

class Serialiser {
    public:
        template <class PrimitiveType>
        static void serialisePrimitive(char* data,
                                       const PrimitiveType& value,
                                       uint64_t& offset) {
            memcpy(data + offset, &value, sizeof (PrimitiveType));

            offset += sizeof (PrimitiveType);
        }

        template <class PrimitiveType, std::size_t Size>
        static void serialisePrimitiveArray(char* data,
                                            const std::array<PrimitiveType, Size>& value,
                                            uint64_t& offset) {
            memcpy(data + offset, value.data(), sizeof (PrimitiveType) * Size);

            offset += sizeof (PrimitiveType) * Size;
        }
        
        static void serialiseString(char* data,
                                    const std::string& value,
                                    uint64_t& offset);
};