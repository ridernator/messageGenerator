#include "Blob.hxx"
#include "Colour.hxx"

#include <iostream>

int main(const int argc,
         const char** argv) {
    Blob blobIn;
    Blob blobOut;
    blobIn.getFatArray().at(0) = 0;
    blobIn.getFatArray().at(1) = 1;
    blobIn.getFatArray().at(2) = 2;
    blobIn.getFatArray().at(3) = 3;
    blobIn.getFatArray().at(4) = 4;

    char data[blobIn.getSizeInBytes()];
    uint64_t offset = 0;


    blobIn.serialise(data, offset);

    offset = 0;
    blobOut.deserialise(data, offset);

    std::cout << "blobOut.getFatArray().at(0) = " << blobOut.getFatArray().at(0) << std::endl;
    std::cout << "blobOut.getFatArray().at(1) = " << blobOut.getFatArray().at(1) << std::endl;
    std::cout << "blobOut.getFatArray().at(2) = " << blobOut.getFatArray().at(2) << std::endl;
    std::cout << "blobOut.getFatArray().at(3) = " << blobOut.getFatArray().at(3) << std::endl;
    std::cout << "blobOut.getFatArray().at(4) = " << blobOut.getFatArray().at(4) << std::endl;
    std::cout << "offset = " << offset << std::endl;
    std::cout << "blobOut.getSizeInBytes() = " << blobOut.getSizeInBytes();
    
    Colour colour = Colour::AMBER;
}