#include "StressTest.hxx"

StressTest::StressTest() {

}

StressTest::~StressTest() {

}

void StressTest::serialise(char* data, uint64_t& offset) {
	Serialiser::serialisePrimitive(data, simpleInt, offset);
	blobber.serialise(data, offset);
	Serialiser::serialiseString(data, stringer, offset);
	Serialiser::serialisePrimitive(data, simpleIntNoDoc, offset);
	Serialiser::serialisePrimitive(data, simpleFloat, offset);
	Serialiser::serialisePrimitiveArray(data, intArray, offset);
}

void StressTest::deserialise(const char* data, uint64_t& offset) {
}

decltype(StressTest::simpleInt) StressTest::getSimpleInt() {
	return simpleInt;
}

decltype(StressTest::blobber)& StressTest::getBlobber() {
	return blobber;
}

decltype(StressTest::stringer)& StressTest::getStringer() {
	return stringer;
}

decltype(StressTest::simpleIntNoDoc) StressTest::getSimpleIntNoDoc() {
	return simpleIntNoDoc;
}

decltype(StressTest::simpleFloat) StressTest::getSimpleFloat() {
	return simpleFloat;
}

decltype(StressTest::intArray)& StressTest::getIntArray() {
	return intArray;
}

decltype(StressTest::floatSequence)& StressTest::getFloatSequence() {
	return floatSequence;
}

decltype(StressTest::arrayOfIntSequences)& StressTest::getArrayOfIntSequences() {
	return arrayOfIntSequences;
}

decltype(StressTest::sequenceOfIntArrays)& StressTest::getSequenceOfIntArrays() {
	return sequenceOfIntArrays;
}

decltype(StressTest::intToDoubleMap)& StressTest::getIntToDoubleMap() {
	return intToDoubleMap;
}

decltype(StressTest::intArray5d)& StressTest::getIntArray5d() {
	return intArray5d;
}

decltype(StressTest::floatSequence5d)& StressTest::getFloatSequence5d() {
	return floatSequence5d;
}

decltype(StressTest::blobSequence5d)& StressTest::getBlobSequence5d() {
	return blobSequence5d;
}

void StressTest::setSimpleInt(const decltype(StressTest::simpleInt) value) {
	simpleInt = value;
}

void StressTest::setSimpleIntNoDoc(const decltype(StressTest::simpleIntNoDoc) value) {
	simpleIntNoDoc = value;
}

void StressTest::setSimpleFloat(const decltype(StressTest::simpleFloat) value) {
	simpleFloat = value;
}

