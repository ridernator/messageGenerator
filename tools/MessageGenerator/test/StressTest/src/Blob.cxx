#include "Blob.hxx"

Blob::Blob() {

}

Blob::~Blob() {

}

void Blob::serialise(char* data, uint64_t& offset) {
	Serialiser::serialisePrimitiveArray(data, fatArray, offset);
	for (auto& colourArraya : colourArray) {
		}
}

void Blob::deserialise(const char* data, uint64_t& offset) {
	Serialiser::deserialisePrimitiveArray(data, fatArray, offset);
	for (auto& colourArraya : colourArray) {
		}
}

uint64_t Blob::getSizeInBytes() {
	return 1000;
}

decltype(Blob::fatArray)& Blob::getFatArray() {
	return fatArray;
}

decltype(Blob::colourArray)& Blob::getColourArray() {
	return colourArray;
}

