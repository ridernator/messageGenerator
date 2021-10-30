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
    
    for (const auto& element : structure.getPrimitiveElement()) {
        os << generateGetterCxx(structure, element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateGetterCxx(structure, enumeration) << std::endl;
    }
    
    for (const auto& subStructure : structure.getStructure1()) {
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
    
    for (const auto& element : structure.getPrimitiveElement()) {
        os << generateSetterCxx(structure, element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateSetterCxx(structure, enumeration) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::PrimitiveElement& element) {
    std::ostringstream os;
    
    std::string upperName = element.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << convertPrimitiveTypeToCppType(element.getType()) << ' ' << structure.getName() << "::get" << upperName << "() const {" << std::endl;
    os << insertTabs(1) << "return " << element.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::StructureElement& subStructure) {
    std::ostringstream os;
    
    std::string upperName = subStructure.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << subStructure.getType() << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    os << insertTabs(1) << "return " << subStructure.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedArray& array) {
    std::ostringstream os;
    
    std::string upperName = array.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << getCxxType(array) << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    os << insertTabs(1) << "return " << array.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedSequence& sequence) {
    std::ostringstream os;
    
    std::string upperName = sequence.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << getCxxType(sequence) << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    os << insertTabs(1) << "return " << sequence.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::NamedMap& map) {
    std::ostringstream os;
    
    std::string upperName = map.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << getCxxType(map) << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    os << insertTabs(1) << "return " << map.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterCxx(const Messaging::Structure& structure,
                                            const Messaging::PrimitiveElement& element) {
    std::ostringstream os;
    
    std::string upperName = element.getName();
    upperName[0] = toupper(upperName[0]);
    
    os << insertTabs() << "void " << structure.getName() << "::set" << upperName << "(const " << convertPrimitiveTypeToCppType(element.getType()) << " value) {" << std::endl;
    os << insertTabs(1) << element.getName() << " = value;" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const Messaging::EnumerationElement& enumeration) {
    std::ostringstream os;
    
    std::string upperName = enumeration.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << enumeration.getType() << ' ' << structure.getName() << "::get" << upperName << "() const {" << std::endl;
    os << insertTabs(1) << "return " << enumeration.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterCxx(const Messaging::Structure& structure,
                                            const Messaging::EnumerationElement& enumeration) {
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
    
    for (const auto& element : structure.getPrimitiveElement()) {
        os << generateGetterHxx(element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateGetterHxx(enumeration) << std::endl;
    }
    
    for (const auto& subStructure : structure.getStructure1()) {
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
    
    for (const auto& element : structure.getPrimitiveElement()) {
        os << generateSetterHxx(element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateSetterHxx(enumeration) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Messaging::PrimitiveElement& element) {
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
    
    os << insertTabs(2) << convertPrimitiveTypeToCppType(element.getType()) << " get" << name << "() const;" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const Messaging::PrimitiveElement& element) {
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

std::string CppGenerator::generateGetterHxx(const Messaging::EnumerationElement& enumeration) {
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
    
    os << insertTabs(2) << enumeration.getType() << " get" << name << "() const;" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const Messaging::StructureElement& structure) {
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
    
    os << insertTabs(2) << structure.getType() << "& get" << name << "();" << std::endl;
    
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
    
    os << insertTabs(2) << getCxxType(array) << "& get" << name << "();" << std::endl;
    
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
    
    os << insertTabs(2) << getCxxType(sequence) << "& get" << name << "();" << std::endl;
    
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
    
    os << insertTabs(2) << getCxxType(map) << "& get" << name << "();" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const Messaging::EnumerationElement& enumeration) {
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

    for (const auto& element : structure.getPrimitiveElement()) {
        if (element.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << element.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        os << insertTabs(2) << convertPrimitiveTypeToCppType(element.getType()) << ' ' << element.getName() << ';' << std::endl;
        os << std::endl;
    }

    for (const auto& enumeration : structure.getEnumeration()) {
        if (enumeration.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << enumeration.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        os << insertTabs(2) << enumeration.getType() << ' ' << enumeration.getName() << ';' << std::endl;
        os << std::endl;
    }

    for (const auto& subStructure : structure.getStructure1()) {
        if (subStructure.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << subStructure.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        os << insertTabs(2) << subStructure.getType() << ' ' << subStructure.getName() << ';' << std::endl;
        os << std::endl;
    }

    for (const auto& array : structure.getArray()) {
        if (array.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << array.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        os << insertTabs(2) << getCxxType(array) << ' ' << array.getName() << ';' << std::endl;
        os << std::endl;
    }

    for (const auto& sequence : structure.getSequence()) {
        if (sequence.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << sequence.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        os << insertTabs(2) << getCxxType(sequence) << ' ' << sequence.getName() << ';' << std::endl;
        os << std::endl;
    }

    for (const auto& map : structure.getMap()) {
        if (map.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << map.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        os << insertTabs(2) << getCxxType(map) << ' ' << map.getName() << ';' << std::endl;
        os << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateIncludesHxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    os << "#include <cstdint>" << std::endl;
    
    if (!structure.getArray().empty()) {
        os << "#include <array>" << std::endl;
    }
    
    if (!structure.getSequence().empty()) {
        os << "#include <vector>" << std::endl;
    }
    
    if (!structure.getMap().empty()) {
        os << "#include <map>" << std::endl;
    }
    
    os << std::endl;
    
    os << "#include \"BaseMessage.hpp\"" << std::endl;    
    os << std::endl;
    
    std::set<std::string> uniqueHeaders;
    
    for (const auto& enumeration : structure.getEnumeration()) {
        uniqueHeaders.insert(enumeration.getType());
    }
    
    for (const auto& subStructure : structure.getStructure1()) {
        uniqueHeaders.insert(subStructure.getType());
    }
    
    for (const auto& array : structure.getArray()) {
        if (array.getType().getEnumerationType().present()) {
            uniqueHeaders.insert(array.getType().getEnumerationType().get());
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
    
    for (const auto& element : structure.getPrimitiveElement()) {
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
       
    for (const auto& subStructure : structure.getStructure1()) {
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
    
    os << insertTabs(1) << "return size;" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str(); 
}

std::string CppGenerator::generateSizeOfArray(const Messaging::Array& array,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;
    uint64_t size;
    uint64_t numDimensions = array.getDimensions().getDimension().size();
    
    if (array.getType().getPrimitiveType().present()) {
        size = sizeOfPrimitiveType(array.getType().getPrimitiveType().get());
        os << insertTabs(numTabs) << "// " << size << " bytes (" << convertPrimitiveTypeToCppType(array.getType().getPrimitiveType().get()) << ')';

        for (const auto& dimension : array.getDimensions().getDimension()) {
            size *= dimension;
            os << " * " << dimension;
        }

        os << " = " << size << " bytes" << std::endl;
        os << insertTabs(numTabs) << "size += " << size << ';' << std::endl;
    } else if (array.getType().getEnumerationType().present()) {
        size = sizeOfEnumeration(array.getType().getEnumerationType().get());
        os << insertTabs(numTabs) << "// " << size << " bytes (" << array.getType().getEnumerationType().get() << ')';

        for (const auto& dimension : array.getDimensions().getDimension()) {
            size *= dimension;
            os << " * " << dimension;
        }

        os << " = " << size << " bytes" << std::endl;
        os << insertTabs(numTabs) << "size += " << size << ';' << std::endl;
    } else if (array.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;

        for (uint64_t index = 1; index < numDimensions; ++index) {
            os << insertTabs(index + numTabs) << "for (const auto& e" << +(index + numTabs) << " : e" << +(index + numTabs - 1) << ") {" << std::endl;
        }

        os << insertTabs(numDimensions + numTabs) << "// Add on size of each individual " << array.getType().getStructureType() << std::endl;
        os << insertTabs(numDimensions + numTabs) << "size += e" << +(numDimensions + numTabs - 1) << ".getSizeInBytes();" << std::endl;

        for (uint64_t index = numDimensions; index != 0; --index) {
            os << insertTabs(index + numTabs - 1) << '}' << std::endl;
        }
    } else if (array.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;

        for (uint64_t index = 1; index < numDimensions; ++index) {
            os << insertTabs(index + numTabs) << "for (const auto& e" << +(index + numTabs) << " : e" << +(index + numTabs - 1) << ") {" << std::endl;
        }

        os << generateSizeOfSequence(array.getType().getSequence().get(), numDimensions + numTabs, ("e" + std::to_string(numDimensions + numTabs - 1)));
        
        for (uint64_t index = numDimensions; index != 0; --index) {
            os << insertTabs(index + numTabs - 1) << '}' << std::endl;
        }
    }
    
    return os.str(); 
}
        
std::string CppGenerator::generateSizeOfSequence(const Messaging::Sequence& sequence,
                                                 const uint8_t numTabs,
                                                 const std::string& parentName) {
    std::ostringstream os;
    
    os << insertTabs(numTabs) << "// Add on size of " << parentName << " length field" << std::endl;
    os << insertTabs(numTabs) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Add on size of " << parentName << " data" << std::endl;
    
    if (sequence.getType().getPrimitiveType().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitiveType().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (sequence.getType().getEnumerationType().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << sequence.getType().getEnumerationType().get() << ") * " << parentName << ".size();" << std::endl;
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
    }
    
    return os.str(); 
}

std::string CppGenerator::generateSerialiseCxx(const Messaging::Structure& structure) {
    std::ostringstream os;   
    
    os << insertTabs() << "void " << structure.getName() << "::serialise(char* data, uint64_t& offset) const {" << std::endl;
    
    if ((structure.getPrimitiveElement().empty()) &&
        (structure.getEnumeration().empty()) &&
        (structure.getStructure1().empty()) &&
        (structure.getArray().empty()) &&
        (structure.getSequence().empty())) {
        os << insertTabs(1) << "// Nothing to serialise" << std::endl;
    } else {
        for (const auto& element : structure.getPrimitiveElement()) {
            os << generateSerialisePrimitiveElement(element);
        }
        
        for (const auto& enumeration : structure.getEnumeration()) {
            os << generateSerialiseEnumeration(enumeration);
        }
        
        for (const auto& subStructure : structure.getStructure1()) {
            os << generateSerialiseStructure(subStructure);
        }
        
        for (const auto& array : structure.getArray()) {
            os << insertTabs(1) << "// Serialise " << array.getName() << std::endl;
            os << generateSerialiseArray(array, 1, array.getName()) << std::endl;
        }
        
        for (const auto& sequence : structure.getSequence()) {
            os << insertTabs(1) << "// Serialise " << sequence.getName() << std::endl;
            os << generateSerialiseSequence(sequence, 1, sequence.getName()) << std::endl;
        }
    }
    
    os.seekp(-1, os.cur);
    os << insertTabs() << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialiseCxx(const Messaging::Structure& structure) {
    std::ostringstream os;   
    
    os << insertTabs() << "void " << structure.getName() << "::deserialise(const char* data, uint64_t& offset) {" << std::endl;
    
    if ((structure.getPrimitiveElement().empty()) &&
        (structure.getEnumeration().empty()) &&
        (structure.getStructure1().empty()) &&
        (structure.getArray().empty()) &&
        (structure.getSequence().empty())) {
        os << insertTabs(1) << "// Nothing to deserialise" << std::endl;
    } else {
        for (const auto& element : structure.getPrimitiveElement()) {
            os << generateDeserialisePrimitiveElement(element);
        }
        
        for (const auto& enumeration : structure.getEnumeration()) {
            os << generateDeserialiseEnumeration(enumeration);
        }
        
        for (const auto& subStructure : structure.getStructure1()) {
            os << generateDeserialiseStructure(subStructure);
        }
        
        for (const auto& array : structure.getArray()) {
            os << insertTabs(1) << "// Deserialise " << array.getName() << std::endl;
            os << generateDeserialiseArray(array, 1, array.getName()) << std::endl;
        }
        
        for (const auto& sequence : structure.getSequence()) {
            os << insertTabs(1) << "// Deserialise " << sequence.getName() << std::endl;
            os << generateDeserialiseSequence(sequence, 1, sequence.getName()) << std::endl;
        }
    }
    
    os.seekp(-1, os.cur);
    os << insertTabs() << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialisePrimitiveElement(const Messaging::PrimitiveElement& element) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << element.getName() << std::endl;
    os << insertTabs(1) << "memcpy(data + offset, &" << element.getName() << ", sizeof(" << convertPrimitiveTypeToCppType(element.getType()) << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << element.getName() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialisePrimitiveElement(const Messaging::PrimitiveElement& element) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << element.getName() << std::endl;
    os << insertTabs(1) << "memcpy(&" << element.getName() << ", data + offset, sizeof(" << convertPrimitiveTypeToCppType(element.getType()) << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << element.getName() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialiseEnumeration(const Messaging::EnumerationElement& enumeration) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << enumeration.getName() << std::endl;
    os << insertTabs(1) << "memcpy(data + offset, &" << enumeration.getName() << ", sizeof(" << enumeration.getType() << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << enumeration.getType() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialiseEnumeration(const Messaging::EnumerationElement& enumeration) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << enumeration.getName() << std::endl;
    os << insertTabs(1) << "memcpy(&" << enumeration.getName() << ", data + offset, sizeof(" << enumeration.getType() << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << enumeration.getType() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialiseStructure(const Messaging::StructureElement& structure) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << structure.getName() << std::endl;
    os << insertTabs(1) << structure.getName() << ".serialise(data + offset, offset);" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialiseStructure(const Messaging::StructureElement& structure) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << structure.getName() << std::endl;
    os << insertTabs(1) << structure.getName() << ".deserialise(data + offset, offset);" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialiseArray(const Messaging::Array& array,
                                                 const uint8_t numTabs,
                                                 const std::string& parentName) {
    std::ostringstream os;
    uint64_t numDimensions = array.getDimensions().getDimension().size();
    
    os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
            
    for (uint64_t index = 1; index < numDimensions - 1; ++index) {
        os << insertTabs(index + numTabs) << "for (const auto& e" << +(index + numTabs) << " : e" << +(index + numTabs - 1) << ") {" << std::endl;
    }
    
    if (array.getType().getPrimitiveType().present()) {
        os << insertTabs(numDimensions + numTabs - 1) << "memcpy(data + offset, e" << +(numDimensions + numTabs - 2) << ".data(), sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitiveType().get()) << ") * e" << +(numDimensions + numTabs - 2) << ".size());" << std::endl;
        os << insertTabs(numDimensions + numTabs - 1) << "offset += sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitiveType().get()) << ") * e" << +(numDimensions + numTabs - 2) << ".size();" << std::endl;
    } else if (array.getType().getEnumerationType().present()) {
        os << insertTabs(numDimensions + numTabs - 1) << "memcpy(data + offset, e" << +(numDimensions + numTabs - 2) << ".data(), sizeof(" << array.getType().getEnumerationType().get() << ") * e" << +(numDimensions + numTabs - 2) << ".size());" << std::endl;
        os << insertTabs(numDimensions + numTabs - 1) << "offset += sizeof(" << array.getType().getEnumerationType().get() << ") * e" << +(numDimensions + numTabs - 2) << ".size();" << std::endl;
    } else if (array.getType().getStructureType().present()) {
        os << insertTabs(numDimensions + numTabs - 1) << "for (const auto& e" << +(numDimensions + numTabs - 1) << " : e" << +(numDimensions + numTabs - 2) << ") {" << std::endl;
        os << insertTabs(numDimensions + numTabs) << 'e' << +(numDimensions + numTabs - 1) << ".serialise(data + offset, offset);" << std::endl;
        os << insertTabs(numDimensions + numTabs - 1) << '}' << std::endl;
    } else if (array.getType().getSequence().present()) {
        os << insertTabs(numDimensions + numTabs - 1) << "for (const auto& e" << +(numDimensions + numTabs - 1) << " : e" << +(numDimensions + numTabs - 2) << ") {" << std::endl;
        os << generateSerialiseSequence(array.getType().getSequence().get(), numDimensions + numTabs, ("e" + std::to_string(numDimensions + numTabs - 1)));
        os << insertTabs(numDimensions + numTabs - 1) << '}' << std::endl;
    }
    
    for (uint64_t index = numDimensions - 1; index != 0; --index) {
        os << insertTabs(index + numTabs - 1) << '}' << std::endl;
    }

    return os.str();
}

std::string CppGenerator::generateDeserialiseArray(const Messaging::Array& array,
                                                   const uint8_t numTabs,
                                                   const std::string& parentName) {
    std::ostringstream os;
    uint64_t numDimensions = array.getDimensions().getDimension().size();
    
    os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
            
    for (uint64_t index = 1; index < numDimensions - 1; ++index) {
        os << insertTabs(index + numTabs) << "for (auto& e" << +(index + numTabs) << " : e" << +(index + numTabs - 1) << ") {" << std::endl;
    }
    
    if (array.getType().getPrimitiveType().present()) {
        os << insertTabs(numDimensions + numTabs - 1) << "memcpy(e" << +(numDimensions + numTabs - 2) << ".data(), " << "data + offset, sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitiveType().get()) << ") * e" << +(numDimensions + numTabs - 2) << ".size());" << std::endl;
        os << insertTabs(numDimensions + numTabs - 1) << "offset += sizeof(" << convertPrimitiveTypeToCppType(array.getType().getPrimitiveType().get()) << ") * e" << +(numDimensions + numTabs - 2) << ".size();" << std::endl;
    } else if (array.getType().getEnumerationType().present()) {
        os << insertTabs(numDimensions + numTabs - 1) << "memcpy(e" << +(numDimensions + numTabs - 2) << ".data(), " << "data + offset, sizeof(" << array.getType().getEnumerationType().get() << ") * e" << +(numDimensions + numTabs - 2) << ".size());" << std::endl;
        os << insertTabs(numDimensions + numTabs - 1) << "offset += sizeof(" << array.getType().getEnumerationType().get() << ") * e" << +(numDimensions + numTabs - 2) << ".size();" << std::endl;
    } else if (array.getType().getStructureType().present()) {
        os << insertTabs(numDimensions + numTabs - 1) << "for (auto& e" << +(numDimensions + numTabs - 1) << " : e" << +(numDimensions + numTabs - 2) << ") {" << std::endl;
        os << insertTabs(numDimensions + numTabs) << 'e' << +(numDimensions + numTabs - 1) << ".deserialise(data + offset, offset);" << std::endl;
        os << insertTabs(numDimensions + numTabs - 1) << '}' << std::endl;
    } else if (array.getType().getSequence().present()) {
        os << insertTabs(numDimensions + numTabs - 1) << "for (auto& e" << +(numDimensions + numTabs - 1) << " : e" << +(numDimensions + numTabs - 2) << ") {" << std::endl;
        os << generateDeserialiseSequence(array.getType().getSequence().get(), numDimensions + numTabs, ("e" + std::to_string(numDimensions + numTabs - 1)));
        os << insertTabs(numDimensions + numTabs - 1) << '}' << std::endl;
    }
    
    for (uint64_t index = numDimensions - 1; index != 0; --index) {
        os << insertTabs(index + numTabs - 1) << '}' << std::endl;
    }

    return os.str();
}

std::string CppGenerator::generateSerialiseSequence(const Messaging::Sequence& sequence,
                                                    const uint8_t numTabs,
                                                    const std::string& parentName) {
    std::ostringstream os;
    
    os << insertTabs(numTabs) << "// Serialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << parentName << "Size = " << parentName << ".size();" << std::endl;
    os << insertTabs(numTabs) << "memcpy(data + offset, &" << parentName << "Size, sizeof(" << parentName << "Size));" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << parentName << "Size);" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Serialise " << parentName << " data" << std::endl;
    
    if (sequence.getType().getPrimitiveType().present()) {
        os << insertTabs(numTabs) << "memcpy(data + offset, &" << parentName << "[0], sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitiveType().get()) << ") * " << parentName << "Size);" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitiveType().get()) << ") * " << parentName << "Size;" << std::endl;
    } else if (sequence.getType().getEnumerationType().present()) {
        os << insertTabs(numTabs) << "memcpy(data + offset, &" << parentName << "[0], sizeof(" << sequence.getType().getEnumerationType().get() << ") * " << parentName << "Size);" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << sequence.getType().getEnumerationType().get() << ") * " << parentName << "Size;" << std::endl;
    } else if (sequence.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".serialise(data + offset, offset);" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialiseSequence(sequence.getType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getArray().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialiseArray(sequence.getType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }

    return os.str();
}

std::string CppGenerator::generateDeserialiseSequence(const Messaging::Sequence& sequence,
                                                      const uint8_t numTabs,
                                                      const std::string& parentName) {
    std::ostringstream os;
    
    os << insertTabs(numTabs) << "// Deserialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << parentName << "Size;" << std::endl;
    os << insertTabs(numTabs) << "memcpy(&" << parentName << "Size, data + offset, sizeof(" << parentName << "Size));" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << parentName << "Size);" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Deserialise " << parentName << " data" << std::endl;
    os << insertTabs(numTabs) << parentName << ".resize(" << parentName << "Size);" << std::endl;
    
    if (sequence.getType().getPrimitiveType().present()) {
        os << insertTabs(numTabs) << "memcpy(&" << parentName << "[0], data + offset, sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitiveType().get()) << ") * " << parentName << "Size);" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << convertPrimitiveTypeToCppType(sequence.getType().getPrimitiveType().get()) << ") * " << parentName << "Size;" << std::endl;
    } else if (sequence.getType().getEnumerationType().present()) {
        os << insertTabs(numTabs) << "memcpy(&" << parentName << "[0], data + offset, sizeof(" << sequence.getType().getEnumerationType().get() << ") * " << parentName << "Size);" << std::endl;
        os << insertTabs(numTabs) << "offset += sizeof(" << sequence.getType().getEnumerationType().get() << ") * " << parentName << "Size;" << std::endl;
    } else if (sequence.getType().getStructureType().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".deserialise(data + offset, offset);" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getSequence().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialiseSequence(sequence.getType().getSequence().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getArray().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialiseArray(sequence.getType().getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }

    return os.str();
}

std::string CppGenerator::getCxxType(const Messaging::Array& array) {
    std::ostringstream os;
    
    for (uint64_t index = 0; index < array.getDimensions().getDimension().size(); ++index) {
        os << "std::array<";
    }
    
    if (array.getType().getPrimitiveType().present()) {
        os << convertPrimitiveTypeToCppType(array.getType().getPrimitiveType().get());
    } else if (array.getType().getEnumerationType().present()) {
        os << array.getType().getEnumerationType().get();
    } else if (array.getType().getStructureType().present()) {
        os << array.getType().getStructureType().get();
    } else if (array.getType().getSequence().present()) {
        os << getCxxType(array.getType().getSequence().get());
    } else if (array.getType().getMap().present()) {
        os << getCxxType(array.getType().getMap().get());
    }
    
    for (const auto& dimension : array.getDimensions().getDimension() | std::views::reverse) {
        os << ", " << dimension << '>';
    }
    
    return os.str();
}

std::string CppGenerator::getCxxType(const Messaging::Sequence& sequence) {
    std::ostringstream os;
    
    os << "std::vector<";
    
    if (sequence.getType().getPrimitiveType().present()) {
        os << convertPrimitiveTypeToCppType(sequence.getType().getPrimitiveType().get());
    } else if (sequence.getType().getEnumerationType().present()) {
        os << sequence.getType().getEnumerationType().get();
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
    
    if (map.getKeyType().getPrimitiveType().present()) {
        os << convertPrimitiveTypeToCppType(map.getKeyType().getPrimitiveType().get());
    } else if (map.getKeyType().getEnumerationType().present()) {
        os << map.getKeyType().getEnumerationType().get();
    } else if (map.getKeyType().getStructureType().present()) {
        os << map.getKeyType().getStructureType().get();
    }
    
    os << ", ";
    
    if (map.getValueType().getPrimitiveType().present()) {
        os << convertPrimitiveTypeToCppType(map.getValueType().getPrimitiveType().get());
    } else if (map.getValueType().getEnumerationType().present()) {
        os << map.getValueType().getEnumerationType().get();
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

uint64_t CppGenerator::sizeOfPrimitiveType(const Messaging::PrimitiveElement::TypeType& type) {
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
    
std::string CppGenerator::convertPrimitiveTypeToCppType(const Messaging::PrimitiveElement::TypeType& type) {
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
