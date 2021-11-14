#pragma once

namespace Messaging {
    class BaseMessage {
        public:
            /**
             * Serialise this class into a block of data
             *
             * @param data The data to serialise to (Ensure it is large enough!)
             * @param offset The offset into the data to start serialising to. Will be updated with the new offset on return
             * @return True if the operation was successful or false if not
             */
            virtual void serialise(char* data,
                                   uint64_t& offset) const = 0;

            /**
             * Deserialise a block of data into this class
             *
             * @param data The data to deserialise from
             * @param offset The offset into the data to start deserialising from. Will be updated with the new offset on return
             * @return True if the operation was successful or false if not
             */
            virtual void deserialise(const char* data,
                                     uint64_t& offset) = 0;
    };
}