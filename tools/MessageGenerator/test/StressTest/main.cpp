#include <iostream>

#include "TestStructure.hxx"

int main(int argc, char** argv) {
    arse::TestStructure ts;
    
    std::cout << "TestStructure size in bytes : " << ts.getSizeInBytes() << std::endl;
    uint64_t offset = 0;
    char* bytes = new char[ts.getSizeInBytes()];
    ts.serialise(bytes, offset);
    std::cout << "Offset after serialise : " << offset << std::endl;
    
    offset = 0;
    ts.deserialise(bytes, offset);
    std::cout << "Offset after deserialise : " << offset << std::endl;
    
    return 0;
}

