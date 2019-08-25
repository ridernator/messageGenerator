#include <cstring>

#include "Serialiser.hpp"

void Serialiser::serialiseString(char* data,
                                 const std::string& value,
                                 uint64_t& offset) {
    serialisePrimitive(data, value.size(), offset);

    memcpy(data + offset, value.c_str(), sizeof (char) * value.size());

    offset += sizeof (char) * value.size();
}

void Serialiser::deserialiseString(const char* data,
                                   std::string& value,
                                   uint64_t& offset) {
    decltype(value.size()) size;
    
    deserialisePrimitive(data, size, offset);
    
    value.assign(data + offset, size);

    offset += sizeof (char) * size;
}

