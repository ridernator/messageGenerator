#pragma once

#include "BaseMessage.hpp"

#include "Serialiser.hpp"

#include "Blob.hxx"
/**
 * This class is the stress test class
 */
class StressTest : public Messaging::BaseMessage {
	public :
		/**
		 * Size of the "intArray" array
		 */
		static const int32_t SIZE_OF_INT_ARRAY = 10;

		/**
		 * Constructor for this class
		 */
		StressTest();

		/**
		 * Destructor for this class
		 */
		~StressTest();

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
		 * Get the serialised size of this class in bytes.
		 * Use to create an array for serialisation.
		 *
		 * @return The size of the serialised version of this class in bytes
		 */
		uint64_t getSizeInBytes();

		/**
		 * Getter for simpleInt
		 *
		 * simpleInt defined as : This type is called simpleInt
		 *
		 * @return simpleInt
		 */
		int32_t getSimpleInt();

		/**
		 * Getter for blobber
		 *
		 * blobber defined as : This type is called blobber
		 *
		 * @return blobber
		 */
		Blob& getBlobber();

		/**
		 * Getter for stringer
		 *
		 * stringer defined as : This type is called stringer
		 *
		 * @return stringer
		 */
		std::string& getStringer();

		/**
		 * Getter for simpleIntNoDoc
		 *
		 * @return simpleIntNoDoc
		 */
		int32_t getSimpleIntNoDoc();

		/**
		 * Getter for simpleFloat
		 *
		 * simpleFloat defined as : This type is called simpleFloat
		 *
		 * @return simpleFloat
		 */
		float getSimpleFloat();

		/**
		 * Getter for intArray
		 *
		 * intArray defined as : This type is called intArray
		 *
		 * @return intArray
		 */
		std::array<int32_t, SIZE_OF_INT_ARRAY>& getIntArray();

		/**
		 * Getter for floatSequence
		 *
		 * floatSequence defined as : This type is called floatSequence
		 *
		 * @return floatSequence
		 */
		std::vector<float>& getFloatSequence();

		/**
		 * Getter for arrayOfIntSequences
		 *
		 * arrayOfIntSequences defined as : This type is called arrayOfIntSequences
		 *
		 * @return arrayOfIntSequences
		 */
		std::array<std::vector<int32_t>, 10>& getArrayOfIntSequences();

		/**
		 * Getter for sequenceOfIntArrays
		 *
		 * sequenceOfIntArrays defined as : This type is called arrayOfIntSequences
		 *
		 * @return sequenceOfIntArrays
		 */
		std::vector<std::array<int32_t, 10>>& getSequenceOfIntArrays();

		/**
		 * Getter for intToDoubleMap
		 *
		 * @return intToDoubleMap
		 */
		std::map<int32_t, double>& getIntToDoubleMap();

		/**
		 * Getter for intArray5d
		 *
		 * intArray5d defined as : This type is called intArray5d
		 *
		 * @return intArray5d
		 */
		std::array<std::array<std::array<std::array<std::array<int32_t, 6>, 5>, 4>, 3>, 2>& getIntArray5d();

		/**
		 * Getter for floatSequence5d
		 *
		 * floatSequence5d defined as : This type is called floatSequence5d
		 *
		 * @return floatSequence5d
		 */
		std::vector<std::vector<std::vector<std::vector<std::vector<float>>>>>& getFloatSequence5d();

		/**
		 * Getter for blobSequence5d
		 *
		 * blobSequence5d defined as : This type is called floatSequence5d
		 *
		 * @return blobSequence5d
		 */
		std::vector<std::vector<std::vector<std::vector<std::vector<Blob>>>>>& getBlobSequence5d();

		/**
		 * Setter for simpleInt
		 *
		 * simpleInt defined as : This type is called simpleInt
		 *
		 * @param value The new value to set
		 */
		void setSimpleInt(const int32_t value);

		/**
		 * Setter for simpleIntNoDoc
		 *
		 * @param value The new value to set
		 */
		void setSimpleIntNoDoc(const int32_t value);

		/**
		 * Setter for simpleFloat
		 *
		 * simpleFloat defined as : This type is called simpleFloat
		 *
		 * @param value The new value to set
		 */
		void setSimpleFloat(const float value);

	private :
		/**
		 * This type is called simpleInt
		 */
		int32_t simpleInt;

		/**
		 * This type is called blobber
		 */
		Blob blobber;

		/**
		 * This type is called stringer
		 */
		std::string stringer;

		int32_t simpleIntNoDoc;

		/**
		 * This type is called simpleFloat
		 */
		float simpleFloat;

		/**
		 * This type is called intArray
		 */
		std::array<int32_t, SIZE_OF_INT_ARRAY> intArray;

		/**
		 * This type is called floatSequence
		 */
		std::vector<float> floatSequence;

		/**
		 * This type is called arrayOfIntSequences
		 */
		std::array<std::vector<int32_t>, 10> arrayOfIntSequences;

		/**
		 * This type is called arrayOfIntSequences
		 */
		std::vector<std::array<int32_t, 10>> sequenceOfIntArrays;

		std::map<int32_t, double> intToDoubleMap;

		/**
		 * This type is called intArray5d
		 */
		std::array<std::array<std::array<std::array<std::array<int32_t, 6>, 5>, 4>, 3>, 2> intArray5d;

		/**
		 * This type is called floatSequence5d
		 */
		std::vector<std::vector<std::vector<std::vector<std::vector<float>>>>> floatSequence5d;

		/**
		 * This type is called floatSequence5d
		 */
		std::vector<std::vector<std::vector<std::vector<std::vector<Blob>>>>> blobSequence5d;

};
