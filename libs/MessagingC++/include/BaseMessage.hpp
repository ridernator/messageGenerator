#pragma once

#include <cstdint>

namespace Messaging {
    class BaseMessage {
        public :
            BaseMessage();
			
            virtual ~BaseMessage();

            virtual bool serialise(char* data,
                                   uint32_t& offset) = 0;

            virtual bool deserialise(const char* data,
                                     uint32_t& offset) = 0;
    };
}