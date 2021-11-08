#include "CppGenerator.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <filesystem>
#include <set>

const std::string CppGenerator::SEQ_SIZE_TYPE = "uint64_t";

CppGenerator::CppGenerator(const Messaging::Definitions& definitions) : Generator(definitions) {
    sanityCheck();
}

void CppGenerator::sanityCheck() {
    bool error = false;
    
    if (error) {
        std::exit(EXIT_FAILURE);
    }
}

bool CppGenerator::generate(const std::string& outputFolder) {
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

bool CppGenerator::generateHeaderFile(const Messaging::Structure& structure,
                                      const std::string& outputFolder,
                                      const Messaging::Definitions::NamespaceOptional& namespaceOptional) {
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

bool CppGenerator::generateHeaderFile(const Messaging::Enumeration& enumeration,
                                      const std::string& outputFolder,
                                      const Messaging::Definitions::NamespaceOptional& namespaceOptional) {
    bool returnVal = true;    
    
    std::ofstream headerFile(outputFolder + std::filesystem::path::preferred_separator + enumeration.getName() + ".hxx");

    headerFile << "#pragma once" << std::endl;
    headerFile << std::endl;
    
    headerFile << "#include <cstdint>" << std::endl;
    headerFile << std::endl;
    
    if (namespaceOptional.present()) {
        headerFile << "namespace " << namespaceOptional.get() << " {" << std::endl;
    }
    
    headerFile << insertTabs() << "enum class " << enumeration.getName() << " : " << convertEnumBaseTypeToCppType(enumeration.getBaseType()) << " {" << std::endl;
    
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

bool CppGenerator::generateSourceFile(const Messaging::Structure& structure,
                                      const std::string& outputFolder,
                                      const Messaging::Definitions::NamespaceOptional& namespaceOptional) {
    bool returnVal = true;
    std::ofstream sourceFile(outputFolder + std::filesystem::path::preferred_separator + structure.getName() + ".cxx");

    sourceFile << "#include \"" << structure.getName() << ".hxx\"" << std::endl;
    sourceFile << std::endl;

    sourceFile << "#include <string.h>" << std::endl;
    sourceFile << std::endl;
    
    if (namespaceOptional.present()) {
        sourceFile << "namespace " << namespaceOptional.get() << " {" << std::endl;
    }

    sourceFile << generateConstructorCxx(structure) << std::endl;

    sourceFile << generateDestructorCxx(structure) << std::endl;

    sourceFile << generateSerialiseCxx(structure) << std::endl;

    sourceFile << generateDeserialiseCxx(structure) << std::endl;
    
    sourceFile << generateGetSizeInBytesCxx(structure) << std::endl;
    
    sourceFile << generateGettersCxx(structure);
    
    sourceFile << generateSettersCxx(structure);
    
    if (namespaceOptional.present()) {
        sourceFile << "}" << std::endl;
    }

    sourceFile.close();
    
    return returnVal;
}

std::string CppGenerator::generateGettersCxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getPrimitive()) {
        os << generateGetterCxx(structure, element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateGetterCxx(structure, enumeration) << std::endl;
    }
    
    for (const auto& subStructure : structure.getStructure()) {
        os << generateGetterCxx(structure, subStructure) << std::endl;
    }
    
    for (const auto& array : structure.getArray()) {
        os << generateGetterCxx(structure, array) << std::endl;
    }
    
    for (const auto& sequence : structure.getSequence()) {
        os << generateGetterCxx(structure, sequence) << std::endl;
    }
    
    for (const auto& map : structure.getMap()) {
        os << generateGetterCxx(structure, map) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateSettersCxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getPrimitive()) {
        os << generateSetterCxx(structure, element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateSetterCxx(structure, enumeration) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedPrimitive& element) {
    std::ostringstream os;
    
    std::string upperName = element.getName();
    upperName[0] = toupper(upperName[0]);

    if ((element.getOptional().present()) && (element.getOptional().get())) {
        os << insertTabs() << "std::optional<" << convertPrimitiveTypeToCppType(element.getType()) << ">& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    } else {
        os << insertTabs() << convertPrimitiveTypeToCppType(element.getType()) << ' ' << structure.getName() << "::get" << upperName << "() const {" << std::endl;
    }
    os << insertTabs(1) << "return " << element.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedStructure& subStructure) {
    std::ostringstream os;
    
    std::string upperName = subStructure.getName();
    upperName[0] = toupper(upperName[0]);

    if ((subStructure.getOptional().present()) && (subStructure.getOptional().get())) {
        os << insertTabs() << "std::optional<" << subStructure.getType() << ">& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    } else {
        os << insertTabs() << subStructure.getType() << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    }
    os << insertTabs(1) << "return " << subStructure.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedArray& array) {
    std::ostringstream os;
    
    std::string upperName = array.getName();
    upperName[0] = toupper(upperName[0]);

    if ((array.getOptional().present()) && (array.getOptional().get())) {
        os << insertTabs() << "std::optional<" << getCxxType(array) << ">& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    } else {
        os << insertTabs() << getCxxType(array) << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    }
    os << insertTabs(1) << "return " << array.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedSequence& sequence) {
    std::ostringstream os;
    
    std::string upperName = sequence.getName();
    upperName[0] = toupper(upperName[0]);

    if ((sequence.getOptional().present()) && (sequence.getOptional().get())) {
        os << insertTabs() << "std::optional<" << getCxxType(sequence) << ">& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    } else {
        os << insertTabs() << getCxxType(sequence) << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    }
    os << insertTabs(1) << "return " << sequence.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedMap& map) {
    std::ostringstream os;
    
    std::string upperName = map.getName();
    upperName[0] = toupper(upperName[0]);

    if ((map.getOptional().present()) && (map.getOptional().get())) {
        os << insertTabs() << "std::optional<" << getCxxType(map) << ">& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    } else {
        os << insertTabs() << getCxxType(map) << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    }
    os << insertTabs(1) << "return " << map.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedPrimitive& element) {
    std::ostringstream os;
    
    std::string upperName = element.getName();
    upperName[0] = toupper(upperName[0]);
    
    os << insertTabs() << "void " << structure.getName() << "::set" << upperName << "(const " << convertPrimitiveTypeToCppType(element.getType()) << " value) {" << std::endl;
    os << insertTabs(1) << element.getName() << " = value;" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedEnumeration& enumeration) {
    std::ostringstream os;
    
    std::string upperName = enumeration.getName();
    upperName[0] = toupper(upperName[0]);

    if ((enumeration.getOptional().present()) && (enumeration.getOptional().get())) {
        os << insertTabs() << "std::optional<" << enumeration.getType() << ">& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    } else {
        os << insertTabs() << enumeration.getType() << ' ' << structure.getName() << "::get" << upperName << "() const {" << std::endl;
    }
    os << insertTabs(1) << "return " << enumeration.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedEnumeration& enumeration) {
    std::ostringstream os;
    
    std::string upperName = enumeration.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << "void " << structure.getName() << "::set" << upperName << "(const " << enumeration.getType() << " value) {" << std::endl;
    os << insertTabs(1) << enumeration.getName() << " = value;" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}
    
std::string CppGenerator::generateConstructorHxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Constructor for this class" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << structure.getName() << "();" << std::endl;
    
    return os.str();
}
    
std::string CppGenerator::generateDestructorHxx(const Messaging::Structure& structure) {
    std::ostringstream os;
        
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Destructor for this class" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << "~" << structure.getName() << "();" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSerialiseHxx() {
    std::ostringstream os;   
    
    os << insertTabs(2) << "void serialise(char* data," << std::endl;
    os << insertTabs(2) << "               uint64_t& offset) const;" << std::endl;
    
    return os.str(); 
}

std::string CppGenerator::generateDeserialiseHxx() {
    std::ostringstream os;   
    
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
    
std::string CppGenerator::generateConstructorCxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    os << insertTabs() << structure.getName() << "::" << structure.getName() << "() {" << std::endl;
    os << std::endl;
    os << insertTabs() << "}" << std::endl;
    
    return os.str();
}
    
std::string CppGenerator::generateDestructorCxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    os << insertTabs() << structure.getName() << "::~" << structure.getName() << "() {" << std::endl;
    os << std::endl;
    os << insertTabs() << "}" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGettersHxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getPrimitive()) {
        os << generateGetterHxx(element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateGetterHxx(enumeration) << std::endl;
    }
    
    for (const auto& subStructure : structure.getStructure()) {
        os << generateGetterHxx(subStructure) << std::endl;
    }
    
    for (const auto& array : structure.getArray()) {
        os << generateGetterHxx(array) << std::endl;
    }
    
    for (const auto& sequence : structure.getSequence()) {
        os << generateGetterHxx(sequence) << std::endl;
    }
    
    for (const auto& map : structure.getMap()) {
        os << generateGetterHxx(map) << std::endl;
    }
    
    // TODO Should we also do setters?
    
    return os.str();
}

std::string CppGenerator::generateSettersHxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getPrimitive()) {
        os << generateSetterHxx(element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateSetterHxx(enumeration) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Messaging::NamedPrimitive& element) {
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
    
    if ((element.getOptional().present()) && (element.getOptional().get())) {
        os << insertTabs(2) << "std::optional<" << convertPrimitiveTypeToCppType(element.getType()) << ">& get" << name << "();" << std::endl;
    } else {
        os << insertTabs(2) << convertPrimitiveTypeToCppType(element.getType()) << " get" << name << "() const;" << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const Messaging::NamedPrimitive& element) {
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
    os << insertTabs(2) << "void set" << name << "(const " << convertPrimitiveTypeToCppType(element.getType()) << " value);" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Messaging::NamedEnumeration& enumeration) {
    std::ostringstream os;
    std::string name = enumeration.getName();
    
    name[0] = toupper(name[0]);
        
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Getter for " << enumeration.getName() << std::endl;

    if (enumeration.getDocumentation().present()) {
        os << insertTabs(2) << " *" << std::endl;
        os << insertTabs(2) << " * " << enumeration.getName() << " defined as : " << enumeration.getDocumentation().get() << std::endl;
    }

    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @return " << enumeration.getName() << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    
    if ((enumeration.getOptional().present()) && (enumeration.getOptional().get())) {
        os << insertTabs(2) << "std::optional<" << enumeration.getType() << ">& get" << name << "();" << std::endl;
    } else {
        os << insertTabs(2) << enumeration.getType() << " get" << name << "() const;" << std::endl;
    }
        
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Messaging::NamedStructure& structure) {
    std::ostringstream os;
    std::string name = structure.getName();
    
    name[0] = toupper(name[0]);
        
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Getter for " << structure.getName() << std::endl;

    if (structure.getDocumentation().present()) {
        os << insertTabs(2) << " *" << std::endl;
        os << insertTabs(2) << " * " << structure.getName() << " defined as : " << structure.getDocumentation().get() << std::endl;
    }

    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @return " << structure.getName() << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    
    if ((structure.getOptional().present()) && (structure.getOptional().get())) {
        os << insertTabs(2) << "std::optional<" << structure.getType() << ">& get" << name << "();" << std::endl;
    } else {
        os << insertTabs(2) << structure.getType() << "& get" << name << "();" << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Messaging::NamedArray& array) {
    std::ostringstream os;
    std::string name = array.getName();
    
    name[0] = toupper(name[0]);
        
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Getter for " << array.getName() << std::endl;

    if (array.getDocumentation().present()) {
        os << insertTabs(2) << " *" << std::endl;
        os << insertTabs(2) << " * " << array.getName() << " defined as : " << array.getDocumentation().get() << std::endl;
    }

    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @return " << array.getName() << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    
    if ((array.getOptional().present()) && (array.getOptional().get())) {
        os << insertTabs(2) << "std::optional<" << getCxxType(array) << ">& get" << name << "();" << std::endl;
    } else {
        os << insertTabs(2) << getCxxType(array) << "& get" << name << "();" << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Messaging::NamedSequence& sequence) {
    std::ostringstream os;
    std::string name = sequence.getName();
    
    name[0] = toupper(name[0]);
        
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Getter for " << sequence.getName() << std::endl;

    if (sequence.getDocumentation().present()) {
        os << insertTabs(2) << " *" << std::endl;
        os << insertTabs(2) << " * " << sequence.getName() << " defined as : " << sequence.getDocumentation().get() << std::endl;
    }

    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @return " << sequence.getName() << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    
    if ((sequence.getOptional().present()) && (sequence.getOptional().get())) {
        os << insertTabs(2) << "std::optional<" << getCxxType(sequence) << ">& get" << name << "();" << std::endl;
    } else {
        os << insertTabs(2) << getCxxType(sequence) << "& get" << name << "();" << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Messaging::NamedMap& map) {
    std::ostringstream os;
    std::string name = map.getName();
    
    name[0] = toupper(name[0]);
        
    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Getter for " << map.getName() << std::endl;

    if (map.getDocumentation().present()) {
        os << insertTabs(2) << " *" << std::endl;
        os << insertTabs(2) << " * " << map.getName() << " defined as : " << map.getDocumentation().get() << std::endl;
    }

    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @return " << map.getName() << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    
    if ((map.getOptional().present()) && (map.getOptional().get())) {
        os << insertTabs(2) << "std::optional<" << getCxxType(map) << ">& get" << name << "();" << std::endl;
    } else {
        os << insertTabs(2) << getCxxType(map) << "& get" << name << "();" << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const Messaging::NamedEnumeration& enumeration) {
    std::ostringstream os;
    std::string name = enumeration.getName();

    name[0] = toupper(name[0]);

    os << insertTabs(2) << "/**" << std::endl;
    os << insertTabs(2) << " * Setter for " << enumeration.getName() << std::endl;

    if (enumeration.getDocumentation().present()) {
        os << insertTabs(2) << " *" << std::endl;
        os << insertTabs(2) << " * " << enumeration.getName() << " defined as : " << enumeration.getDocumentation().get() << std::endl;
    }

    os << insertTabs(2) << " *" << std::endl;
    os << insertTabs(2) << " * @param value The new value to set" << std::endl;
    os << insertTabs(2) << " */" << std::endl;
    os << insertTabs(2) << "void set" << name << "(const " << enumeration.getType() << " value);" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateMembersHxx(const Messaging::Structure& structure) {
    std::ostringstream os;

    for (const auto& element : structure.getPrimitive()) {
        if (element.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << element.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if ((element.getOptional().present()) && (element.getOptional().get())) {
            os << insertTabs(2) << "std::optional<" << convertPrimitiveTypeToCppType(element.getType()) << "> " << element.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << convertPrimitiveTypeToCppType(element.getType()) << ' ' << element.getName() << ';' << std::endl;
        }
        os << std::endl;
    }

    for (const auto& enumeration : structure.getEnumeration()) {
        if (enumeration.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << enumeration.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if ((enumeration.getOptional().present()) && (enumeration.getOptional().get())) {
            os << insertTabs(2) << "std::optional<" << enumeration.getType() << "> " << enumeration.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << enumeration.getType() << ' ' << enumeration.getName() << ';' << std::endl;
        }
        os << std::endl;
    }

    for (const auto& subStructure : structure.getStructure()) {
        if (subStructure.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << subStructure.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if ((subStructure.getOptional().present()) && (subStructure.getOptional().get())) {
            os << insertTabs(2) << "std::optional<" << subStructure.getType() << "> " << subStructure.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << subStructure.getType() << ' ' << subStructure.getName() << ';' << std::endl;
        }
        os << std::endl;
    }

    for (const auto& array : structure.getArray()) {
        if (array.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << array.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if ((array.getOptional().present()) && (array.getOptional().get())) {
            os << insertTabs(2) << "std::optional<" << getCxxType(array) << "> " << array.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << getCxxType(array) << ' ' << array.getName() << ';' << std::endl;
        }
        os << std::endl;
    }

    for (const auto& sequence : structure.getSequence()) {
        if (sequence.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << sequence.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if ((sequence.getOptional().present()) && (sequence.getOptional().get())) {
            os << insertTabs(2) << "std::optional<" << getCxxType(sequence) << "> " << sequence.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << getCxxType(sequence) << ' ' << sequence.getName() << ';' << std::endl;
        }
        os << std::endl;
    }

    for (const auto& map : structure.getMap()) {
        if (map.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << map.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if ((map.getOptional().present()) && (map.getOptional().get())) {
            os << insertTabs(2) << "std::optional<" << getCxxType(map) << "> " << map.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << getCxxType(map) << ' ' << map.getName() << ';' << std::endl;
        }
        os << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateIncludesHxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    os << "#include <cstdint>" << std::endl;
    
    //if (!structure.getArray().empty()) {
        os << "#include <array>" << std::endl;
    //}
    
    //if (!structure.getSequence().empty()) {
        os << "#include <vector>" << std::endl;
    //}
    
    //if (!structure.getMap().empty()) {
        os << "#include <map>" << std::endl;
    //}
    
    os << std::endl;
    
    os << "#include \"BaseMessage.hpp\"" << std::endl;    
    os << std::endl;
    
    std::set<std::string> uniqueHeaders;
    
    for (const auto& enumeration : structure.getEnumeration()) {
        uniqueHeaders.insert(enumeration.getType());
    }
    
    for (const auto& subStructure : structure.getStructure()) {
        uniqueHeaders.insert(subStructure.getType());
    }
    
    for (const auto& array : structure.getArray()) {
        if (array.getType().getEnumeration().present()) {
            uniqueHeaders.insert(array.getType().getEnumeration().get());
        } else if (array.getType().getStructureType().present()) {
            uniqueHeaders.insert(array.getType().getStructureType().get());
        }
    }
    
    for (const auto& header : uniqueHeaders) {
	os << "#include \"" << header << ".hxx\"" << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetSizeInBytesCxx(const Messaging::Structure& structure) {
    std::ostringstream os;   
    uint64_t size = 0;
    
    os << insertTabs(0) << "uint64_t " << structure.getName() << "::getSizeInBytes() const {" << std::endl;
    
    for (const auto& element : structure.getPrimitive()) {
        os << insertTabs(1) << "// " << element.getName() << " = " << sizeOfPrimitiveType(element.getType()) << " bytes (" << convertPrimitiveTypeToCppType(element.getType()) << ')' << std::endl;
        size += sizeOfPrimitiveType(element.getType());
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << insertTabs(1) << "// " << enumeration.getName() << " = " << sizeOfEnumeration(enumeration.getType()) << " bytes (" << enumeration.getType() << ')' << std::endl;
        size += sizeOfEnumeration(enumeration.getType());
    }
    
    os << insertTabs(1) << "// Size of primitive types in this structure" << std::endl;
    os << insertTabs(1) << "uint64_t size = " << size << ';' << std::endl;
    os << std::endl;
       
    for (const auto& subStructure : structure.getStructure()) {
        os << insertTabs(1) << "// Add on size of " << subStructure.getName() << std::endl;
        os << insertTabs(1) << "size += " << subStructure.getName() << ".getSizeInBytes();" << std::endl;
        os << std::endl;
    }
       
    for (const auto& array : structure.getArray()) {
        os << insertTabs(1) << "// Add on size of " << array.getName() << std::endl;
        
        os << generateSizeOfArray(array, 1, array.getName()) << std::endl;
    }
       
    for (const auto& sequence : structure.getSequence()) {
        os << insertTabs(1) << "// Add on size of " << sequence.getName() << std::endl;
        
        os << generateSizeOfSequence(sequence, 1, sequence.getName()) << std::endl;
    }
       
    for (const auto& map : structure.getMap()) {
        os << insertTabs(1) << "// Add on size of " << map.getName() << std::endl;
        
        os << generateSizeOfMap(map, 1, map.getName()) << std::endl;
    }
    
    os << insertTabs(1) << "return size;" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str(); 
}

std::string CppGenerator::generateSizeOfArray(const Messaging::Array& array,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;
    
    if (array.getType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitive().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (array.getType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << array.getType().getEnumeration().get() << ") * " << parentName << ".size();" << std::endl;
    } else if (array.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << array.getType().getStructureType() << std::endl;
        os << insertTabs(numTabs + 1) << "size += e" << +numTabs << ".getSizeInBytes();" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getArray().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfArray(array.getType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfSequence(array.getType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }
    
    return os.str(); 
}
        
std::string CppGenerator::generateSizeOfSequence(const Messaging::Sequence& sequence,
                                                 const uint8_t numTabs,
                                                 const std::string& parentName) {
    std::ostringstream os;
    std::string sensibleName = parentName;
    std::replace(sensibleName.begin(), sensibleName.end(), '.', '_');
    
    os << insertTabs(numTabs) << "// Add on size of " << parentName << " length field" << std::endl;
    os << insertTabs(numTabs) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Add on size of " << parentName << " data" << std::endl;
    
    if (sequence.getType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitive().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (sequence.getType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << sequence.getType().getEnumeration().get() << ") * " << parentName << ".size();" << std::endl;
    } else if (sequence.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << sequence.getType().getStructureType() << std::endl;
        os << insertTabs(numTabs + 1) << "size += e" << +numTabs << ".getSizeInBytes();" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getArray().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfArray(sequence.getType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfSequence(sequence.getType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getMap().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfMap(sequence.getType().getMap().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }
    
    return os.str(); 
}
        
std::string CppGenerator::generateSizeOfMap(const Messaging::Map& map,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;
    std::string sensibleName = parentName;
    std::replace(sensibleName.begin(), sensibleName.end(), '.', '_');
    
    os << insertTabs(numTabs) << "// Add on size of " << parentName << " length field" << std::endl;
    os << insertTabs(numTabs) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Add on size of " << parentName << " key data" << std::endl;
    
    if (map.getKeyType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << convertPrimitiveTypeToCppType(map.getKeyType().getPrimitive().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (map.getKeyType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << map.getKeyType().getEnumeration().get() << ") * " << parentName << ".size();" << std::endl;
    }    
    
    os << std::endl;
    os << insertTabs(numTabs) << "// Add on size of " << parentName << " value data" << std::endl;
    
    if (map.getValueType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << convertPrimitiveTypeToCppType(map.getValueType().getPrimitive().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (map.getValueType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << map.getValueType().getEnumeration().get() << ") * " << parentName << ".size();" << std::endl;
    } else if (map.getValueType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << map.getValueType().getStructureType() << std::endl;
        os << insertTabs(numTabs + 1) << "size += e" << +numTabs << ".second.getSizeInBytes();" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (map.getValueType().getArray().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfArray(map.getValueType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)) + ".second");
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (map.getValueType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfSequence(map.getValueType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)) + ".second");
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (map.getValueType().getMap().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfMap(map.getValueType().getMap().get(), numTabs + 1, ("e" + std::to_string(numTabs)) + ".second");
        os << insertTabs(numTabs) << '}' << std::endl;
    }
    
    return os.str(); 
}

std::string CppGenerator::generateSerialiseCxx(const Messaging::Structure& structure) {
    std::ostringstream os;   
    
    os << insertTabs() << "void " << structure.getName() << "::serialise(char* data, uint64_t& offset) const {" << std::endl;
    
    if ((structure.getPrimitive().empty()) &&
        (structure.getEnumeration().empty()) &&
        (structure.getStructure().empty()) &&
        (structure.getArray().empty()) &&
        (structure.getSequence().empty())) {
        os << insertTabs(1) << "// Nothing to serialise" << std::endl;
    } else {
        for (const auto& element : structure.getPrimitive()) {
            os << generateSerialise(element);
        }
        
        for (const auto& enumeration : structure.getEnumeration()) {
            os << generateSerialise(enumeration);
        }
        
        for (const auto& subStructure : structure.getStructure()) {
            os << generateSerialise(subStructure);
        }
        
        for (const auto& array : structure.getArray()) {
            os << insertTabs(1) << "// Serialise " << array.getName() << std::endl;
            os << generateSerialise(array, 1, array.getName()) << std::endl;
        }
        
        for (const auto& sequence : structure.getSequence()) {
            os << insertTabs(1) << "// Serialise " << sequence.getName() << std::endl;
            os << generateSerialise(sequence, 1, sequence.getName()) << std::endl;
        }
        
        for (const auto& map : structure.getMap()) {
            os << insertTabs(1) << "// Serialise " << map.getName() << std::endl;
            os << generateSerialise(map, 1, map.getName()) << std::endl;
        }
    }
    
    os.seekp(-1, os.cur);
    os << insertTabs() << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialiseCxx(const Messaging::Structure& structure) {
    std::ostringstream os;   
    
    os << insertTabs() << "void " << structure.getName() << "::deserialise(const char* data, uint64_t& offset) {" << std::endl;
    
    if ((structure.getPrimitive().empty()) &&
        (structure.getEnumeration().empty()) &&
        (structure.getStructure().empty()) &&
        (structure.getArray().empty()) &&
        (structure.getSequence().empty())) {
        os << insertTabs(1) << "// Nothing to deserialise" << std::endl;
    } else {
        for (const auto& element : structure.getPrimitive()) {
            os << generateDeserialise(element);
        }
        
        for (const auto& enumeration : structure.getEnumeration()) {
            os << generateDeserialise(enumeration);
        }
        
        for (const auto& subStructure : structure.getStructure()) {
            os << generateDeserialise(subStructure);
        }
        
        for (const auto& array : structure.getArray()) {
            os << insertTabs(1) << "// Deserialise " << array.getName() << std::endl;
            os << generateDeserialise(array, 1, array.getName()) << std::endl;
        }
        
        for (const auto& sequence : structure.getSequence()) {
            os << insertTabs(1) << "// Deserialise " << sequence.getName() << std::endl;
            os << generateDeserialise(sequence, 1, sequence.getName()) << std::endl;
        }
        
        for (const auto& map : structure.getMap()) {
            os << insertTabs(1) << "// Deserialise " << map.getName() << std::endl;
            os << generateDeserialise(map, 1, map.getName()) << std::endl;
        }
    }
    
    os.seekp(-1, os.cur);
    os << insertTabs() << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::NamedPrimitive& element) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << element.getName() << std::endl;
    os << insertTabs(1) << "memcpy(data + offset, &" << element.getName() << ", sizeof(" << convertPrimitiveTypeToCppType(element.getType()) << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << element.getName() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::NamedPrimitive& element) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << element.getName() << std::endl;
    os << insertTabs(1) << "memcpy(&" << element.getName() << ", data + offset, sizeof(" << convertPrimitiveTypeToCppType(element.getType()) << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << element.getName() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::NamedEnumeration& enumeration) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << enumeration.getName() << std::endl;
    os << insertTabs(1) << "memcpy(data + offset, &" << enumeration.getName() << ", sizeof(" << enumeration.getType() << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << enumeration.getType() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::NamedEnumeration& enumeration) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << enumeration.getName() << std::endl;
    os << insertTabs(1) << "memcpy(&" << enumeration.getName() << ", data + offset, sizeof(" << enumeration.getType() << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << enumeration.getType() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::NamedStructure& structure) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << structure.getName() << std::endl;
    os << insertTabs(1) << structure.getName() << ".serialise(data + offset, offset);" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::NamedStructure& structure) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << structure.getName() << std::endl;
    os << insertTabs(1) << structure.getName() << ".deserialise(data + offset, offset);" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::Array& array,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;
    
    if (array.getType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "memcpy(data + offset, &" << parentName << "[0], sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitive().get()) << ") * " << parentName << ".size());" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitive().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (array.getType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "memcpy(data + offset, &" << parentName << "[0], sizeof(" << array.getType().getEnumeration().get() << ") * " << parentName << ".size());" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << array.getType().getEnumeration().get() << ") * " << parentName << ".size();" << std::endl;
    } else if (array.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".serialise(data + offset, offset);" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialise(array.getType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getArray().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialise(array.getType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getMap().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialise(array.getType().getMap().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::Array& array,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;
    
    if (array.getType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "memcpy(&" << parentName << "[0], data + offset, sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitive().get()) << ") * " << parentName << ".size());" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitive().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (array.getType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "memcpy(&" << parentName << "[0], data + offset, sizeof(" << array.getType().getEnumeration().get() << ") * " << parentName << ".size());" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << array.getType().getEnumeration().get() << ") * " << parentName << ".size();" << std::endl;
    } else if (array.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".deserialise(data + offset, offset);" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialise(array.getType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getArray().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialise(array.getType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getMap().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialise(array.getType().getMap().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::Sequence& sequence,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;
    std::string sensibleName = parentName;
    std::replace(sensibleName.begin(), sensibleName.end(), '.', '_');
    
    os << insertTabs(numTabs) << "// Serialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size = " << parentName << ".size();" << std::endl;
    os << insertTabs(numTabs) << "memcpy(data + offset, &" << sensibleName << "Size, sizeof(" << sensibleName << "Size));" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << sensibleName << "Size);" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Serialise " << parentName << " data" << std::endl;
    
    if (sequence.getType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "memcpy(data + offset, &" << parentName << "[0], sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitive().get()) << ") * " << sensibleName << "Size);" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitive().get()) << ") * " << sensibleName << "Size;" << std::endl;
    } else if (sequence.getType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "memcpy(data + offset, &" << parentName << "[0], sizeof(" << sequence.getType().getEnumeration().get() << ") * " << sensibleName << "Size);" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << sequence.getType().getEnumeration().get() << ") * " << sensibleName << "Size;" << std::endl;
    } else if (sequence.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".serialise(data + offset, offset);" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialise(sequence.getType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getArray().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialise(sequence.getType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getMap().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialise(sequence.getType().getMap().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::Sequence& sequence,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;
    std::string sensibleName = parentName;
    std::replace(sensibleName.begin(), sensibleName.end(), '.', '_');
    
    os << insertTabs(numTabs) << "// Deserialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size;" << std::endl;
    os << insertTabs(numTabs) << "memcpy(&" << sensibleName << "Size, data + offset, sizeof(" << sensibleName << "Size));" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << sensibleName << "Size);" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Deserialise " << parentName << " data" << std::endl;
    os << insertTabs(numTabs) << sensibleName << ".resize(" << sensibleName << "Size);" << std::endl;
    
    if (sequence.getType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "memcpy(&" << parentName << "[0], data + offset, sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitive().get()) << ") * " << sensibleName << "Size);" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitive().get()) << ") * " << sensibleName << "Size;" << std::endl;
    } else if (sequence.getType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "memcpy(&" << parentName << "[0], data + offset, sizeof(" << sequence.getType().getEnumeration().get() << ") * " << sensibleName << "Size);" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << sequence.getType().getEnumeration().get() << ") * " << sensibleName << "Size;" << std::endl;
    } else if (sequence.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".deserialise(data + offset, offset);" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialise(sequence.getType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getArray().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialise(sequence.getType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getMap().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialise(sequence.getType().getMap().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::Map& map,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;
    std::string sensibleName = parentName;
    std::replace(sensibleName.begin(), sensibleName.end(), '.', '_');
    
    os << insertTabs(numTabs) << "// Serialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size = " << parentName << ".size();" << std::endl;
    os << insertTabs(numTabs) << "memcpy(data + offset, &" << sensibleName << "Size, sizeof(" << sensibleName << "Size));" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << sensibleName << "Size);" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "for (const auto& e" << +(numTabs - 1) << " : " << parentName << ") {" << std::endl;
    os << insertTabs(numTabs + 1) << "// Serialise " << sensibleName << " key data" << std::endl;
    
    if (map.getKeyType().getPrimitive().present()) {
        os << insertTabs(numTabs + 1) << "memcpy(data + offset, &e" << +(numTabs - 1) << ".first, sizeof(" << convertPrimitiveTypeToCppType(map.getKeyType().getPrimitive().get()) << "));" << std::endl;
        os << insertTabs(numTabs + 1) << "offset += sizeof(" << convertPrimitiveTypeToCppType(map.getKeyType().getPrimitive().get()) << ");" << std::endl;
    } else if (map.getKeyType().getEnumeration().present()) {
        os << insertTabs(numTabs + 1) << "memcpy(data + offset, &e" << +(numTabs - 1) << ".first, sizeof(" << map.getKeyType().getEnumeration().get() << "));" << std::endl;
        os << insertTabs(numTabs + 1) << "offset += sizeof(" << map.getKeyType().getEnumeration().get() << ");" << std::endl;
    }

    os << std::endl;
    os << insertTabs(numTabs + 1) << "// Serialise " << sensibleName << " value data" << std::endl;
    
    if (map.getValueType().getPrimitive().present()) {
        os << insertTabs(numTabs + 1) << "memcpy(data + offset, &e" << +(numTabs - 1) << ".second, sizeof(" << convertPrimitiveTypeToCppType(map.getValueType().getPrimitive().get()) << "));" << std::endl;
        os << insertTabs(numTabs + 1) << "offset += sizeof(" << convertPrimitiveTypeToCppType(map.getValueType().getPrimitive().get()) << ");" << std::endl;
    } else if (map.getValueType().getEnumeration().present()) {
        os << insertTabs(numTabs + 1) << "memcpy(data + offset, &e" << +(numTabs - 1) << ".second, sizeof(" << map.getValueType().getEnumeration().get() << "));" << std::endl;
        os << insertTabs(numTabs + 1) << "offset += sizeof(" << map.getValueType().getEnumeration().get() << ");" << std::endl;
    } else if (map.getValueType().getStructureType().present()) {
        os << insertTabs(numTabs + 1) << 'e' << +(numTabs - 1) << ".second.serialise(data + offset, offset);" << std::endl;
    } else if (map.getValueType().getSequence().present()) {
        os << generateSerialise(map.getValueType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs - 1) + ".second"));
    } else if (map.getValueType().getArray().present()) {
        os << generateSerialise(map.getValueType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs - 1) + ".second"));
    } else if (map.getValueType().getMap().present()) {
        os << generateSerialise(map.getValueType().getMap().get(), numTabs + 1, ("e" + std::to_string(numTabs - 1) + ".second"));
    }
    
    os << insertTabs(numTabs) << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::Map& map,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;
    std::string sensibleName = parentName;
    std::replace(sensibleName.begin(), sensibleName.end(), '.', '_');
    
    os << insertTabs(numTabs) << "// Deserialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size;" << std::endl;
    os << insertTabs(numTabs) << "memcpy(&" << sensibleName << "Size, data + offset, sizeof(" << sensibleName << "Size));" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << sensibleName << "Size);" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Deserialise " << parentName << " data" << std::endl;
    os << insertTabs(numTabs) << "for (" << SEQ_SIZE_TYPE << " index = 0; index < " << sensibleName << "Size; ++index) {" << std::endl;
    os << insertTabs(numTabs + 1) << "// Deserialise " << sensibleName << " key data" << std::endl;
    
    if (map.getKeyType().getPrimitive().present()) {
        os << insertTabs(numTabs + 1) << convertPrimitiveTypeToCppType(map.getKeyType().getPrimitive().get()) << " first" << +(numTabs - 1) << ';' << std::endl;
        os << insertTabs(numTabs + 1) << "memcpy(&first" << +(numTabs - 1) << ", data + offset, sizeof(first" << +(numTabs - 1) << "));" << std::endl;
        os << insertTabs(numTabs + 1) << "offset += sizeof(first" << +(numTabs - 1) << ");" << std::endl;
    } else if (map.getKeyType().getEnumeration().present()) {
        os << insertTabs(numTabs + 1) << map.getKeyType().getEnumeration().get() << " first" << +(numTabs - 1) << ';' << std::endl;
        os << insertTabs(numTabs + 1) << "memcpy(&first" << +(numTabs - 1) << ", data + offset, sizeof(first" << +(numTabs - 1) << "));" << std::endl;
        os << insertTabs(numTabs + 1) << "offset += sizeof(first" << +(numTabs - 1) << ");" << std::endl;
    }

    os << std::endl;
    os << insertTabs(numTabs + 1) << "// Deserialise " << sensibleName << " value data" << std::endl;
    
    if (map.getValueType().getPrimitive().present()) {
        os << insertTabs(numTabs + 1) << convertPrimitiveTypeToCppType(map.getValueType().getPrimitive().get()) << " second" << +(numTabs - 1) << ';' << std::endl;
        os << insertTabs(numTabs + 1) << "memcpy(&second" << +(numTabs - 1) << ", data + offset, sizeof(second" << +(numTabs - 1) << "));" << std::endl;
        os << insertTabs(numTabs + 1) << "offset += sizeof(second" << +(numTabs - 1) << ");" << std::endl;
    } else if (map.getValueType().getEnumeration().present()) {
        os << insertTabs(numTabs + 1) << map.getValueType().getEnumeration().get() << " second" << +(numTabs - 1) << ";" << std::endl;
        os << insertTabs(numTabs + 1) << "memcpy(&second" << +(numTabs - 1) << ", data + offset, sizeof(second" << +(numTabs - 1) << "));" << std::endl;
        os << insertTabs(numTabs + 1) << "offset += sizeof(second" << +(numTabs - 1) << ");" << std::endl;
    } else if (map.getValueType().getStructureType().present()) {
        os << insertTabs(numTabs + 1) << map.getValueType().getStructureType().get() << " second" << +(numTabs - 1) << ';' << std::endl;
        os << insertTabs(numTabs + 1) << "second" << +(numTabs - 1) << ".deserialise(data + offset, offset);" << std::endl;
    } else if (map.getValueType().getSequence().present()) {
        os << insertTabs(numTabs + 1) << getCxxType(map.getValueType().getSequence().get()) << " second" << +(numTabs - 1) << ';' << std::endl;
        os << generateDeserialise(map.getValueType().getSequence().get(), numTabs + 1, ("second" + std::to_string(numTabs - 1)));
    } else if (map.getValueType().getArray().present()) {
        os << insertTabs(numTabs + 1) << getCxxType(map.getValueType().getArray().get()) << " second" << +(numTabs - 1) << ';' << std::endl;
        os << generateDeserialise(map.getValueType().getArray().get(), numTabs + 1, ("second" + std::to_string(numTabs - 1)));
    } else if (map.getValueType().getMap().present()) {
        os << insertTabs(numTabs + 1) << getCxxType(map.getValueType().getMap().get()) << " second" << +(numTabs - 1) << ';' << std::endl;
        os << generateDeserialise(map.getValueType().getMap().get(), numTabs + 1, ("second" + std::to_string(numTabs - 1)));
    }
    
    os << std::endl;
    os << insertTabs(numTabs + 1) << parentName << ".insert({first" << +(numTabs - 1) << ", second" << +(numTabs - 1) << "});" << std::endl;
    
    os << insertTabs(numTabs) << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::getCxxType(const Messaging::Array& array) {
    std::ostringstream os;
    
    os << "std::array<";
    
    if (array.getType().getPrimitive().present()) {
        os << convertPrimitiveTypeToCppType(array.getType().getPrimitive().get());
    } else if (array.getType().getEnumeration().present()) {
        os << array.getType().getEnumeration().get();
    } else if (array.getType().getStructureType().present()) {
        os << array.getType().getStructureType().get();
    } else if (array.getType().getArray().present()) {
        os << getCxxType(array.getType().getArray().get());
    } else if (array.getType().getSequence().present()) {
        os << getCxxType(array.getType().getSequence().get());
    } else if (array.getType().getMap().present()) {
        os << getCxxType(array.getType().getMap().get());
    }
    
    os << ", " << array.getSize() << '>';
    
    return os.str();
}

std::string CppGenerator::getCxxType(const Messaging::Sequence& sequence) {
    std::ostringstream os;
    
    os << "std::vector<";
    
    if (sequence.getType().getPrimitive().present()) {
        os << convertPrimitiveTypeToCppType(sequence.getType().getPrimitive().get());
    } else if (sequence.getType().getEnumeration().present()) {
        os << sequence.getType().getEnumeration().get();
    } else if (sequence.getType().getStructureType().present()) {
        os << sequence.getType().getStructureType().get();
    } else if (sequence.getType().getArray().present()) {
        os << getCxxType(sequence.getType().getArray().get());
    } else if (sequence.getType().getSequence().present()) {
        os << getCxxType(sequence.getType().getSequence().get());
    } else if (sequence.getType().getMap().present()) {
        os << getCxxType(sequence.getType().getMap().get());
    }
    
    os << '>';
    
    return os.str();
}

std::string CppGenerator::getCxxType(const Messaging::Map& map) {
    std::ostringstream os;
    
    os << "std::map<";
    
    if (map.getKeyType().getPrimitive().present()) {
        os << convertPrimitiveTypeToCppType(map.getKeyType().getPrimitive().get());
    } else if (map.getKeyType().getEnumeration().present()) {
        os << map.getKeyType().getEnumeration().get();
    }
    
    os << ", ";
    
    if (map.getValueType().getPrimitive().present()) {
        os << convertPrimitiveTypeToCppType(map.getValueType().getPrimitive().get());
    } else if (map.getValueType().getEnumeration().present()) {
        os << map.getValueType().getEnumeration().get();
    } else if (map.getValueType().getStructureType().present()) {
        os << map.getValueType().getStructureType().get();
    } else if (map.getValueType().getArray().present()) {
        os << getCxxType(map.getValueType().getArray().get());
    } else if (map.getValueType().getSequence().present()) {
        os << getCxxType(map.getValueType().getSequence().get());
    } else if (map.getValueType().getMap().present()) {
        os << getCxxType(map.getValueType().getMap().get());
    }
    
    os << ">";
    
    return os.str();
}

uint64_t CppGenerator::sizeOfPrimitiveType(const Messaging::NamedPrimitive::TypeType& type) {
    uint64_t size = 0;

    if ((type == "signed_int_8") || (type == "unsigned_int_8")) {
        size = 1;
    } else if ((type == "signed_int_16") || (type == "unsigned_int_16")) {
        size = 2;
    } else if ((type == "signed_int_32") || (type == "unsigned_int_32")) {
        size = 4;
    } else if ((type == "signed_int_64") || (type == "unsigned_int_64")) {
        size = 8;
    } else if (type == "float_32") {
        size = 4;
    } else if (type == "float_64") {
        size = 8;
    }

    return size;
}

uint64_t CppGenerator::sizeOfEnumeration(const std::string& enumString) {
    uint64_t size = 0;
    
    for (const auto& enumeration : definitions.getEnumeration()) {
        if (enumString.compare(enumeration.getName()) == 0) {
            if ((enumeration.getBaseType() == "signed_int_8") || (enumeration.getBaseType() == "unsigned_int_8")) {
                size = 1;
            } else if ((enumeration.getBaseType() == "signed_int_16") || (enumeration.getBaseType() == "unsigned_int_16")) {
                size = 2;
            } else if ((enumeration.getBaseType() == "signed_int_32") || (enumeration.getBaseType() == "unsigned_int_32")) {
                size = 4;
            } else if ((enumeration.getBaseType() == "signed_int_64") || (enumeration.getBaseType() == "unsigned_int_64")) {
                size = 8;
            } else {
                std::cerr << "Error : Enumeration (" << enumeration.getName() << ") has an invalid base type (" << enumeration.getBaseType() << ')' << std::endl;
                std::exit(EXIT_FAILURE);
            }

            break;
        }
    }
    
    if (size == 0) {
        std::cerr << "Error : Enumeration has an invalid type (" << enumString << ')' << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    return size;
}
    
std::string CppGenerator::convertEnumBaseTypeToCppType(const Messaging::Enumeration::BaseTypeType& type) {
    std::ostringstream os;

    if (type == "unsigned_int_8") {
        os << "uint8_t";
    } else if (type == "unsigned_int_16") {
        os << "uint16_t";
    } else if (type == "unsigned_int_32") {
        os << "uint32_t";
    } else if (type == "unsigned_int_64") {
        os << "uint64_t";
    } else if (type == "signed_int_8") {
        os << "int8_t";
    } else if (type == "signed_int_16") {
        os << "int16_t";
    } else if (type == "signed_int_32") {
        os << "int32_t";
    } else if (type == "signed_int_64") {
        os << "int64_t";
    } else {
        std::cerr << "Error : Invalid enumeration base type (" << type << ')' << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return os.str();
}
    
std::string CppGenerator::convertPrimitiveTypeToCppType(const Messaging::NamedPrimitive::TypeType& type) {
    std::ostringstream os;

    if (type == "unsigned_int_8") {
        os << "uint8_t";
    } else if (type == "unsigned_int_16") {
        os << "uint16_t";
    } else if (type == "unsigned_int_32") {
        os << "uint32_t";
    } else if (type == "unsigned_int_64") {
        os << "uint64_t";
    } else if (type == "signed_int_8") {
        os << "int8_t";
    } else if (type == "signed_int_16") {
        os << "int16_t";
    } else if (type == "signed_int_32") {
        os << "int32_t";
    } else if (type == "signed_int_64") {
        os << "int64_t";
    } else if (type == "float_32") {
        os << "float";
    } else if (type == "float_64") {
        os << "double";
    } else {
        std::cerr << "Error : PrimitiveType is invalid (" << type << ')' << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return os.str();
}
