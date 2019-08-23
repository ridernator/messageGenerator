#include "Blob.hxx"

Blob::Blob() {

}

Blob::~Blob() {

}

bool Blob::serialise(char* data, uint32_t& offset) {
	return true;
}

bool Blob::deserialise(const char* data, uint32_t& offset) {
	return true;
}

decltype(Blob::fatArray)& Blob::getFatArray() {
	return fatArray;
}

