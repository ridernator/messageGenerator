#include "Blob.hxx"
#include "Colour.hxx"
#include "StressTest.hxx"

#include <iostream>

int main(const int argc,
         const char** argv) {
    StressTest in;
    StressTest out;
    
    uint64_t offset;
    
    std::cout << "sizeInBytes = " << in.getSizeInBytes() << std::endl;
    in.setSimpleFloat(1.234);
    in.setSimpleInt(1234);
    in.setSimpleIntNoDoc(5678);
    
    in.getArrayOfIntSequences().at(3).push_back(3);
    in.getArrayOfIntSequences().at(3).push_back(4);
    
    in.getBlobber().getColourArray().at(2) = Colour::GREEN;
    
    char data[in.getSizeInBytes()];
    offset = 0;
    in.serialise(data, offset);
    
    offset = 0;
    out.deserialise(data, offset);
    
    std::cout << "simpleFloat before=" << in.getSimpleFloat() << ", after=" << out.getSimpleFloat() << std::endl;
    std::cout << "simpleInt before=" << in.getSimpleInt() << ", after=" << out.getSimpleInt() << std::endl;
    std::cout << "simpleIntNoDoc before=" << in.getSimpleIntNoDoc() << ", after=" << out.getSimpleIntNoDoc() << std::endl;
    std::cout << "arrayOfIntSequences before=" << in.getArrayOfIntSequences().at(3).at(0) << ", after=" << out.getArrayOfIntSequences().at(3).at(0) << std::endl;
    std::cout << "arrayOfIntSequences before=" << in.getArrayOfIntSequences().at(3).at(1) << ", after=" << out.getArrayOfIntSequences().at(3).at(1) << std::endl;
    std::cout << "blobber.colourArray before=" << (uint32_t) in.getBlobber().getColourArray().at(2) << ", after=" << (uint32_t) out.getBlobber().getColourArray().at(2) << std::endl;
}