#include "CppGenerator.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

static const std::string TAB = "    ";

bool CppGenerator::generate(const Definitions& definitions,
                            const std::string& outputFolder) {
    for (const auto& structure : definitions.getStructure()) {
        generateHeaderFile(structure, outputFolder);
        generateSourceFile(structure, outputFolder);
    }
    
    for (const auto& enumeration : definitions.getEnumeration()) {
        generateHeaderFile(enumeration, outputFolder);
    }
}

bool CppGenerator::generateHeaderFile(const Structure& structure,
                                      const std::string& outputFolder) {    
    std::ofstream headerFile(outputFolder + '/' + structure.getName() + ".hxx");

    headerFile << "#pragma once\n\n";
    
    headerFile << generateIncludesHxx(structure);

    if (structure.getDocumentation().present()) {
        headerFile << "/**\n";
        headerFile << " * " << structure.getDocumentation().get() + '\n';
        headerFile << " */\n";
    }
    headerFile << "class " + structure.getName() + " : public Messaging::BaseMessage {\n";
    headerFile << insertTabs(1) << "public :\n";
    
    headerFile << generateConstants(structure);
    
    headerFile << generateConstructorHxx(structure);   
    
    headerFile << generateDestructorHxx(structure);   
    
    headerFile << generateSerialiseHxx();
    
    headerFile << generateDeserialiseHxx();
    
    headerFile << generateGetSizeInBytesHxx();
    
    headerFile << generateGettersHxx(structure);
    
    headerFile << generateSettersHxx(structure);

    headerFile << insertTabs(1) << "private :\n";
    
    headerFile << generateMembersHxx(structure);

    headerFile << "};\n";

    headerFile.close();
}

bool CppGenerator::generateHeaderFile(const Enumeration& enumeration,
                                      const std::string& outputFolder) {    
    std::ofstream headerFile(outputFolder + '/' + enumeration.getName() + ".hxx");

    headerFile << "#pragma once\n\n";
    
    headerFile << "#include <cstdint>\n\n";
    
    headerFile << "typedef enum class " << enumeration.getName() << " : " << convertBaseTypeToCppType(enumeration.getBaseType()) << " {\n";
    
    for (const auto& member : enumeration.getMember()) {
        if (member.getDocumentation().present()) {
            headerFile << insertTabs(1) << "/**\n";
            headerFile << insertTabs(1) << " * " << member.getDocumentation().get() + '\n';
            headerFile << insertTabs(1) << " */\n";
        }
            
        if (member.getValue().present()) {
            headerFile << insertTabs(1) << member.getName() << " = "<< member.getValue().get() << ",\n";
        } else {
            headerFile << insertTabs(1) << member.getName() << ",\n";
        }
    }
    
    headerFile.seekp(-2, headerFile.cur);
    headerFile << "\n} " << enumeration.getName() << ";\n";

    headerFile.close();
}

std::string CppGenerator::generateMembersHxx(const Structure& structure) {
    std::ostringstream os;

    for (const auto& element : structure.getElement()) {
        if (element.getDocumentation().present()) {
            os << insertTabs(2) << "/**\n";
            os << insertTabs(2) << " * " << element.getDocumentation().get() + '\n';
            os << insertTabs(2) << " */\n";
        }

        os << insertTabs(2) << convertElementToCppType(element) << ' ' << element.getName() << ";\n\n";
    }
    
    return os.str();
}

std::string CppGenerator::generateConstants(const Structure& structure) {
    std::ostringstream os;

    for (const auto& constant : structure.getConstant()) {
        if (constant.getDocumentation().present()) {
            os << insertTabs(2) << "/**\n";
            os << insertTabs(2) << " * " << constant.getDocumentation().get() + '\n';
            os << insertTabs(2) << " */\n";
        }

        os << insertTabs(2) << convertConstantToCppConstant(constant) << "\n\n";
    }
    
    return os.str();
}

std::string CppGenerator::generateIncludesHxx(const Structure& structure) {
    std::ostringstream os;
    
    os << "#include \"BaseMessage.hpp\"\n\n";
    os << "#include \"Serialiser.hpp\"\n\n";
    
    for (const auto& include : getListOfStructIncludes(structure)) {
	os << "#include \"" << include << ".hxx\"\n";
    }
    
    return os.str();
}

std::string CppGenerator::generateGettersHxx(const Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getElement()) {
        os << generateGetterHxx(element);
    }
    
    return os.str();
}

std::string CppGenerator::generateSettersHxx(const Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getElement()) {
        if (isPrimitiveType(element)) {
            os << generateSetterHxx(element);
        }
    }
    
    return os.str();
}

bool CppGenerator::generateSourceFile(const Structure& structure,
                                      const std::string& outputFolder) { 
    std::ofstream sourceFile(outputFolder + '/' + structure.getName() + ".cxx");

    sourceFile << "#include \"" << structure.getName() << ".hxx\"\n\n";

    sourceFile << generateConstructorCxx(structure) << "\n\n";

    sourceFile << generateDestructorCxx(structure) << "\n\n";

    sourceFile << generateSerialiseCxx(structure) << "\n\n";

    sourceFile << generateDeserialiseCxx(structure) << "\n\n";
    
    sourceFile << generateGetSizeInBytesCxx(structure) << "\n\n";

    for (const auto& element : structure.getElement()) {
        std::string upperName = element.getName().get();
        upperName[0] = toupper(upperName[0]);

        if (isPrimitiveType(element)) {
            sourceFile << "decltype(" << structure.getName() << "::" << element.getName().get() << ") " << structure.getName() << "::get" << upperName << "() {\n";
        } else {
            sourceFile << "decltype(" << structure.getName() << "::" << element.getName().get() << ")& " << structure.getName() << "::get" << upperName << "() {\n";
        }

        sourceFile << insertTabs(1) << "return " << element.getName().get() << ";\n";
        sourceFile << "}\n\n";
    }

    for (const auto& element : structure.getElement()) {
        std::string upperName = element.getName().get();
        upperName[0] = toupper(upperName[0]);

        if (isPrimitiveType(element)) {
            sourceFile << "void " << structure.getName() << "::set" << upperName << "(const decltype(" << structure.getName() << "::" << element.getName().get() << ") value) {\n";
            sourceFile << insertTabs(1) << "" << element.getName().get() << " = value;\n";
            sourceFile << "}\n\n";
        }
    }

    sourceFile.close();
}
    
std::string CppGenerator::generateConstructorHxx(const Structure& structure) {
    std::ostringstream os;
    
    os << insertTabs(2) << "/**\n";
    os << insertTabs(2) << " * Constructor for this class\n";
    os << insertTabs(2) << " */\n";
    os << insertTabs(2) << structure.getName() << "();\n\n";
    
    return os.str();
}
    
std::string CppGenerator::generateConstructorCxx(const Structure& structure) {
    std::ostringstream os;
    
    os << structure.getName() << "::" << structure.getName() << "() {\n";
    os << "\n";
    os << "}";
    
    return os.str();
}
    
std::string CppGenerator::generateDestructorHxx(const Structure& structure) {
    std::ostringstream os;
        
    os << insertTabs(2) << "/**\n";
    os << insertTabs(2) << " * Destructor for this class\n";
    os << insertTabs(2) << " */\n";
    os << insertTabs(2) << "~" << structure.getName() << "();\n\n";
    
    return os.str();
}
    
std::string CppGenerator::generateDestructorCxx(const Structure& structure) {
    std::ostringstream os;
    
    os << structure.getName() << "::~" << structure.getName() << "() {\n";
    os << "\n";
    os << "}";
    
    return os.str();
}

std::string CppGenerator::generateGetSizeInBytesHxx() {
    std::ostringstream os;   
    
    os << insertTabs(2) << "/**\n";
    os << insertTabs(2) << " * Get the serialised size of this class in bytes.\n";
    os << insertTabs(2) << " * Use to create an array for serialisation.\n";
    os << insertTabs(2) << " *\n";
    os << insertTabs(2) << " * @return The size of the serialised version of this class in bytes\n";
    os << insertTabs(2) << " */\n";
    os << insertTabs(2) << "uint64_t getSizeInBytes() const;\n\n"; 
    
    return os.str(); 
}

uint64_t CppGenerator::sizeOfPrimitiveType(const Element::TypeType type) {
    uint64_t size = 0;

    if ((type == "int_8") || (type == "unsigned_int_8")) {
        size = 1;
    } else if ((type == "int_16") || (type == "unsigned_int_16")) {
        size = 2;
    } else if ((type == "int_32") || (type == "unsigned_int_32")) {
        size = 4;
    } else if ((type == "int_64") || (type == "unsigned_int_64")) {
        size = 8;
    } else if (type == "float_32") {
        size = 4;
    } else if (type == "float_64") {
        size = 8;
    }

    return size;
}

std::string CppGenerator::generateGetSizeInBytesCxx(const Structure& structure) {
    std::ostringstream os;   
    uint64_t staticSize = 0;
    
    os << "uint64_t " << structure.getName() << "::getSizeInBytes() const {\n";
    
    for (const auto& element : structure.getElement()) {
        if (isPrimitiveType(element)) {
            staticSize += sizeOfPrimitiveType(element.getType());
        }
    }
    
    os << insertTabs(1) << "// Size of primitive types in this structure\n";
    os << insertTabs(1) << "uint64_t size = " << staticSize << ";\n\n";

    for (const auto& element : structure.getElement()) {
        if (!isPrimitiveType(element)) {
            os << insertTabs(1) << "// Calculate size of " << element.getName().get() << "\n";
            os << generateSizeStatement(element, element.getName().get()) << "\n\n";
        }
    }
    
    os << insertTabs(1) << "return size;\n";
    os << "}";
    
    return os.str(); 
}

std::string CppGenerator::generateSizeStatement(const Element& element,
                                                const std::string& nameToUse,
                                                const uint8_t numTabs) {
    std::ostringstream os;

    if (element.getType() == "array") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << insertTabs(numTabs) << "size += " << element.getArraySize().get() << " * sizeof(" << convertElementToCppType(element.getSubElement().get()) << ");";
        } else {
            os << insertTabs(numTabs) << "for (const auto& " << nameToUse << "Member : " << nameToUse << ") {\n";
            os << generateSizeStatement(element.getSubElement().get(), nameToUse + "Member", numTabs + 1) << "\n";
            os << insertTabs(numTabs) << "}";
        }
    } else if (element.getType() == "sequence") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << insertTabs(numTabs) << "size += (" << nameToUse << ".size() * sizeof(" << convertElementToCppType(element.getSubElement().get()) << ")) + sizeof(" << nameToUse << ".size());";
        } else {
            os << insertTabs(numTabs) << "for (const auto& " << nameToUse << "Member : " << nameToUse << ") {\n";
            os << generateSizeStatement(element.getSubElement().get(), nameToUse + "Member", numTabs + 1) << "\n";
            os << insertTabs(numTabs) << "}\n";
            os << insertTabs(numTabs) << "// Extra field for size of " << nameToUse << " sequence\n";
            os << insertTabs(numTabs) << "size += sizeof(" << nameToUse << ".size());";
        }
    } else if (element.getType() == "string") {
        os << insertTabs(numTabs) << "size += " << nameToUse << ".size() + sizeof(" << nameToUse << ".size());";
    } else if (element.getType() == "map") {
        os << insertTabs(numTabs) << "// Don't know what to do with map=" << nameToUse;
    } else if (element.getType() == "structure") {
        os << insertTabs(numTabs) << "size += " << nameToUse << ".getSizeInBytes();";
    }
    
    return os.str();
}

std::string CppGenerator::insertTabs(const uint8_t numTabs) {
    std::ostringstream os;
    
    for (uint8_t index = 0; index < numTabs; ++index) {
        os << TAB;
    }
    
    return os.str();
}



std::string CppGenerator::generateSerialiseHxx() {
    std::ostringstream os;   
    
    os << insertTabs(2) << "/**\n";
    os << insertTabs(2) << " * Serialise this class into a block of data\n";
    os << insertTabs(2) << " *\n";
    os << insertTabs(2) << " * @param data The data to serialise to (Ensure it is large enough!)\n";
    os << insertTabs(2) << " * @param offset The offset into the data to start serialising to. Will be updated with the new offset on return\n";
    os << insertTabs(2) << " * @return True if the operation was successful or false if not\n";
    os << insertTabs(2) << " */\n";
    os << insertTabs(2) << "void serialise(char* data,\n";
    os << insertTabs(2) << "               uint64_t& offset) const;\n\n"; 
    
    return os.str(); 
}

std::string CppGenerator::generateSerialiseCxx(const Structure& structure) {
    std::ostringstream os;   
    
    os << "void " << structure.getName() << "::serialise(char* data, uint64_t& offset) const {\n";
    
    if (structure.getElement().size() == 0) {
        os << insertTabs(1) << "// Nothing to serialise\n";
    } else {
        for (const auto& element : structure.getElement()) {
            os << insertTabs(1) << "// Serialise " << element.getName().get() << "\n";
            os << generateMemberSerialisation(element, element.getName().get()) << "\n\n";
        }
    }
    
    os.seekp(-1, os.cur);
    os << "}";

    return os.str();
}

std::string CppGenerator::generateMemberSerialisation(const Element& element,
                                                      const std::string& nameToUse,
                                                      const uint8_t numTabs) {
    std::ostringstream os;

    if (isPrimitiveType(element)) {
        os << insertTabs(numTabs) << "Serialiser::serialisePrimitive(data, " << nameToUse << ", offset);";
    } else if (element.getType() == "array") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << insertTabs(numTabs) << "Serialiser::serialisePrimitiveArray(data, " << nameToUse << ", offset);";
        } else {
            os << insertTabs(numTabs) << "for (const auto& " << nameToUse << "Member : " << nameToUse << ") {\n";
            os << generateMemberSerialisation(element.getSubElement().get(), nameToUse + "Member", numTabs + 1) << "\n";
            os << insertTabs(numTabs) << "}";
        }
    } else if (element.getType() == "sequence") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << insertTabs(numTabs) << "Serialiser::serialisePrimitiveSequence(data, " << nameToUse << ", offset);";
        } else {
            os << insertTabs(numTabs) << "for (const auto& " << nameToUse << "Member : " << nameToUse << ") {\n";
            os << generateMemberSerialisation(element.getSubElement().get(), nameToUse + "Member", numTabs + 1) << "\n";
            os << insertTabs(numTabs) << "}";
        }
    } else if (element.getType() == "string") {
        os << insertTabs(numTabs) << "Serialiser::serialiseString(data, " << nameToUse << ", offset);";
    } else if (element.getType() == "map") {
        os << insertTabs(numTabs) << "// Don't know what to do with map=" << nameToUse;
    } else if (element.getType() == "structure") {
        os << insertTabs(numTabs) << nameToUse << ".serialise(data, offset);";
    }
    
    return os.str();
}

std::string CppGenerator::generateDeserialiseHxx() {
    std::ostringstream os;   
    
    os << insertTabs(2) << "/**\n";
    os << insertTabs(2) << " * Deserialise a block of data into this class\n";
    os << insertTabs(2) << " *\n";
    os << insertTabs(2) << " * @param data The data to deserialise from\n";
    os << insertTabs(2) << " * @param offset The offset into the data to start deserialising from. Will be updated with the new offset on return\n";
    os << insertTabs(2) << " * @return True if the operation was successful or false if not\n";
    os << insertTabs(2) << " */\n";
    os << insertTabs(2) << "void deserialise(const char* data,\n";
    os << insertTabs(2) << "                 uint64_t& offset);\n\n"; 
    
    return os.str();   
}

std::string CppGenerator::generateDeserialiseCxx(const Structure& structure) {
    std::ostringstream os;   
    
    os << "void " << structure.getName() << "::deserialise(const char* data, uint64_t& offset) {\n";
    
    if (structure.getElement().size() == 0) {
        os << insertTabs(1) << "// Nothing to deserialise\n";
    } else {
        for (const auto& element : structure.getElement()) {
            os << insertTabs(1) << "// Deserialise " << element.getName().get() << "\n";
            os << generateMemberDeserialisation(element, element.getName().get()) << "\n\n";
        }
    }
    
    os.seekp(-1, os.cur);
    os << "}";

    return os.str();
}

std::string CppGenerator::generateMemberDeserialisation(const Element& element,
                                                        const std::string& nameToUse,
                                                        const uint8_t numTabs) {
    std::ostringstream os;

    if (isPrimitiveType(element)) {
        os << insertTabs(numTabs) << "Serialiser::deserialisePrimitive(data, " << nameToUse << ", offset);";
    } else if (element.getType() == "array") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << insertTabs(numTabs) << "Serialiser::deserialisePrimitiveArray(data, " << nameToUse << ", offset);";
        } else {
            os << insertTabs(numTabs) << "for (auto& " << nameToUse << "Member : " << nameToUse << ") {\n";
            os << generateMemberDeserialisation(element.getSubElement().get(), nameToUse + "Member", numTabs + 1) << "\n";
            os << insertTabs(numTabs) << "}";
        }
    } else if (element.getType() == "sequence") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << insertTabs(numTabs) << "Serialiser::deserialisePrimitiveSequence(data, " << nameToUse << ", offset);";
        } else {
            os << insertTabs(numTabs) << "for (auto& " << nameToUse << "Member : " << nameToUse << ") {\n";
            os << generateMemberDeserialisation(element.getSubElement().get(), nameToUse + "Member", numTabs + 1) << "\n";
            os << insertTabs(numTabs) << "}";
        }
    } else if (element.getType() == "string") {
        os << insertTabs(numTabs) << "Serialiser::deserialiseString(data, " << nameToUse << ", offset);";
    } else if (element.getType() == "map") {
        os << insertTabs(numTabs) << "// Don't know what to do with map=" << nameToUse;
    } else if (element.getType() == "structure") {
        os << insertTabs(numTabs) << nameToUse << ".deserialise(data, offset);";
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Element& element) {
    std::ostringstream os;
    std::string name = element.getName().get();
    
    name[0] = toupper(name[0]);
        
    os << insertTabs(2) << "/**\n";
    os << insertTabs(2) << " * Getter for " << element.getName().get() + '\n';

    if (element.getDocumentation().present()) {
        os << insertTabs(2) << " *\n";
        os << insertTabs(2) << " * " << element.getName().get() << " defined as : " << element.getDocumentation().get() << '\n';
    }

    os << insertTabs(2) << " *\n";
    os << insertTabs(2) << " * @return " << element.getName().get() << '\n';
    os << insertTabs(2) << " */\n";
    
    if (isPrimitiveType(element)) {
	os << insertTabs(2) << convertElementToCppType(element) << " get" << name << "();\n\n";
    } else {
	os << insertTabs(2) << convertElementToCppType(element) << "& get" << name << "();\n\n";
    }
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const Element& element) {
    std::ostringstream os;
    std::string name = element.getName().get();

    name[0] = toupper(name[0]);

    os << insertTabs(2) << "/**\n";
    os << insertTabs(2) << " * Setter for " << element.getName().get() + '\n';

    if (element.getDocumentation().present()) {
        os << insertTabs(2) << " *\n";
        os << insertTabs(2) << " * " << element.getName().get() << " defined as : " << element.getDocumentation().get() << '\n';
    }

    os << insertTabs(2) << " *\n";
    os << insertTabs(2) << " * @param value The new value to set\n";
    os << insertTabs(2) << " */\n";
    os << insertTabs(2) << "void set" << name << "(const " << convertElementToCppType(element) << " value);\n\n";
    
    return os.str();
}
    
std::string CppGenerator::convertElementToCppType(const Element& element) {
    std::ostringstream os;

    if (element.getType() == "unsigned_int_8") {
        os << "uint8_t";
    } else if (element.getType() == "unsigned_int_16") {
        os << "uint16_t";
    } else if (element.getType() == "unsigned_int_32") {
        os << "uint32_t";
    } else if (element.getType() == "unsigned_int_64") {
        os << "uint64_t";
    } else if (element.getType() == "int_8") {
        os << "int8_t";
    } else if (element.getType() == "int_16") {
        os << "int16_t";
    } else if (element.getType() == "int_32") {
        os << "int32_t";
    } else if (element.getType() == "int_64") {
        os << "int64_t";
    } else if (element.getType() == "float_32") {
        os << "float";
    } else if (element.getType() == "float_64") {
        os << "double";
    } else if (element.getType() == "string") {
        os << "std::string";
    } else if (element.getType() == "complex_unsigned_int_8") {
        os << "std::complex<uint8_t>";
    } else if (element.getType() == "complex_unsigned_int_16") {
        os << "std::complex<uint16_t>";
    } else if (element.getType() == "complex_unsigned_int_32") {
        os << "std::complex<uint32_t>";
    } else if (element.getType() == "complex_unsigned_int_64") {
        os << "std::complex<uint64_t>";
    } else if (element.getType() == "complex_int_8") {
        os << "std::complex<int8_t>";
    } else if (element.getType() == "complex_int_16") {
        os << "std::complex<int16_t>";
    } else if (element.getType() == "complex_int_32") {
        os << "std::complex<int32_t>";
    } else if (element.getType() == "complex_int_64") {
        os << "std::complex<int64_t>";
    } else if (element.getType() == "complex_float_32") {
        os << "std::complex<float>";
    } else if (element.getType() == "complex_float_64") {
        os << "std::complex<double>";
    } else if (element.getType() == "array") {
        os << "std::array<" << convertElementToCppType(element.getSubElement().get()) << ", " << element.getArraySize().get() << ">";
    } else if (element.getType() == "sequence") {
        os << "std::vector<" << convertElementToCppType(element.getSubElement().get()) << ">";
    } else if (element.getType() == "map") {
        os << "std::map<" << convertElementToCppType(element.getSubElement().get()) << ", " << convertElementToCppType(element.getSubElement2().get()) << ">";
    } else {
        os << element.getStructureName();
    }

    return os.str();
}
    
std::string CppGenerator::convertBaseTypeToCppType(const Enumeration::BaseTypeType& type) {
    std::ostringstream os;

    if (type == "unsigned_int_8") {
        os << "uint8_t";
    } else if (type == "unsigned_int_16") {
        os << "uint16_t";
    } else if (type == "unsigned_int_32") {
        os << "uint32_t";
    } else if (type == "unsigned_int_64") {
        os << "uint64_t";
    } else {
        os << "???";
    }

    return os.str();
}

std::string CppGenerator::convertConstantToCppConstant(const Constant& constant) {
    std::ostringstream os;

    if (constant.getType() == "unsigned_int_8") {
        os << "static const uint8_t " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "unsigned_int_16") {
        os << "static const uint16_t " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "unsigned_int_32") {
        os << "static const uint32_t " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "unsigned_int_64") {
        os << "static const uint64_t " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "int_8") {
        os << "static const int8_t " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "int_16") {
        os << "static const int16_t " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "int_32") {
        os << "static const int32_t " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "int_64") {
        os << "static const int64_t " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "float_32") {
        os << "static const float " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "float_64") {
        os << "static const double " << constant.getName() << " = " << constant.getValue() << ";";
    } else if (constant.getType() == "string") {
        os << "static const std::string " << constant.getName() << " = \"" << constant.getValue() << "\";";
    } else {
        os << "Parsing error";
    }

    return os.str();
}

bool CppGenerator::isPrimitiveType(const Element& element) {
    bool returnVal = false;

    if ((element.getType() == "unsigned_int_8") ||
        (element.getType() == "unsigned_int_16") ||
        (element.getType() == "unsigned_int_32") ||
        (element.getType() == "unsigned_int_64") ||
        (element.getType() == "int_8") ||
        (element.getType() == "int_16") ||
        (element.getType() == "int_32") ||
        (element.getType() == "int_64") ||
        (element.getType() == "float_32") ||
        (element.getType() == "float_64") ||
        (element.getType() == "enumeration")) {
        returnVal = true;
    }

    return returnVal;
}

std::set<std::string> CppGenerator::getListOfStructIncludes(const Structure& structure) {
    std::set<std::string> returnVal;
    
    for (const auto& element : structure.getElement()) {
        std::set<std::string> includes = getListOfStructIncludes(element);

        returnVal.insert(includes.begin(), includes.end());
    }
    
    return returnVal;
}

std::set<std::string> CppGenerator::getListOfStructIncludes(const Element& element) {
    std::set<std::string> returnVal;
    
    if ((element.getType() == "structure") ||
        (element.getType() == "enumeration")) {
	if (element.getStructureName().present()) {
	    returnVal.insert(element.getStructureName().get());
	}
    }
    
    if (element.getSubElement().present()) {
	std::set<std::string> includes = getListOfStructIncludes(element.getSubElement().get());
	
	returnVal.insert(includes.begin(), includes.end());
    }
    
    if (element.getSubElement2().present()) {
	std::set<std::string> includes = getListOfStructIncludes(element.getSubElement2().get());
	
	returnVal.insert(includes.begin(), includes.end());
    }
    
    return returnVal;
}
