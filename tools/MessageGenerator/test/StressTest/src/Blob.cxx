#include "Blob.hxx"

Blob::Blob() {

}

Blob::~Blob() {

}

void Blob::serialise(char* data, uint64_t& offset) const {
    // Serialise fatArray
    Serialiser::serialisePrimitiveArray(data, fatArray, offset);

    // Serialise colourArray
    Serialiser::serialisePrimitiveArray(data, colourArray, offset);

}

void Blob::deserialise(const char* data, uint64_t& offset) {
    // Deserialise fatArray
    Serialiser::deserialisePrimitiveArray(data, fatArray, offset);

    // Deserialise colourArray
    Serialiser::deserialisePrimitiveArray(data, colourArray, offset);

}

uint64_t Blob::getSizeInBytes() const {
    // Size of primitive types in this structure
    uint64_t size = 0;

    // Calculate size of fatArray
    size += SIZE_OF_ARRAY * sizeof(double);

    // Calculate size of colourArray
    size += 3 * sizeof(Colour);

    return size;
}

decltype(Blob::fatArray)& Blob::getFatArray() {
    return fatArray;
}

decltype(Blob::colourArray)& Blob::getColourArray() {
    return colourArray;
}

