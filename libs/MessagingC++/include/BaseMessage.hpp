#pragma once

#include <cstdint>

namespace Messaging {
    class BaseMessage {
        public :
            BaseMessage();
			
            virtual ~BaseMessage();

            virtual void serialise(char* data,
                                   uint64_t& offset) = 0;

            virtual void deserialise(const char* data,
                                     uint64_t& offset) = 0;

            virtual uint64_t getSizeInBytes() = 0;
    };
}