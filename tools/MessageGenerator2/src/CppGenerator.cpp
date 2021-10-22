#include "CppGenerator.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <filesystem>

bool CppGenerator::generate(const Definitions& definitions,
                            const std::string& outputFolder) {
    bool returnVal = true;
    
    setNamespace(definitions.getNamespace().present());
    
    for (const auto& structure : definitions.getStructure()) {
        returnVal &= generateHeaderFile(structure, outputFolder, definitions.getNamespace());
        returnVal &= generateSourceFile(structure, outputFolder, definitions.getNamespace());
    }
    
    for (const auto& enumeration : definitions.getEnumeration()) {
        returnVal &= generateHeaderFile(enumeration, outputFolder, definitions.getNamespace());
    }
    
    return returnVal;
}

bool CppGenerator::generateHeaderFile(const Structure& structure,
                                      const std::string& outputFolder,
                                      const Definitions::NamespaceOptional& namespaceOptional) {
    bool returnVal = true;
    
    std::ofstream headerFile(outputFolder + std::filesystem::path::preferred_separator + structure.getName() + ".hxx");

    headerFile << "#pragma once" << std::endl;
    headerFile << std::endl;
    
    headerFile << generateIncludesHxx(structure) << std::endl;
    
    if (namespaceOptional.present()) {
        headerFile << "namespace " << namespaceOptional.get() << " {" << std::endl;
    }

    if (structure.getDocumentation().present()) {
        headerFile << insertTabs() << "/**" << std::endl;
        headerFile << insertTabs() << " * " << structure.getDocumentation().get() << std::endl;
        headerFile << insertTabs() << " */" << std::endl;
    }
    headerFile << insertTabs() << "class " + structure.getName() + " : public Messaging::BaseMessage {" << std::endl;
    headerFile << insertTabs(1) << "public :" << std::endl;
    
    headerFile << generateConstructorHxx(structure) << std::endl;
    
    headerFile << generateDestructorHxx(structure) << std::endl;
    
    headerFile << generateSerialiseHxx() << std::endl;
    
    headerFile << generateDeserialiseHxx() << std::endl;
    
    headerFile << generateGetSizeInBytesHxx() << std::endl;
    
    headerFile << generateGettersHxx(structure);
    
    headerFile << generateSettersHxx(structure);

    headerFile << insertTabs(1) << "private :" << std::endl;
    
    headerFile << generateMembersHxx(structure);
        
    headerFile.seekp(-1, headerFile.cur);

    headerFile << insertTabs() << "};" << std::endl;
    
    if (namespaceOptional.present()) {
        headerFile << "}" << std::endl;
    }

    headerFile.close();
    
    return returnVal;
}

bool CppGenerator::generateHeaderFile(const Enumeration& enumeration,
                                      const std::string& outputFolder,
                                      const Definitions::NamespaceOptional& namespaceOptional) {
    bool returnVal = true;    
    
    std::ofstream headerFile(outputFolder + std::filesystem::path::preferred_separator + enumeration.getName() + ".hxx");

    headerFile << "#pragma once" << std::endl;
    headerFile << std::endl;
    
    headerFile << "#include <cstdint>" << std::endl;
    headerFile << std::endl;
    
    if (namespaceOptional.present()) {
        headerFile << "namespace " << namespaceOptional.get() << " {" << std::endl;
    }
    
    headerFile << insertTabs() << "enum class " << enumeration.getName() << " : " << convertEnumToCppBaseType(enumeration) << " {" << std::endl;
    
    for (const auto& member : enumeration.getMember()) {
        if (member.getDocumentation().present()) {
            headerFile << insertTabs(1) << "/**" << std::endl;
            headerFile << insertTabs(1) << " * " << member.getDocumentation().get() << std::endl;
            headerFile << insertTabs(1) << " */" << std::endl;
        }
            
        if (member.getValue().present()) {
            headerFile << insertTabs(1) << member.getName() << " = "<< member.getValue().get() << "," << std::endl;
        } else {
            headerFile << insertTabs(1) << member.getName() << "," << std::endl;
        }
    }
    
    headerFile.seekp(-2, headerFile.cur);
    headerFile << std::endl;
    headerFile << insertTabs() << "};" << std::endl;
    
    if (namespaceOptional.present()) {
        headerFile << "}" << std::endl;
    }

    headerFile.close();
    
    return returnVal;
}

bool CppGenerator::generateSourceFile(const Structure& structure,
                                      const std::string& outputFolder,
                                      const Definitions::NamespaceOptional& namespaceOptional) {
    bool returnVal = true;
    std::ofstream sourceFile(outputFolder + std::filesystem::path::preferred_separator + structure.getName() + ".cxx");

    sourceFile << "#include \"" << structure.getName() << ".hxx\"" << std::endl;
    sourceFile << std::endl;
    
    if (namespaceOptional.present()) {
        sourceFile << "namespace " << namespaceOptional.get() << " {" << std::endl;
    }

    sourceFile << generateConstructorCxx(structure) << std::endl;

    sourceFile << generateDestructorCxx(structure) << std::endl;

    sourceFile << generateSerialiseCxx(structure) << std::endl;

    sourceFile << generateDeserialiseCxx(structure) << std::endl;
    
    sourceFile << generateGetSizeInBytesCxx(structure) << std::endl;

    for (const auto& element : structure.getPrimitiveElement()) {
        std::string upperName = element.getName();
        upperName[0] = toupper(upperName[0]);

        sourceFile << insertTabs() << convertPrimitiveElementToCppType(element) << ' ' << structure.getName() << "::get" << upperName << "() {" << std::endl;
        sourceFile << insertTabs(1) << "return " << element.getName() << ";" << std::endl;
        sourceFile << insertTabs() << '}' << std::endl;
        sourceFile << std::endl;
    }
    
    sourceFile.seekp(-1, sourceFile.cur);

    for (const auto& element : structure.getPrimitiveElement()) {
        std::string upperName = element.getName();
        upperName[0] = toupper(upperName[0]);

        sourceFile << insertTabs() << "void " << structure.getName() << "::set" << upperName << "(const " << convertPrimitiveElementToCppType(element) << " value) {" << std::endl;
        sourceFile << insertTabs(1) << element.getName() << " = value;" << std::endl;
        sourceFile << insertTabs() << '}' << std::endl;
        sourceFile << std::endl;
    }
    
    sourceFile.seekp(-1, sourceFile.cur);
    
    if (namespaceOptional.present()) {
        sourceFile << "}" << std::endl;
    }

    sourceFile.close();
    
    return returnVal;
}
    
std::string CppGenerator::generateConstructorHxx(const Structure& structure) {
    std::ostringstream os;
    
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Constructor for this class" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << structure.getName() << "();" << std::endl;
    
    return os.str();
}
    
std::string CppGenerator::generateDestructorHxx(const Structure& structure) {
    std::ostringstream os;
        
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Destructor for this class" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << "~" << structure.getName() << "();" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSerialiseHxx() {
    std::ostringstream os;   
    
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Serialise this class into a block of data" << std::endl;
    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @param data The data to serialise to (Ensure it is large enough!)" << std::endl;
    os << insertTabs(2) << " * @param offset The offset into the data to start serialising to. Will be updated with the new offset on return" << std::endl;
    os << insertTabs(2) << " * @return True if the operation was successful or false if not" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << "void serialise(char* data," << std::endl;
    os << insertTabs(2) << "               uint64_t& offset) const;" << std::endl;
    
    return os.str(); 
}

std::string CppGenerator::generateDeserialiseHxx() {
    std::ostringstream os;   
    
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Deserialise a block of data into this class" << std::endl;
    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @param data The data to deserialise from" << std::endl;
    os << insertTabs(2) << " * @param offset The offset into the data to start deserialising from. Will be updated with the new offset on return" << std::endl;
    os << insertTabs(2) << " * @return True if the operation was successful or false if not" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << "void deserialise(const char* data," << std::endl;
    os << insertTabs(2) << "                 uint64_t& offset);" << std::endl;
    
    return os.str();   
}

std::string CppGenerator::generateGetSizeInBytesHxx() {
    std::ostringstream os;   
    
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Get the serialised size of this class in bytes." << std::endl;
    os << insertTabs(2) << " * Use to create an array for serialisation." << std::endl;
    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @return The size of the serialised version of this class in bytes" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << "uint64_t getSizeInBytes() const;" << std::endl;
    
    return os.str(); 
}
    
std::string CppGenerator::generateConstructorCxx(const Structure& structure) {
    std::ostringstream os;
    
    os << insertTabs() << structure.getName() << "::" << structure.getName() << "() {" << std::endl;
    os << std::endl;
    os << insertTabs() << "}" << std::endl;
    
    return os.str();
}
    
std::string CppGenerator::generateDestructorCxx(const Structure& structure) {
    std::ostringstream os;
    
    os << insertTabs() << structure.getName() << "::~" << structure.getName() << "() {" << std::endl;
    os << std::endl;
    os << insertTabs() << "}" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGettersHxx(const Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getPrimitiveElement()) {
        os << generateGetterHxx(element) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateSettersHxx(const Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getPrimitiveElement()) {
        os << generateSetterHxx(element) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const PrimitiveElement& element) {
    std::ostringstream os;
    std::string name = element.getName();
    
    name[0] = toupper(name[0]);
        
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Getter for " << element.getName() << std::endl;

    if (element.getDocumentation().present()) {
        os << insertTabs(2) << " *" << std::endl;
        os << insertTabs(2) << " * " << element.getName() << " defined as : " << element.getDocumentation().get() << std::endl;
    }

    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @return " << element.getName() << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    
    os << insertTabs(2) << convertPrimitiveElementToCppType(element) << " get" << name << "() const;" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const PrimitiveElement& element) {
    std::ostringstream os;
    std::string name = element.getName();

    name[0] = toupper(name[0]);

    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Setter for " << element.getName() << std::endl;

    if (element.getDocumentation().present()) {
        os << insertTabs(2) << " *" << std::endl;
        os << insertTabs(2) << " * " << element.getName() << " defined as : " << element.getDocumentation().get() << std::endl;
    }

    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @param value The new value to set" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << "void set" << name << "(const " << convertPrimitiveElementToCppType(element) << " value);" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateMembersHxx(const Structure& structure) {
    std::ostringstream os;

    for (const auto& element : structure.getPrimitiveElement()) {
        if (element.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << element.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        os << insertTabs(2) << convertPrimitiveElementToCppType(element) << ' ' << element.getName() << ';' << std::endl;
        os << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateIncludesHxx(const Structure& structure) {
    std::ostringstream os;
    
    os << "#include <cstdint>" << std::endl;
    os << std::endl;
    
    os << "#include \"BaseMessage.hpp\"" << std::endl;
    os << "#include \"Serialiser.hpp\"" << std::endl;
    
//    for (const auto& include : getListOfStructIncludes(structure)) {
//	os << "#include \"" << include << ".hxx\"" << std::endl;
//    }
    
    return os.str();
}

std::string CppGenerator::generateGetSizeInBytesCxx(const Structure& structure) {
    std::ostringstream os;   
    uint64_t size = 0;
    
    os << insertTabs(0) << "uint64_t " << structure.getName() << "::getSizeInBytes() {" << std::endl;
    
    for (const auto& element : structure.getPrimitiveElement()) {
        size += sizeOfPrimitiveElement(element);
    }
    
    os << insertTabs(1) << "// Size of primitive types in this structure" << std::endl;
    os << insertTabs(1) << "uint64_t size = " << size << ';' << std::endl;
    os << std::endl;    
    os << insertTabs(1) << "return size;" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str(); 
}

std::string CppGenerator::generateSerialiseCxx(const Structure& structure) {
    std::ostringstream os;   
    
    os << insertTabs() << "void " << structure.getName() << "::serialise(char* data, uint64_t& offset) const {" << std::endl;
    
    if (structure.getPrimitiveElement().size() == 0) {
        os << insertTabs(1) << "// Nothing to serialise" << std::endl;
    } else {
        for (const auto& element : structure.getPrimitiveElement()) {
            os << insertTabs(1) << "// Serialise " << element.getName() << std::endl;
            os << insertTabs(1) << "memcpy(data + offset, &" << element.getName() << ", sizeof (" << convertPrimitiveElementToCppType(element) << "));" << std::endl;
            os << insertTabs(1) << "offset += sizeof (" << convertPrimitiveElementToCppType(element) << ");" << std::endl;
            os << std::endl;
        }
    }
    
    os.seekp(-1, os.cur);
    os << insertTabs() << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialiseCxx(const Structure& structure) {
    std::ostringstream os;   
    
    os << insertTabs() << "void " << structure.getName() << "::deserialise(const char* data, uint64_t& offset) {" << std::endl;
    
    if (structure.getPrimitiveElement().size() == 0) {
        os << insertTabs(1) << "// Nothing to deserialise" << std::endl;
    } else {
        for (const auto& element : structure.getPrimitiveElement()) {
            os << insertTabs(1) << "// Deserialise " << element.getName() << std::endl;
            os << insertTabs(1) << "memcpy(&" << element.getName() << ", data + offset, sizeof (" << convertPrimitiveElementToCppType(element) << "));" << std::endl;
            os << insertTabs(1) << "offset += sizeof (" << convertPrimitiveElementToCppType(element) << ");" << std::endl;
            os << std::endl;
        }
    }
    
    os.seekp(-1, os.cur);
    os << insertTabs() << '}' << std::endl;

    return os.str();
}

uint64_t CppGenerator::sizeOfPrimitiveElement(const PrimitiveElement& element) {
    uint64_t size = 0;

    if ((element.getType() == "signed_int_8") || (element.getType() == "unsigned_int_8")) {
        size = 1;
    } else if ((element.getType() == "signed_int_16") || (element.getType() == "unsigned_int_16")) {
        size = 2;
    } else if ((element.getType() == "signed_int_32") || (element.getType() == "unsigned_int_32")) {
        size = 4;
    } else if ((element.getType() == "signed_int_64") || (element.getType() == "unsigned_int_64")) {
        size = 8;
    } else if (element.getType() == "float_32") {
        size = 4;
    } else if (element.getType() == "float_64") {
        size = 8;
    }

    return size;
}
    
std::string CppGenerator::convertEnumToCppBaseType(const Enumeration& enumeration) {
    std::ostringstream os;

    if (enumeration.getBaseType() == "unsigned_int_8") {
        os << "uint8_t";
    } else if (enumeration.getBaseType() == "unsigned_int_16") {
        os << "uint16_t";
    } else if (enumeration.getBaseType() == "unsigned_int_32") {
        os << "uint32_t";
    } else if (enumeration.getBaseType() == "unsigned_int_64") {
        os << "uint64_t";
    } else if (enumeration.getBaseType() == "signed_int_8") {
        os << "int8_t";
    } else if (enumeration.getBaseType() == "signed_int_16") {
        os << "int16_t";
    } else if (enumeration.getBaseType() == "signed_int_32") {
        os << "int32_t";
    } else if (enumeration.getBaseType() == "signed_int_64") {
        os << "int64_t";
    } else {
        os << "???";
    }

    return os.str();
}
    
std::string CppGenerator::convertPrimitiveElementToCppType(const PrimitiveElement& element) {
    std::ostringstream os;

    if (element.getType() == "unsigned_int_8") {
        os << "uint8_t";
    } else if (element.getType() == "unsigned_int_16") {
        os << "uint16_t";
    } else if (element.getType() == "unsigned_int_32") {
        os << "uint32_t";
    } else if (element.getType() == "unsigned_int_64") {
        os << "uint64_t";
    } else if (element.getType() == "signed_int_8") {
        os << "int8_t";
    } else if (element.getType() == "signed_int_16") {
        os << "int16_t";
    } else if (element.getType() == "signed_int_32") {
        os << "int32_t";
    } else if (element.getType() == "signed_int_64") {
        os << "int64_t";
    } else if (element.getType() == "float_32") {
        os << "float";
    } else if (element.getType() == "float_64") {
        os << "double";
    } else {
        os << "???";
    }

    return os.str();
}