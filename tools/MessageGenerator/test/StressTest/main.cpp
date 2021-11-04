#include <iostream>

#include "MapTest.hxx"

int main(int argc, char** argv) {
    MyNamespace::MapTest t;
    MyNamespace::MapTest t2;
    t.setColour(MyNamespace::Colour::RED);
    t.setLastElement(42);
    
    t.getPrimitiveToPrimitive().insert({1, 2});
    t.getPrimitiveToPrimitive().insert({3, 4});
    
    t.getPrimitiveToEnum().insert({1, MyNamespace::Colour::AMBER});
    t.getPrimitiveToEnum().insert({2, MyNamespace::Colour::GREEN});
    
    t.getPrimitiveToStruct().insert({1, MyNamespace::TestSubStruct()});
    t.getPrimitiveToStruct().insert({2, MyNamespace::TestSubStruct()});
    
    t.getPrimitiveToSequence().insert({1, {{1, 2, 3}, {4, 5, 6}}});
    t.getPrimitiveToSequence().insert({2, {{2, 3, 4, 5}, {6, 7, 8, 9}}});
    
    t.getPrimitiveToArray().insert({1, {{{1, 2}, {3, 4}}}});
    t.getPrimitiveToArray().insert({2, {{{5, 6}, {7, 8}}}});
    
    t.getPrimitiveToMap().insert({1, {{2, {{3, MyNamespace::Colour::GREEN}}}}});    
    t.getPrimitiveToMap().insert({2, {{3, {{4, MyNamespace::Colour::RED}}}}});
    
    std::cout << "MapTest size in bytes : " << t.getSizeInBytes() << std::endl;
    uint64_t offset = 0;
    char* bytes = new char[t.getSizeInBytes()];
    t.serialise(bytes, offset);
    std::cout << "Offset after serialise : " << offset << std::endl;
    
    offset = 0;
    t2.deserialise(bytes, offset);
    std::cout << "Offset after deserialise : " << offset << std::endl;
    std::cout << "lastElement is " << t.getLastElement();
    
    delete [] bytes;
    return 0;
}

