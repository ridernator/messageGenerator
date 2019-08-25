#include "Blob.hxx"

Blob::Blob() {

}

Blob::~Blob() {

}

void Blob::serialise(char* data, uint64_t& offset) {
	Serialiser::serialisePrimitiveArray(data, fatArray, offset);
}

void Blob::deserialise(const char* data, uint64_t& offset) {
	Serialiser::deserialisePrimitiveArray(data, fatArray, offset);
}

decltype(Blob::fatArray)& Blob::getFatArray() {
	return fatArray;
}

