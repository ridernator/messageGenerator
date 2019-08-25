#pragma once

#include "BaseMessage.hpp"

#include "Serialiser.hpp"

/**
 * This class is the blob class
 */
class Blob : public Messaging::BaseMessage {
	public :
		/**
		 * Size of the "intArray" array
		 */
		static const uint32_t SIZE_OF_ARRAY = 10000;

		/**
		 * Constructor for this class
		 */
		Blob();

		/**
		 * Destructor for this class
		 */
		~Blob();

		/**
		 * Serialise this class into a block of data
		 *
		 * @param data The data to serialise to (Ensure it is large enough!)
		 * @param offset The offset into the data to start serialising to. Will be updated with the new offset on return
		 * @return True if the operation was successful or false if not
		 */
		void serialise(char* data,
		               uint64_t& offset);

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
		 * Getter for fatArray
		 *
		 * fatArray defined as : This type is called fatArray
		 *
		 * @return fatArray
		 */
		std::array<double, SIZE_OF_ARRAY>& getFatArray();

	private :
		/**
		 * This type is called fatArray
		 */
		std::array<double, SIZE_OF_ARRAY> fatArray;

};
