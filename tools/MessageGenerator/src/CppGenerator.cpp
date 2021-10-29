#include "CppGenerator.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <filesystem>
#include <set>

CppGenerator::CppGenerator(const Definitions& definitions) : Generator(definitions) {
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

bool CppGenerator::generateSourceFile(const Structure& structure,
                                      const std::string& outputFolder,
                                      const Definitions::NamespaceOptional& namespaceOptional) {
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

std::string CppGenerator::generateGettersCxx(const Structure& structure) {
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
    
    return os.str();
}

std::string CppGenerator::generateSettersCxx(const Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getPrimitiveElement()) {
        os << generateSetterCxx(structure, element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateSetterCxx(structure, enumeration) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Structure& structure,
                                            const PrimitiveElement& element) {
    std::ostringstream os;
    
    std::string upperName = element.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << convertPrimitiveTypeToCppType(element.getType()) << ' ' << structure.getName() << "::get" << upperName << "() const {" << std::endl;
    os << insertTabs(1) << "return " << element.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Structure& structure,
                                            const StructureElement& subStructure) {
    std::ostringstream os;
    
    std::string upperName = subStructure.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << subStructure.getType() << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    os << insertTabs(1) << "return " << subStructure.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Structure& structure,
                                            const NamedArray& array) {
    std::ostringstream os;
    
    std::string upperName = array.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << getArrayType(array) << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    os << insertTabs(1) << "return " << array.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Structure& structure,
                                            const NamedSequence& sequence) {
    std::ostringstream os;
    
    std::string upperName = sequence.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << getSequenceType(sequence) << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    os << insertTabs(1) << "return " << sequence.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterCxx(const Structure& structure,
                                            const PrimitiveElement& element) {
    std::ostringstream os;
    
    std::string upperName = element.getName();
    upperName[0] = toupper(upperName[0]);
    
    os << insertTabs() << "void " << structure.getName() << "::set" << upperName << "(const " << convertPrimitiveTypeToCppType(element.getType()) << " value) {" << std::endl;
    os << insertTabs(1) << element.getName() << " = value;" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Structure& structure,
                                            const EnumerationElement& enumeration) {
    std::ostringstream os;
    
    std::string upperName = enumeration.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << enumeration.getType() << ' ' << structure.getName() << "::get" << upperName << "() const {" << std::endl;
    os << insertTabs(1) << "return " << enumeration.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterCxx(const Structure& structure,
                                            const EnumerationElement& enumeration) {
    std::ostringstream os;
    
    std::string upperName = enumeration.getName();
    upperName[0] = toupper(upperName[0]);

    os << insertTabs() << "void " << structure.getName() << "::set" << upperName << "(const " << enumeration.getType() << " value) {" << std::endl;
    os << insertTabs(1) << enumeration.getName() << " = value;" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
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
    
    // TODO Should we also do setters?
    
    return os.str();
}

std::string CppGenerator::generateSettersHxx(const Structure& structure) {
    std::ostringstream os;
    
    for (const auto& element : structure.getPrimitiveElement()) {
        os << generateSetterHxx(element) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateSetterHxx(enumeration) << std::endl;
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
    
    os << insertTabs(2) << convertPrimitiveTypeToCppType(element.getType()) << " get" << name << "() const;" << std::endl;
    
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
    os << insertTabs(2) << "void set" << name << "(const " << convertPrimitiveTypeToCppType(element.getType()) << " value);" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const EnumerationElement& enumeration) {
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

std::string CppGenerator::generateGetterHxx(const StructureElement& structure) {
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

std::string CppGenerator::generateGetterHxx(const NamedArray& array) {
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
    
    os << insertTabs(2) << getArrayType(array) << "& get" << name << "();" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const NamedSequence& sequence) {
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
    
    os << insertTabs(2) << getSequenceType(sequence) << "& get" << name << "();" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const EnumerationElement& enumeration) {
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

std::string CppGenerator::generateMembersHxx(const Structure& structure) {
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

        os << insertTabs(2) << getArrayType(array) << ' ' << array.getName() << ';' << std::endl;
        os << std::endl;
    }

    for (const auto& sequence : structure.getSequence()) {
        if (sequence.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << sequence.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        os << insertTabs(2) << getSequenceType(sequence) << ' ' << sequence.getName() << ';' << std::endl;
        os << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateIncludesHxx(const Structure& structure) {
    std::ostringstream os;
    
    os << "#include <cstdint>" << std::endl;
    
    if (!structure.getArray().empty()) {
        os << "#include <array>" << std::endl;
    }
    
    if (!structure.getSequence().empty()) {
        os << "#include <vector>" << std::endl;
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
        if (array.getEnumerationType().present()) {
            uniqueHeaders.insert(array.getEnumerationType().get());
        } else if (array.getStructureType().present()) {
            uniqueHeaders.insert(array.getStructureType().get());
        }
    }
    
    for (const auto& header : uniqueHeaders) {
	os << "#include \"" << header << ".hxx\"" << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetSizeInBytesCxx(const Structure& structure) {
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

std::string CppGenerator::generateSizeOfArray(const Array& array,
                                              const uint8_t numTabs,
                                              const std::string parentName) {
    std::ostringstream os;
    uint64_t size;
    
    if (array.getPrimitiveType().present()) {
        size = sizeOfPrimitiveType(array.getPrimitiveType().get());
        os << insertTabs(numTabs) << "// " << size << " bytes (" << convertPrimitiveTypeToCppType(array.getPrimitiveType().get()) << ')';

        for (const auto& dimension : array.getDimension()) {
            size *= dimension;
            os << " * " << dimension;
        }

        os << " = " << size << " bytes" << std::endl;
        os << insertTabs(numTabs) << "size += " << size << ';' << std::endl;
    } else if (array.getEnumerationType().present()) {
        size = sizeOfEnumeration(array.getEnumerationType().get());
        os << insertTabs(numTabs) << "// " << size << " bytes (" << array.getEnumerationType().get() << ')';

        for (const auto& dimension : array.getDimension()) {
            size *= dimension;
            os << " * " << dimension;
        }

        os << " = " << size << " bytes" << std::endl;
        os << insertTabs(numTabs) << "size += " << size << ';' << std::endl;
    } else if (array.getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;

        for (uint64_t index = 1; index < array.getDimension().size(); ++index) {
            os << insertTabs(index + numTabs) << "for (const auto& e" << +(index + numTabs) << " : e" << +(index + numTabs - 1) << ") {" << std::endl;
        }

        os << insertTabs(array.getDimension().size() + numTabs) << "// Add on size of each individual " << array.getStructureType() << std::endl;
        os << insertTabs(array.getDimension().size() + numTabs) << "size += e" << +(array.getDimension().size() + numTabs - 1) << ".getSizeInBytes();" << std::endl;

        for (uint64_t index = array.getDimension().size(); index != 0; --index) {
            os << insertTabs(index + numTabs - 1) << '}' << std::endl;
        }
    } else if (array.getSequence().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;

        for (uint64_t index = 1; index < array.getDimension().size(); ++index) {
            os << insertTabs(index + numTabs) << "for (const auto& e" << +(index + numTabs) << " : e" << +(index + numTabs - 1) << ") {" << std::endl;
        }

        os << generateSizeOfSequence(array.getSequence().get(), array.getDimension().size() + numTabs, ("e" + std::to_string(array.getDimension().size() + numTabs - 1)));
        
        for (uint64_t index = array.getDimension().size(); index != 0; --index) {
            os << insertTabs(index + numTabs - 1) << '}' << std::endl;
        }
    }
    
    return os.str(); 
}
        
std::string CppGenerator::generateSizeOfSequence(const Sequence& sequence,
                                                 const uint8_t numTabs,
                                                 const std::string parentName) {
    std::ostringstream os;
    
    if (sequence.getPrimitiveType().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << convertPrimitiveTypeToCppType(sequence.getPrimitiveType().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (sequence.getEnumerationType().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << sequence.getEnumerationType().get() << ") * " << parentName << ".size();" << std::endl;
    } else if (sequence.getStructureType().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << sequence.getStructureType() << std::endl;
        os << insertTabs(numTabs + 1) << "size += e" << +numTabs << ".getSizeInBytes();" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getArray().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfArray(sequence.getArray().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getSequence1().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSizeOfSequence(sequence.getSequence1().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    }
    
    return os.str(); 
}

std::string CppGenerator::generateSerialiseCxx(const Structure& structure) {
    std::ostringstream os;   
    
    os << insertTabs() << "void " << structure.getName() << "::serialise(char* data, uint64_t& offset) const {" << std::endl;
    
    if (structure.getPrimitiveElement().size() == 0) {
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
            os << generateSerialiseArray(array);
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
            os << generateDeserialisePrimitiveElement(element);
        }
        
        for (const auto& enumeration : structure.getEnumeration()) {
            os << generateDeserialiseEnumeration(enumeration);
        }
        
        for (const auto& subStructure : structure.getStructure1()) {
            os << generateDeserialiseStructure(subStructure);
        }
        
        for (const auto& array : structure.getArray()) {
            os << generateDeserialiseArray(array);
        }
    }
    
    os.seekp(-1, os.cur);
    os << insertTabs() << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialisePrimitiveElement(const PrimitiveElement& element) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << element.getName() << std::endl;
    os << insertTabs(1) << "memcpy(data + offset, &" << element.getName() << ", sizeof(" << convertPrimitiveTypeToCppType(element.getType()) << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << element.getName() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialisePrimitiveElement(const PrimitiveElement& element) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << element.getName() << std::endl;
    os << insertTabs(1) << "memcpy(&" << element.getName() << ", data + offset, sizeof(" << convertPrimitiveTypeToCppType(element.getType()) << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << element.getName() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialiseEnumeration(const EnumerationElement& enumeration) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << enumeration.getName() << std::endl;
    os << insertTabs(1) << "memcpy(data + offset, &" << enumeration.getName() << ", sizeof(" << enumeration.getType() << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << enumeration.getType() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialiseEnumeration(const EnumerationElement& enumeration) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << enumeration.getName() << std::endl;
    os << insertTabs(1) << "memcpy(&" << enumeration.getName() << ", data + offset, sizeof(" << enumeration.getType() << "));" << std::endl;
    os << insertTabs(1) << "offset += sizeof(" << enumeration.getType() << ");" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialiseStructure(const StructureElement& structure) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << structure.getName() << std::endl;
    os << insertTabs(1) << structure.getName() << ".serialise(data + offset, offset);" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialiseStructure(const StructureElement& structure) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << structure.getName() << std::endl;
    os << insertTabs(1) << structure.getName() << ".deserialise(data + offset, offset);" << std::endl;
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialiseArray(const NamedArray& array) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Serialise " << array.getName() << std::endl;
    os << insertTabs(1) << "for (const auto& e1 : " << array.getName() << ") {" << std::endl;
            
    for (uint64_t index = 1; index < array.getDimension().size() - 1; ++index) {
        os << insertTabs(index + 1) << "for (const auto& e" << (index + 1) << " : e" << index << ") {" << std::endl;
    }
    
    if (array.getPrimitiveType().present()) {
        os << insertTabs(array.getDimension().size()) << "memcpy(data + offset, e" << (array.getDimension().size() - 1) << ".data(), sizeof(" << convertPrimitiveTypeToCppType(array.getPrimitiveType().get()) << ") * e" << (array.getDimension().size() - 1) << ".size());" << std::endl;
        os << insertTabs(array.getDimension().size()) << "offset += sizeof(" << convertPrimitiveTypeToCppType(array.getPrimitiveType().get()) << ") * e" << (array.getDimension().size() - 1) << ".size();" << std::endl;
    } else if (array.getEnumerationType().present()) {
        os << insertTabs(array.getDimension().size()) << "memcpy(data + offset, e" << (array.getDimension().size() - 1) << ".data(), sizeof(" << array.getEnumerationType().get() << ") * e" << (array.getDimension().size() - 1) << ".size());" << std::endl;
        os << insertTabs(array.getDimension().size()) << "offset += sizeof(" << array.getEnumerationType().get() << ") * e" << (array.getDimension().size() - 1) << ".size();" << std::endl;
    } else if (array.getStructureType().present()) {
        os << insertTabs(array.getDimension().size()) << "for (const auto& e" << array.getDimension().size() << " : e" << (array.getDimension().size() - 1) << ") {" << std::endl;
        os << insertTabs(array.getDimension().size() + 1) << 'e' << array.getDimension().size() << ".serialise(data + offset, offset);" << std::endl;
        os << insertTabs(array.getDimension().size()) << '}' << std::endl;
    }
    
    for (uint64_t index = array.getDimension().size() - 1; index != 0; --index) {
        os << insertTabs(index) << '}' << std::endl;
    }
    
    os << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialiseArray(const NamedArray& array) {
    std::ostringstream os;   
    
    os << insertTabs(1) << "// Deserialise " << array.getName() << std::endl;
    os << insertTabs(1) << "for (auto& e1 : " << array.getName() << ") {" << std::endl;
            
    for (uint64_t index = 1; index < array.getDimension().size() - 1; ++index) {
        os << insertTabs(index + 1) << "for (auto& e" << (index + 1) << " : e" << index << ") {" << std::endl;
    }
    
    if (array.getPrimitiveType().present()) {
        os << insertTabs(array.getDimension().size()) << "memcpy(e" << (array.getDimension().size() - 1) << ".data(), " << "data + offset, sizeof(" << convertPrimitiveTypeToCppType(array.getPrimitiveType().get()) << ") * e" << (array.getDimension().size() - 1) << ".size());" << std::endl;
        os << insertTabs(array.getDimension().size()) << "offset += sizeof(" << convertPrimitiveTypeToCppType(array.getPrimitiveType().get()) << ") * e" << (array.getDimension().size() - 1) << ".size();" << std::endl;
    } else if (array.getEnumerationType().present()) {
        os << insertTabs(array.getDimension().size()) << "memcpy(e" << (array.getDimension().size() - 1) << ".data(), " << "data + offset, sizeof(" << array.getEnumerationType().get() << ") * e" << (array.getDimension().size() - 1) << ".size());" << std::endl;
        os << insertTabs(array.getDimension().size()) << "offset += sizeof(" << array.getEnumerationType().get() << ") * e" << (array.getDimension().size() - 1) << ".size();" << std::endl;
    } else if (array.getStructureType().present()) {
        os << insertTabs(array.getDimension().size()) << "for (auto& e" << array.getDimension().size() << " : e" << (array.getDimension().size() - 1) << ") {" << std::endl;
        os << insertTabs(array.getDimension().size() + 1) << 'e' << array.getDimension().size() << ".deserialise(data + offset, offset);" << std::endl;
        os << insertTabs(array.getDimension().size()) << '}' << std::endl;
    }
    
    for (uint64_t index = array.getDimension().size() - 1; index != 0; --index) {
        os << insertTabs(index) << '}' << std::endl;
    }
    
    os << std::endl;

    return os.str();
}

std::string CppGenerator::getArrayType(const Array& array) {
    std::ostringstream os;
    
    for (uint64_t index = 0; index < array.getDimension().size(); ++index) {
        os << "std::array<";
    }
    
    if (array.getPrimitiveType().present()) {
        os << convertPrimitiveTypeToCppType(array.getPrimitiveType().get());
    } else if (array.getEnumerationType().present()) {
        os << array.getEnumerationType().get();
    } else if (array.getStructureType().present()) {
        os << array.getStructureType().get();
    } else if (array.getSequence().present()) {
        os << getSequenceType(array.getSequence().get());
    }
    
    for (const auto& dimension : array.getDimension() | std::views::reverse) {
        os << ", " << dimension << '>';
    }
    
    return os.str();
}

std::string CppGenerator::getSequenceType(const Sequence& sequence) {
    std::ostringstream os;
    
    os << "std::vector<";
    
    if (sequence.getPrimitiveType().present()) {
        os << convertPrimitiveTypeToCppType(sequence.getPrimitiveType().get());
    } else if (sequence.getEnumerationType().present()) {
        os << sequence.getEnumerationType().get();
    } else if (sequence.getStructureType().present()) {
        os << sequence.getStructureType().get();
    } else if (sequence.getArray().present()) {
        os << getArrayType(sequence.getArray().get());
    } else if (sequence.getSequence1().present()) {
        os << getSequenceType(sequence.getSequence1().get());
    }
    
    os << '>';
    
    return os.str();
}

uint64_t CppGenerator::sizeOfPrimitiveType(const PrimitiveElement::TypeType& type) {
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
    
std::string CppGenerator::convertEnumBaseTypeToCppType(const Enumeration::BaseTypeType& type) {
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
    
std::string CppGenerator::convertPrimitiveTypeToCppType(const PrimitiveElement::TypeType& type) {
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