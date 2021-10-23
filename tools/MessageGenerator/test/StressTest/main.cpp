#include <iostream>

#include "TestStructure.hxx"

int main(int argc, char** argv) {
    arse::TestStructure ts;
    
    std::cout << "TestStructure size in bytes : " << ts.getSizeInBytes() << std::endl;
    
    return 0;
}

