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
	Serialiser::serialisePrimitiveSequence(data, floatSequence, offset);
	for (auto& arrayOfIntSequencesa : arrayOfIntSequences) {
		Serialiser::serialisePrimitiveSequence(data, arrayOfIntSequencesa, offset);
	}
	for (auto& sequenceOfIntArraysa : sequenceOfIntArrays) {
		Serialiser::serialisePrimitiveArray(data, sequenceOfIntArraysa, offset);
	}
	for (auto& intArray5da : intArray5d) {
		for (auto& intArray5daa : intArray5da) {
		for (auto& intArray5daaa : intArray5daa) {
		for (auto& intArray5daaaa : intArray5daaa) {
		Serialiser::serialisePrimitiveArray(data, intArray5daaaa, offset);
	}
	}
	}
	}
	for (auto& floatSequence5da : floatSequence5d) {
		for (auto& floatSequence5daa : floatSequence5da) {
		for (auto& floatSequence5daaa : floatSequence5daa) {
		for (auto& floatSequence5daaaa : floatSequence5daaa) {
		Serialiser::serialisePrimitiveSequence(data, floatSequence5daaaa, offset);
	}
	}
	}
	}
	for (auto& blobSequence5da : blobSequence5d) {
		for (auto& blobSequence5daa : blobSequence5da) {
		for (auto& blobSequence5daaa : blobSequence5daa) {
		for (auto& blobSequence5daaaa : blobSequence5daaa) {
		for (auto& blobSequence5daaaaa : blobSequence5daaaa) {
		blobSequence5daaaaa.serialise(data, offset);
	}
	}
	}
	}
	}
}

void StressTest::deserialise(const char* data, uint64_t& offset) {
	Serialiser::deserialisePrimitive(data, simpleInt, offset);
	blobber.deserialise(data, offset);
	Serialiser::deserialiseString(data, stringer, offset);
	Serialiser::deserialisePrimitive(data, simpleIntNoDoc, offset);
	Serialiser::deserialisePrimitive(data, simpleFloat, offset);
	Serialiser::deserialisePrimitiveArray(data, intArray, offset);
	for (auto& arrayOfIntSequencesa : arrayOfIntSequences) {
		}
	for (auto& intArray5da : intArray5d) {
		for (auto& intArray5daa : intArray5da) {
		for (auto& intArray5daaa : intArray5daa) {
		for (auto& intArray5daaaa : intArray5daaa) {
		Serialiser::deserialisePrimitiveArray(data, intArray5daaaa, offset);
	}
	}
	}
	}
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

