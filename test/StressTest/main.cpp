#include <iostream>

#include "MapTest.hxx"
#include "OptionalTest.hxx"

int main(int argc, char** argv) {
    {
        MyNamespace::MapTest t;
        MyNamespace::MapTest t2;

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
        std::cout << "\tOffset after serialise : " << offset << std::endl;

        offset = 0;
        t2.deserialise(bytes, offset);
        std::cout << "\tOffset after deserialise : " << offset << std::endl;

        delete [] bytes;
    }
    
    {
        MyNamespace::OptionalTest o;
        MyNamespace::OptionalTest o2;
        o.setLastElement(19);

        o.setEnumeration(MyNamespace::Colour::AMBER);
        o.setPrimitive(444);
        o.getStructure() = MyNamespace::TestSubStruct();
        o.getStructure().value().setBigUnsigned(88);
        std::cout << "big unsigned is " << o.getStructure().value().getBigUnsigned() << std::endl;
        o.getArray() = {1, 2, 3};
        o.getSequence() = {MyNamespace::Colour::GREEN, MyNamespace::Colour::AMBER, MyNamespace::Colour::RED};
        o.getMap() = {{1, MyNamespace::Colour::RED}, {2, MyNamespace::Colour::GREEN}};

        std::cout << "OptionalTest size in bytes : " << o.getSizeInBytes() << std::endl;
        uint64_t offset = 0;
        char* bytes = new char[o.getSizeInBytes()];
        o.serialise(bytes, offset);
        std::cout << "\tOffset after serialise : " << offset << std::endl;

        offset = 0;
        o2.deserialise(bytes, offset);
        std::cout << "\tOffset after deserialise : " << offset << std::endl;
        std::cout << "\tlastElement is " << o2.getLastElement() << std::endl;
        std::cout << "big unsigned is " << o2.getStructure().value().getBigUnsigned() << std::endl;
        std::cout << "primitive is " << o2.getPrimitive().value() << std::endl;
        std::cout << "array[2] is " << o2.getArray().value()[2] << std::endl;

        delete [] bytes;
    }
    
    return 0;
}

