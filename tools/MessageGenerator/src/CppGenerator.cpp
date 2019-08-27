#include "CppGenerator.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

bool CppGenerator::generate(const Definitions& definitions,
                            const std::string& outputFolder) {
    for (auto& structure : definitions.getStructure()) {
        generateHeaderFile(structure, outputFolder);
        generateSourceFile(structure, outputFolder);
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
    headerFile << "\tpublic :\n";
    
    headerFile << generateConstants(structure);
    
    headerFile << generateConstructorHxx(structure);   
    
    headerFile << generateDestructorHxx(structure);   
    
    headerFile << generateSerialiseHxx();
    
    headerFile << generateDeserialiseHxx();
    
    headerFile << generateGettersHxx(structure);
    
    headerFile << generateSettersHxx(structure);

    headerFile << "\tprivate :\n";
    
    headerFile << generateMembersHxx(structure);

    headerFile << "};\n";

    headerFile.close();
}

std::string CppGenerator::generateMembersHxx(const Structure& structure) {
    std::ostringstream os;

    for (const auto& element : structure.getElement()) {
        if (element.getDocumentation().present()) {
            os << "\t\t/**\n";
            os << "\t\t * " << element.getDocumentation().get() + '\n';
            os << "\t\t */\n";
        }

        os << "\t\t" << convertElementToCppType(element) << ' ' << element.getName() << ";\n\n";
    }
    
    return os.str();
}

std::string CppGenerator::generateConstants(const Structure& structure) {
    std::ostringstream os;

    for (const auto& constant : structure.getConstant()) {
        if (constant.getDocumentation().present()) {
            os << "\t\t/**\n";
            os << "\t\t * " << constant.getDocumentation().get() + '\n';
            os << "\t\t */\n";
        }

        os << "\t\t" << convertConstantToCppConstant(constant) << "\n\n";
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

    for (const auto& element : structure.getElement()) {
        std::string upperName = element.getName().get();
        upperName[0] = toupper(upperName[0]);

        if (isPrimitiveType(element)) {
            sourceFile << "decltype(" << structure.getName() << "::" << element.getName().get() << ") " << structure.getName() << "::get" << upperName << "() {\n";
        } else {
            sourceFile << "decltype(" << structure.getName() << "::" << element.getName().get() << ")& " << structure.getName() << "::get" << upperName << "() {\n";
        }

        sourceFile << "\treturn " << element.getName().get() << ";\n";
        sourceFile << "}\n\n";
    }

    for (const auto& element : structure.getElement()) {
        std::string upperName = element.getName().get();
        upperName[0] = toupper(upperName[0]);

        if (isPrimitiveType(element)) {
            sourceFile << "void " << structure.getName() << "::set" << upperName << "(const decltype(" << structure.getName() << "::" << element.getName().get() << ") value) {\n";
            sourceFile << "\t" << element.getName().get() << " = value;\n";
            sourceFile << "}\n\n";
        }
    }

    sourceFile.close();
}
    
std::string CppGenerator::generateConstructorHxx(const Structure& structure) {
    std::ostringstream os;
    
    os << "\t\t/**\n";
    os << "\t\t * Constructor for this class\n";
    os << "\t\t */\n";
    os << "\t\t" << structure.getName() << "();\n\n";
    
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
        
    os << "\t\t/**\n";
    os << "\t\t * Destructor for this class\n";
    os << "\t\t */\n";
    os << "\t\t~" << structure.getName() << "();\n\n";
    
    return os.str();
}
    
std::string CppGenerator::generateDestructorCxx(const Structure& structure) {
    std::ostringstream os;
    
    os << structure.getName() << "::~" << structure.getName() << "() {\n";
    os << "\n";
    os << "}";
    
    return os.str();
}

std::string CppGenerator::generateSerialiseHxx() {
    std::ostringstream os;   
    
    os << "\t\t/**\n";
    os << "\t\t * Serialise this class into a block of data\n";
    os << "\t\t *\n";
    os << "\t\t * @param data The data to serialise to (Ensure it is large enough!)\n";
    os << "\t\t * @param offset The offset into the data to start serialising to. Will be updated with the new offset on return\n";
    os << "\t\t * @return True if the operation was successful or false if not\n";
    os << "\t\t */\n";
    os << "\t\tvoid serialise(char* data,\n";
    os << "\t\t               uint64_t& offset);\n\n"; 
    
    return os.str(); 
}

std::string CppGenerator::generateSerialiseCxx(const Structure& structure) {
    std::ostringstream os;   
    
    os << "void " << structure.getName() << "::serialise(char* data, uint64_t& offset) {\n";
    
    if (structure.getElement().size() == 0) {
        os << "\t// Nothing to serialise\n";
    } else {
        for (const auto& element : structure.getElement()) {
            os << generateMemberSerialisation(element, element.getName().get());
        }
    }
    os << "}";

    return os.str();
}

std::string CppGenerator::generateMemberSerialisation(const Element& element,
                                                      const std::string& nameToUse) {
    std::ostringstream os;

    if (isPrimitiveType(element)) {
        os << "\tSerialiser::serialisePrimitive(data, " << nameToUse << ", offset);\n";
    } else if (element.getType() == "array") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << "\tSerialiser::serialisePrimitiveArray(data, " << nameToUse << ", offset);\n";
        } else {
            os << "\tfor (auto& " << nameToUse << "a : " << nameToUse << ") {\n";
            os << "\t" << generateMemberSerialisation(element.getSubElement().get(), nameToUse + "a");
            os << "\t}\n";
        }
    } else if (element.getType() == "sequence") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << "\tSerialiser::serialisePrimitiveSequence(data, " << nameToUse << ", offset);\n";
        } else {
            os << "\tfor (auto& " << nameToUse << "a : " << nameToUse << ") {\n";
            os << "\t" << generateMemberSerialisation(element.getSubElement().get(), nameToUse + "a");
            os << "\t}\n";
        }
    } else if (element.getType() == "string") {
        os << "\tSerialiser::serialiseString(data, " << nameToUse << ", offset);\n";
    } else if (element.getType() == "map") {
        // map
    } else if (element.getType() == "structure") {
        os << "\t" << nameToUse << ".serialise(data, offset);\n";
    }
    
    return os.str();
}

std::string CppGenerator::generateDeserialiseHxx() {
    std::ostringstream os;   
    
    os << "\t\t/**\n";
    os << "\t\t * Deserialise a block of data into this class\n";
    os << "\t\t *\n";
    os << "\t\t * @param data The data to deserialise from\n";
    os << "\t\t * @param offset The offset into the data to start deserialising from. Will be updated with the new offset on return\n";
    os << "\t\t * @return True if the operation was successful or false if not\n";
    os << "\t\t */\n";
    os << "\t\tvoid deserialise(const char* data,\n";
    os << "\t\t                 uint64_t& offset);\n\n"; 
    
    return os.str();   
}

std::string CppGenerator::generateDeserialiseCxx(const Structure& structure) {
    std::ostringstream os;   
    
    os << "void " << structure.getName() << "::deserialise(const char* data, uint64_t& offset) {\n";
    
    if (structure.getElement().size() == 0) {
        os << "\t// Nothing to deserialise\n";
    } else {
        for (const auto& element : structure.getElement()) {
            os << generateMemberDeserialisation(element, element.getName().get());
        }
    }
    os << "}";

    return os.str();
}

std::string CppGenerator::generateMemberDeserialisation(const Element& element,
                                                        const std::string& nameToUse) {
    std::ostringstream os;

    if (isPrimitiveType(element)) {
        os << "\tSerialiser::deserialisePrimitive(data, " << nameToUse << ", offset);\n";
    } else if (element.getType() == "array") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << "\tSerialiser::deserialisePrimitiveArray(data, " << nameToUse << ", offset);\n";
        } else {
            os << "\tfor (auto& " << nameToUse << "a : " << nameToUse << ") {\n";
            os << "\t" << generateMemberDeserialisation(element.getSubElement().get(), nameToUse + "a");
            os << "\t}\n";
        }
    } else if (element.getType() == "sequence") {
        if (isPrimitiveType(element.getSubElement().get())) {
            os << "\tSerialiser::deserialisePrimitiveSequence(data, " << nameToUse << ", offset);\n";
        } else {
//            os << "\tfor (auto& " << nameToUse << "a : " << nameToUse << ") {\n";
//            os << "\t" << generateMemberDeserialisation(element.getSubElement().get(), nameToUse + "a");
//            os << "\t}\n";
        }
    } else if (element.getType() == "string") {
        os << "\tSerialiser::deserialiseString(data, " << nameToUse << ", offset);\n";
    } else if (element.getType() == "map") {
        // map
    } else if (element.getType() == "structure") {
        os << "\t" << nameToUse << ".deserialise(data, offset);\n";
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Element& element) {
    std::ostringstream os;
    std::string name = element.getName().get();
    
    name[0] = toupper(name[0]);
        
    os << "\t\t/**\n";
    os << "\t\t * Getter for " << element.getName().get() + '\n';

    if (element.getDocumentation().present()) {
        os << "\t\t *\n";
        os << "\t\t * " << element.getName().get() << " defined as : " << element.getDocumentation().get() << '\n';
    }

    os << "\t\t *\n";
    os << "\t\t * @return " << element.getName().get() << '\n';
    os << "\t\t */\n";
    
    if (isPrimitiveType(element)) {
	os << "\t\t" << convertElementToCppType(element) << " get" << name << "();\n\n";
    } else {
	os << "\t\t" << convertElementToCppType(element) << "& get" << name << "();\n\n";
    }
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const Element& element) {
    std::ostringstream os;
    std::string name = element.getName().get();

    name[0] = toupper(name[0]);

    os << "\t\t/**\n";
    os << "\t\t * Setter for " << element.getName().get() + '\n';

    if (element.getDocumentation().present()) {
        os << "\t\t *\n";
        os << "\t\t * " << element.getName().get() << " defined as : " << element.getDocumentation().get() << '\n';
    }

    os << "\t\t *\n";
    os << "\t\t * @param value The new value to set\n";
    os << "\t\t */\n";
    os << "\t\tvoid set" << name << "(const " << convertElementToCppType(element) << " value);\n\n";
    
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
        (element.getType() == "float_64")) {
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
    
    if (element.getType() == "structure") {
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
