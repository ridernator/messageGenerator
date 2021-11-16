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
    
    for (const auto& primitive : structure.getPrimitive()) {
        os << generateGetterCxx(structure, primitive, convertPrimitiveTypeToCppType(primitive.getType()), false) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateGetterCxx(structure, enumeration, enumeration.getType(), false) << std::endl;
    }
    
    for (const auto& string : structure.getString()) {
        os << generateGetterCxx(structure, string, getCxxType(string)) << std::endl;
    }
    
    for (const auto& subStructure : structure.getStructure()) {
        os << generateGetterCxx(structure, subStructure, subStructure.getType()) << std::endl;
    }
    
    for (const auto& array : structure.getArray()) {
        os << generateGetterCxx(structure, array, getCxxType(array)) << std::endl;
    }
    
    for (const auto& sequence : structure.getSequence()) {
        os << generateGetterCxx(structure, sequence, getCxxType(sequence)) << std::endl;
    }
    
    for (const auto& map : structure.getMap()) {
        os << generateGetterCxx(structure, map, getCxxType(map)) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateSettersCxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    for (const auto& primitive : structure.getPrimitive()) {
        os << generateSetterCxx(structure, primitive, convertPrimitiveTypeToCppType(primitive.getType())) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateSetterCxx(structure, enumeration, enumeration.getType()) << std::endl;
    }
    
    for (const auto& string : structure.getString()) {
        os << generateSetterCxx(structure, string, getCxxType(string), true) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateSetterCxx(const Messaging::Structure& structure,
                                            const auto& element,
                                            const std::string& type,
                                            const bool passAsReference) {
    std::ostringstream os;
    
    std::string upperName = element.getName();
    upperName[0] = toupper(upperName[0]);
    
    if (passAsReference) {
        os << insertTabs() << "void " << structure.getName() << "::set" << upperName << "(const " << type << "& value) {" << std::endl;
    } else {
        os << insertTabs() << "void " << structure.getName() << "::set" << upperName << "(const " << type << " value) {" << std::endl;
    }
    
    os << insertTabs(1) << element.getName() << " = value;" << std::endl;
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
    
    for (const auto& primitive : structure.getPrimitive()) {
        os << generateGetterHxx(primitive, convertPrimitiveTypeToCppType(primitive.getType()), false) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateGetterHxx(enumeration, enumeration.getType(), false) << std::endl;
    }
    
    for (const auto& string : structure.getString()) {
        os << generateGetterHxx(string, getCxxType(string)) << std::endl;
    }
    
    for (const auto& subStructure : structure.getStructure()) {
        os << generateGetterHxx(subStructure, subStructure.getType()) << std::endl;
    }
    
    for (const auto& array : structure.getArray()) {
        os << generateGetterHxx(array, getCxxType(array)) << std::endl;
    }
    
    for (const auto& sequence : structure.getSequence()) {
        os << generateGetterHxx(sequence, getCxxType(sequence)) << std::endl;
    }
    
    for (const auto& map : structure.getMap()) {
        os << generateGetterHxx(map, getCxxType(map)) << std::endl;
    }
    
    // TODO Should we also do setters?
    
    return os.str();
}

std::string CppGenerator::generateSettersHxx(const Messaging::Structure& structure) {
    std::ostringstream os;
    
    for (const auto& primitive : structure.getPrimitive()) {
        os << generateSetterHxx(primitive, convertPrimitiveTypeToCppType(primitive.getType())) << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << generateSetterHxx(enumeration, enumeration.getType()) << std::endl;
    }
    
    for (const auto& string : structure.getString()) {
        os << generateSetterHxx(string, getCxxType(string), true) << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateSetterHxx(const auto& element,
                                            const std::string& type,
                                            const bool passAsReference) {
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
    
    if (passAsReference) {
        os << insertTabs(2) << "void set" << name << "(const " << type << "& value);" << std::endl;
    } else {
        os << insertTabs(2) << "void set" << name << "(const " << type << " value);" << std::endl;
    }
    
    return os.str();
}

std::string CppGenerator::generateGetterCxx(const Messaging::Structure& structure,
                                            const auto& element,
                                            const std::string& type,
                                            const bool returnReference) {
    std::ostringstream os;
    
    std::string upperName = element.getName();
    upperName[0] = toupper(upperName[0]);

    if (isOptional(element)) {
        os << insertTabs() << "std::optional<" << type << ">& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
    } else {
        if (returnReference) {
            os << insertTabs() << type << "& " << structure.getName() << "::get" << upperName << "() {" << std::endl;
        } else {
            os << insertTabs() << type << ' ' << structure.getName() << "::get" << upperName << "() const {" << std::endl;
        }
    }
    os << insertTabs(1) << "return " << element.getName() << ";" << std::endl;
    os << insertTabs() << '}' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateGetterHxx(const auto& element,
                                            const std::string& type,
                                            const bool returnReference) {
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
    
    if (isOptional(element)) {
        os << insertTabs(2) << "std::optional<" << type << ">& get" << name << "();" << std::endl;
    } else {
        if (returnReference) {
            os << insertTabs(2) << type << '&' << " get" << name << "();" << std::endl;
        } else {
            os << insertTabs(2) << type << " get" << name << "() const;" << std::endl;
        }
    }
        
    return os.str();
}

std::string CppGenerator::generateMembersHxx(const Messaging::Structure& structure) {
    std::ostringstream os;

    for (const auto& primitive : structure.getPrimitive()) {
        if (primitive.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << primitive.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if (isOptional(primitive)) {
            os << insertTabs(2) << "std::optional<" << convertPrimitiveTypeToCppType(primitive.getType()) << "> " << primitive.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << convertPrimitiveTypeToCppType(primitive.getType()) << ' ' << primitive.getName() << ';' << std::endl;
        }
        os << std::endl;
    }

    for (const auto& enumeration : structure.getEnumeration()) {
        if (enumeration.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << enumeration.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if (isOptional(enumeration)) {
            os << insertTabs(2) << "std::optional<" << enumeration.getType() << "> " << enumeration.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << enumeration.getType() << ' ' << enumeration.getName() << ';' << std::endl;
        }
        os << std::endl;
    }

    for (const auto& string : structure.getString()) {
        if (string.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << string.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if (isOptional(string)) {
            os << insertTabs(2) << "std::optional<" << getCxxType(string) << "> " << string.getName() << ';' << std::endl;
        } else {
            os << insertTabs(2) << getCxxType(string) << ' ' << string.getName() << ';' << std::endl;
        }
        os << std::endl;
    }

    for (const auto& subStructure : structure.getStructure()) {
        if (subStructure.getDocumentation().present()) {
            os << insertTabs(2) << "/**" << std::endl;
            os << insertTabs(2) << " * " << subStructure.getDocumentation().get() << std::endl;
            os << insertTabs(2) << " */" << std::endl;
        }

        if (isOptional(subStructure)) {
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

        if (isOptional(array)) {
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

        if (isOptional(sequence)) {
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

        if (isOptional(map)) {
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
    
    std::set<std::string> uniqueHeaders;

    for (const auto& primitive : structure.getPrimitive()) {
        if (isOptional(primitive)) {
            uniqueHeaders.insert("#include <optional>");
        }

        uniqueHeaders.insert("#include <cstdint>");
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        if (isOptional(enumeration)) {
            uniqueHeaders.insert("#include <optional>");
        }

        uniqueHeaders.insert("#include \"" + enumeration.getType() + ".hxx\"");
    }
    
    for (const auto& string : structure.getString()) {
        if (isOptional(string)) {
            uniqueHeaders.insert("#include <optional>");
        }

        uniqueHeaders.insert("#include <string>");
    }
    
    for (const auto& subStructure : structure.getStructure()) {
        if (isOptional(subStructure)) {
            uniqueHeaders.insert("#include <optional>");
        }

        uniqueHeaders.insert("#include \"" + subStructure.getType() + ".hxx\"");
    }
    
    for (const auto& array : structure.getArray()) {
        if (isOptional(array)) {
            uniqueHeaders.insert("#include <optional>");
        }

        addRequiredHeaders(array, uniqueHeaders);
    }
    
    for (const auto& sequence : structure.getSequence()) {
        if (isOptional(sequence)) {
            uniqueHeaders.insert("#include <optional>");
        }

        addRequiredHeaders(sequence, uniqueHeaders);
    }
    
    for (const auto& map : structure.getMap()) {
        if (isOptional(map)) {
            uniqueHeaders.insert("#include <optional>");
        }

        addRequiredHeaders(map, uniqueHeaders);
    }
    
    for (const auto& header : uniqueHeaders) {
        if (header.find('<') != std::string::npos) {
            os << header << std::endl;
        }
    }    
    os << std::endl;

    os << "#include \"BaseMessage.hpp\"" << std::endl;  
    for (const auto& header : uniqueHeaders) {
        if (header.find('<') == std::string::npos) {
            os << header << std::endl;
        }
    }
    
    return os.str();
}

void CppGenerator::addRequiredHeaders(const Messaging::Array array,
                                      std::set<std::string>& headers) {
    headers.insert("#include <array>");

    if (array.getType().getPrimitive().present()) {
        headers.insert("#include <cstdint>");
    } else if (array.getType().getEnumeration().present()) {
        headers.insert("#include \"" + array.getType().getEnumeration().get() + ".hxx\"");
    } else if (array.getType().getString().present()) {
        headers.insert("#include <string>");
    } else if (array.getType().getStructure().present()) {
        headers.insert("#include \"" + array.getType().getStructure().get() + ".hxx\"");
    } else if (array.getType().getArray().present()) {
        addRequiredHeaders(array.getType().getArray().get(), headers);
    } else if (array.getType().getSequence().present()) {
        addRequiredHeaders(array.getType().getSequence().get(), headers);
    } else if (array.getType().getMap().present()) {
        addRequiredHeaders(array.getType().getMap().get(), headers);
    }
}

void CppGenerator::addRequiredHeaders(const Messaging::Sequence sequence,
                                      std::set<std::string>& headers) {
    headers.insert("#include <vector>");

    if (sequence.getType().getPrimitive().present()) {
        headers.insert("#include <cstdint>");
    } else if (sequence.getType().getEnumeration().present()) {
        headers.insert("#include \"" + sequence.getType().getEnumeration().get() + ".hxx\"");
    } else if (sequence.getType().getString().present()) {
        headers.insert("#include <string>");
    } else if (sequence.getType().getStructure().present()) {
        headers.insert("#include \"" + sequence.getType().getStructure().get() + ".hxx\"");
    } else if (sequence.getType().getArray().present()) {
        addRequiredHeaders(sequence.getType().getArray().get(), headers);
    } else if (sequence.getType().getSequence().present()) {
        addRequiredHeaders(sequence.getType().getSequence().get(), headers);
    } else if (sequence.getType().getMap().present()) {
        addRequiredHeaders(sequence.getType().getMap().get(), headers);
    }
}

void CppGenerator::addRequiredHeaders(const Messaging::Map map,
                                      std::set<std::string>& headers) {
    headers.insert("#include <map>");

    if (map.getKeyType().getPrimitive().present()) {
        headers.insert("#include <cstdint>");
    } else if (map.getKeyType().getEnumeration().present()) {
        headers.insert("#include \"" + map.getKeyType().getEnumeration().get() + ".hxx\"");
    } else if (map.getKeyType().getString().present()) {
        headers.insert("#include <string>");
    }

    if (map.getValueType().getPrimitive().present()) {
        headers.insert("#include <cstdint>");
    } else if (map.getValueType().getEnumeration().present()) {
        headers.insert("#include \"" + map.getValueType().getEnumeration().get() + ".hxx\"");
    } else if (map.getValueType().getString().present()) {
        headers.insert("#include <string>");
    } else if (map.getValueType().getStructure().present()) {
        headers.insert("#include \"" + map.getValueType().getStructure().get() + ".hxx\"");
    } else if (map.getValueType().getArray().present()) {
        addRequiredHeaders(map.getValueType().getArray().get(), headers);
    } else if (map.getValueType().getSequence().present()) {
        addRequiredHeaders(map.getValueType().getSequence().get(), headers);
    } else if (map.getValueType().getMap().present()) {
        addRequiredHeaders(map.getValueType().getMap().get(), headers);
    }
}

std::string CppGenerator::generateGetSizeInBytesCxx(const Messaging::Structure& structure) {
    std::ostringstream os;   
    
    os << insertTabs(0) << "uint64_t " << structure.getName() << "::getSizeInBytes() const {" << std::endl;
    
    os << insertTabs(1) << "uint64_t size = 0;" << std::endl;
    os << std::endl;
    
    for (const auto& primitive : structure.getPrimitive()) {
        os << insertTabs(1) << "// Add on size of " << primitive.getName() << std::endl;
        if (isOptional(primitive)) {            
            os << insertTabs(1) << "// Optionality flag for " << primitive.getName() << " = 1 byte" << std::endl;
            os << insertTabs(1) << "++size;" << std::endl;
            os << std::endl;
            os << insertTabs(1) << "// If " << primitive.getName() << " is present then add on size of it" << std::endl;
            os << insertTabs(1) << "if (" << primitive.getName() << ".has_value()) {" << std::endl;
            os << insertTabs(2) << "size += sizeof(std::decay_t<decltype(" << primitive.getName() << ")>::value_type);" << std::endl;
            os << insertTabs(1) << '}' << std::endl;
        } else {
            os << insertTabs(1) << "size += sizeof(" << primitive.getName() << ");" << std::endl;
        }
        os << std::endl;
    }
    
    for (const auto& enumeration : structure.getEnumeration()) {
        os << insertTabs(1) << "// Add on size of " << enumeration.getName() << std::endl;
        if (isOptional(enumeration)) {            
            os << insertTabs(1) << "// Optionality flag for " << enumeration.getName() << " = 1 byte" << std::endl;
            os << insertTabs(1) << "++size;" << std::endl;
            os << std::endl;
            os << insertTabs(1) << "// If " << enumeration.getName() << " is present then add on size of it" << std::endl;
            os << insertTabs(1) << "if (" << enumeration.getName() << ".has_value()) {" << std::endl;
            os << insertTabs(2) << "size += sizeof(std::decay_t<decltype(" << enumeration.getName() << ")>::value_type);" << std::endl;
            os << insertTabs(1) << '}' << std::endl;
        } else {
            os << insertTabs(1) << "size += sizeof(" << enumeration.getName() << ");" << std::endl;
        }
        os << std::endl;
    }
    
    for (const auto& string : structure.getString()) {
        os << insertTabs(1) << "// Add on size of " << string.getName() << std::endl;
        if (isOptional(string)) {            
            os << insertTabs(1) << "// Optionality flag for " << string.getName() << " = 1 byte" << std::endl;
            os << insertTabs(1) << "++size;" << std::endl;
            os << std::endl;
            os << insertTabs(1) << "// If " << string.getName() << " is present then add on size of it" << std::endl;
            os << insertTabs(1) << "if (" << string.getName() << ".has_value()) {" << std::endl;
            os << insertTabs(2) << "size += sizeof(" << getCxxType(string) << "::value_type) * " << string.getName() << ".value().size();" << std::endl;
            os << std::endl;
            os << insertTabs(2) << "// Add on size of string size field" << std::endl;
            os << insertTabs(2) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
            os << insertTabs(1) << '}' << std::endl;
        } else {
            os << insertTabs(1) << "size += sizeof(" << getCxxType(string) << "::value_type) * " << string.getName() << ".size();" << std::endl;
            os << std::endl;
            os << insertTabs(1) << "// Add on size of string size field" << std::endl;
            os << insertTabs(1) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
        }
        os << std::endl;
    }
       
    for (const auto& subStructure : structure.getStructure()) {
        os << insertTabs(1) << "// Add on size of " << subStructure.getName() << std::endl;
        if (isOptional(subStructure)) {
            os << insertTabs(1) << "// Optionality flag for " << subStructure.getName() << " = 1 byte" << std::endl;
            os << insertTabs(1) << "++size;" << std::endl;
            os << std::endl;
            os << insertTabs(1) << "// If " << subStructure.getName() << " is present then add on size of it" << std::endl;
            os << insertTabs(1) << "if (" << subStructure.getName() << ".has_value()) {" << std::endl;
            os << insertTabs(2) << "size += " << subStructure.getName() << ".value().getSizeInBytes();" << std::endl;
            os << insertTabs(1) << '}' << std::endl;
        } else {
            os << insertTabs(1) << "size += " << subStructure.getName() << ".getSizeInBytes();" << std::endl;
        }
        os << std::endl;
    }
       
    for (const auto& array : structure.getArray()) {
        os << insertTabs(1) << "// Add on size of " << array.getName() << std::endl;
        if (isOptional(array)) {
            os << insertTabs(1) << "// Optionality flag for " << array.getName() << " = 1 byte" << std::endl;
            os << insertTabs(1) << "++size;" << std::endl;
            os << std::endl;
            os << insertTabs(1) << "// If " << array.getName() << " is present then add on size of it" << std::endl;
            os << insertTabs(1) << "if (" << array.getName() << ".has_value()) {" << std::endl;
            os << generateSizeOfArray(array, 2, array.getName() + ".value()");
            os << insertTabs(1) << '}' << std::endl;
        } else {
            os << generateSizeOfArray(array, 1, array.getName());
        }
        os << std::endl;
    }
       
    for (const auto& sequence : structure.getSequence()) {
        os << insertTabs(1) << "// Add on size of " << sequence.getName() << std::endl;
        if (isOptional(sequence)) {
            os << insertTabs(1) << "// Optionality flag for " << sequence.getName() << " = 1 byte" << std::endl;
            os << insertTabs(1) << "++size;" << std::endl;
            os << std::endl;
            os << insertTabs(1) << "// If " << sequence.getName() << " is present then add on size of it" << std::endl;
            os << insertTabs(1) << "if (" << sequence.getName() << ".has_value()) {" << std::endl;
            os << generateSizeOfSequence(sequence, 2, sequence.getName() + ".value()");
            os << insertTabs(1) << '}' << std::endl;
        } else {
            os << generateSizeOfSequence(sequence, 1, sequence.getName());
        }
        os << std::endl;
    }
       
    for (const auto& map : structure.getMap()) {
        os << insertTabs(1) << "// Add on size of " << map.getName() << std::endl;
        if (isOptional(map)) {
            os << insertTabs(1) << "// Optionality flag for " << map.getName() << " = 1 byte" << std::endl;
            os << insertTabs(1) << "++size;" << std::endl;
            os << std::endl;
            os << insertTabs(1) << "// If " << map.getName() << " is present then add on size of it" << std::endl;
            os << insertTabs(1) << "if (" << map.getName() << ".has_value()) {" << std::endl;
            os << generateSizeOfMap(map, 2, map.getName() + ".value()");
            os << insertTabs(1) << '}' << std::endl;
        } else {
            os << generateSizeOfMap(map, 1, map.getName());
        }
        os << std::endl;
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
    } else if (array.getType().getString().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << getCxxType(array.getType().getString().get()) << std::endl;
        os << insertTabs(numTabs + 1) << "size += sizeof(" << getCxxType(array.getType().getString().get()) << "::value_type) * e" << +numTabs << ".size();" << std::endl;
        os << std::endl;
        os << insertTabs(numTabs + 1) << "// Add on size of string size field" << std::endl;
        os << insertTabs(numTabs + 1) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getStructure().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << array.getType().getStructure() << std::endl;
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
    } else if (sequence.getType().getString().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << getCxxType(sequence.getType().getString().get()) << std::endl;
        os << insertTabs(numTabs + 1) << "size += sizeof(" << getCxxType(sequence.getType().getString().get()) << "::value_type) * e" << +numTabs << ".size();" << std::endl;
        os << std::endl;
        os << insertTabs(numTabs + 1) << "// Add on size of string size field" << std::endl;
        os << insertTabs(numTabs + 1) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getStructure().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << sequence.getType().getStructure() << std::endl;
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
    } else if (map.getKeyType().getString().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << getCxxType(map.getKeyType().getString().get()) << std::endl;
        os << insertTabs(numTabs + 1) << "size += sizeof(" << getCxxType(map.getKeyType().getString().get()) << "::value_type) * e" << +numTabs << ".first.size();" << std::endl;
        os << std::endl;
        os << insertTabs(numTabs + 1) << "// Add on size of string size field" << std::endl;
        os << insertTabs(numTabs + 1) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    }    
    
    os << std::endl;
    os << insertTabs(numTabs) << "// Add on size of " << parentName << " value data" << std::endl;
    
    if (map.getValueType().getPrimitive().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << convertPrimitiveTypeToCppType(map.getValueType().getPrimitive().get()) << ") * " << parentName << ".size();" << std::endl;
    } else if (map.getValueType().getEnumeration().present()) {
        os << insertTabs(numTabs) << "size += sizeof(" << map.getValueType().getEnumeration().get() << ") * " << parentName << ".size();" << std::endl;
    }  else if (map.getValueType().getString().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << getCxxType(map.getValueType().getString().get()) << std::endl;
        os << insertTabs(numTabs + 1) << "size += sizeof(" << getCxxType(map.getValueType().getString().get()) << "::value_type) * e" << +numTabs << ".second.size();" << std::endl;
        os << std::endl;
        os << insertTabs(numTabs + 1) << "// Add on size of string size field" << std::endl;
        os << insertTabs(numTabs + 1) << "size += sizeof(" << SEQ_SIZE_TYPE << ");" << std::endl;
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (map.getValueType().getStructure().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;        
        os << insertTabs(numTabs + 1) << "// Add on size of each individual " << map.getValueType().getStructure() << std::endl;
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
        (structure.getString().empty()) &&
        (structure.getStructure().empty()) &&
        (structure.getArray().empty()) &&
        (structure.getSequence().empty()) &&
        (structure.getMap().empty())) {
        os << insertTabs(1) << "// Nothing to serialise" << std::endl;
    } else {
        for (const auto& primitive : structure.getPrimitive()) {
            os << insertTabs(1) << "// Serialise " << primitive.getName() << std::endl;
            if (isOptional(primitive)) {
                os << insertTabs(1) << "// Serialise " << primitive.getName() << " presence flag" << std::endl;
                os << insertTabs(1) << "*(data + offset++) = " << primitive.getName() << ".has_value() ? 1 : 0;" << std::endl;
                os << std::endl;
                os << insertTabs(1) << "// Serialise " << primitive.getName() << " data" << std::endl;
                os << insertTabs(1) << "if (" << primitive.getName() << ".has_value()) {" << std::endl;
                os << generateSerialise(primitive, 2, primitive.getName() + ".value()");
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateSerialise(primitive, 1, primitive.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& enumeration : structure.getEnumeration()) {
            os << insertTabs(1) << "// Serialise " << enumeration.getName() << std::endl;
            if (isOptional(enumeration)) {
                os << insertTabs(1) << "// Serialise " << enumeration.getName() << " presence flag" << std::endl;
                os << insertTabs(1) << "*(data + offset++) = " << enumeration.getName() << ".has_value() ? 1 : 0;" << std::endl;
                os << std::endl;
                os << insertTabs(1) << "// Serialise " << enumeration.getName() << " data" << std::endl;
                os << insertTabs(1) << "if (" << enumeration.getName() << ".has_value()) {" << std::endl;
                os << generateSerialise(enumeration, 2, enumeration.getName() + ".value()");
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateSerialise(enumeration, 1, enumeration.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& string : structure.getString()) {
            os << insertTabs(1) << "// Serialise " << string.getName() << std::endl;
            if (isOptional(string)) {
                os << insertTabs(1) << "// Serialise " << string.getName() << " presence flag" << std::endl;
                os << insertTabs(1) << "*(data + offset++) = " << string.getName() << ".has_value() ? 1 : 0;" << std::endl;
                os << std::endl;
                os << insertTabs(1) << "// Serialise " << string.getName() << " data" << std::endl;
                os << insertTabs(1) << "if (" << string.getName() << ".has_value()) {" << std::endl;
                os << generateSerialise(string, 2, string.getName() + ".value()");
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateSerialise(string, 1, string.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& subStructure : structure.getStructure()) {
            os << insertTabs(1) << "// Serialise " << subStructure.getName() << std::endl;
            if (isOptional(subStructure)) {
                os << insertTabs(1) << "// Serialise " << subStructure.getName() << " presence flag" << std::endl;
                os << insertTabs(1) << "*(data + offset++) = " << subStructure.getName() << ".has_value() ? 1 : 0;" << std::endl;
                os << std::endl;
                os << insertTabs(1) << "// Serialise " << subStructure.getName() << " data" << std::endl;
                os << insertTabs(1) << "if (" << subStructure.getName() << ".has_value()) {" << std::endl;
                os << generateSerialise(subStructure, 2, subStructure.getName() + ".value()");
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateSerialise(subStructure, 1, subStructure.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& array : structure.getArray()) {
            os << insertTabs(1) << "// Serialise " << array.getName() << std::endl;
            if (isOptional(array)) {
                os << insertTabs(1) << "// Serialise " << array.getName() << " presence flag" << std::endl;
                os << insertTabs(1) << "*(data + offset++) = " << array.getName() << ".has_value() ? 1 : 0;" << std::endl;
                os << std::endl;
                os << insertTabs(1) << "// Serialise " << array.getName() << " data" << std::endl;
                os << insertTabs(1) << "if (" << array.getName() << ".has_value()) {" << std::endl;
                os << generateSerialise(array, 2, array.getName() + ".value()");
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateSerialise(array, 1, array.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& sequence : structure.getSequence()) {
            os << insertTabs(1) << "// Serialise " << sequence.getName() << std::endl;
            if (isOptional(sequence)) {
                os << insertTabs(1) << "// Serialise " << sequence.getName() << " presence flag" << std::endl;
                os << insertTabs(1) << "*(data + offset++) = " << sequence.getName() << ".has_value() ? 1 : 0;" << std::endl;
                os << std::endl;
                os << insertTabs(1) << "// Serialise " << sequence.getName() << " data" << std::endl;
                os << insertTabs(1) << "if (" << sequence.getName() << ".has_value()) {" << std::endl;
                os << generateSerialise(sequence, 2, sequence.getName() + ".value()");
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateSerialise(sequence, 1, sequence.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& map : structure.getMap()) {
            os << insertTabs(1) << "// Serialise " << map.getName() << std::endl;
            if (isOptional(map)) {
                os << insertTabs(1) << "// Serialise " << map.getName() << " presence flag" << std::endl;
                os << insertTabs(1) << "*(data + offset++) = " << map.getName() << ".has_value() ? 1 : 0;" << std::endl;
                os << std::endl;
                os << insertTabs(1) << "// Serialise " << map.getName() << " data" << std::endl;
                os << insertTabs(1) << "if (" << map.getName() << ".has_value()) {" << std::endl;
                os << generateSerialise(map, 2, map.getName() + ".value()");
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateSerialise(map, 1, map.getName());
            }
            
            os << std::endl;
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
        (structure.getString().empty()) &&
        (structure.getStructure().empty()) &&
        (structure.getArray().empty()) &&
        (structure.getSequence().empty()) &&
        (structure.getMap().empty())) {
        os << insertTabs(1) << "// Nothing to deserialise" << std::endl;
    } else {
        for (const auto& primitive : structure.getPrimitive()) {
            os << insertTabs(1) << "// Deserialise " << primitive.getName() << std::endl;
            if (isOptional(primitive)) {
                os << insertTabs(1) << "if (*(data + offset++) == 1) {" << std::endl;
                os << insertTabs(2) << primitive.getName() << " = typename std::decay_t<decltype(" << primitive.getName() << ")>::value_type{};" << std::endl;
                os << generateDeserialise(primitive, 2, primitive.getName() + ".value()");
                os << insertTabs(1) << "} else {" << std::endl;
                os << insertTabs(2) << primitive.getName() << ".reset();" << std::endl;
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateDeserialise(primitive, 1, primitive.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& enumeration : structure.getEnumeration()) {
            os << insertTabs(1) << "// Deserialise " << enumeration.getName() << std::endl;
            if (isOptional(enumeration)) {
                os << insertTabs(1) << "if (*(data + offset++) == 1) {" << std::endl;
                os << insertTabs(2) << enumeration.getName() << " = typename std::decay_t<decltype(" << enumeration.getName() << ")>::value_type{};" << std::endl;
                os << generateDeserialise(enumeration, 2, enumeration.getName() + ".value()");
                os << insertTabs(1) << "} else {" << std::endl;
                os << insertTabs(2) << enumeration.getName() << ".reset();" << std::endl;
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateDeserialise(enumeration, 1, enumeration.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& string : structure.getString()) {
            os << insertTabs(1) << "// Deserialise " << string.getName() << std::endl;
            if (isOptional(string)) {
                os << insertTabs(1) << "if (*(data + offset++) == 1) {" << std::endl;
                os << insertTabs(2) << string.getName() << " = typename std::decay_t<decltype(" << string.getName() << ")>::value_type{};" << std::endl;
                os << generateDeserialise(string, 2, string.getName() + ".value()");
                os << insertTabs(1) << "} else {" << std::endl;
                os << insertTabs(2) << string.getName() << ".reset();" << std::endl;
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateDeserialise(string, 1, string.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& subStructure : structure.getStructure()) {
            os << insertTabs(1) << "// Deserialise " << subStructure.getName() << std::endl;
            if (isOptional(subStructure)) {
                os << insertTabs(1) << "if (*(data + offset++) == 1) {" << std::endl;
                os << insertTabs(2) << subStructure.getName() << " = typename std::decay_t<decltype(" << subStructure.getName() << ")>::value_type{};" << std::endl;
                os << generateDeserialise(subStructure, 2, subStructure.getName() + ".value()");
                os << insertTabs(1) << "} else {" << std::endl;
                os << insertTabs(2) << subStructure.getName() << ".reset();" << std::endl;
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateDeserialise(subStructure, 1, subStructure.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& array : structure.getArray()) {
            os << insertTabs(1) << "// Deserialise " << array.getName() << std::endl;
            if (isOptional(array)) {
                os << insertTabs(1) << "if (*(data + offset++) == 1) {" << std::endl;
                os << insertTabs(2) << array.getName() << " = typename std::decay_t<decltype(" << array.getName() << ")>::value_type{};" << std::endl;
                os << generateDeserialise(array, 2, array.getName() + ".value()");
                os << insertTabs(1) << "} else {" << std::endl;
                os << insertTabs(2) << array.getName() << ".reset();" << std::endl;
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateDeserialise(array, 1, array.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& sequence : structure.getSequence()) {
            os << insertTabs(1) << "// Deserialise " << sequence.getName() << std::endl;
            if (isOptional(sequence)) {
                os << insertTabs(1) << "if (*(data + offset++) == 1) {" << std::endl;
                os << insertTabs(2) << sequence.getName() << " = typename std::decay_t<decltype(" << sequence.getName() << ")>::value_type{};" << std::endl;
                os << generateDeserialise(sequence, 2, sequence.getName() + ".value()");
                os << insertTabs(1) << "} else {" << std::endl;
                os << insertTabs(2) << sequence.getName() << ".reset();" << std::endl;
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateDeserialise(sequence, 1, sequence.getName());
            }
            
            os << std::endl;
        }
        
        for (const auto& map : structure.getMap()) {
            os << insertTabs(1) << "// Deserialise " << map.getName() << std::endl;
            if (isOptional(map)) {
                os << insertTabs(1) << "if (*(data + offset++) == 1) {" << std::endl;
                os << insertTabs(2) << map.getName() << " = typename std::decay_t<decltype(" << map.getName() << ")>::value_type{};" << std::endl;
                os << generateDeserialise(map, 2, map.getName() + ".value()");
                os << insertTabs(1) << "} else {" << std::endl;
                os << insertTabs(2) << map.getName() << ".reset();" << std::endl;
                os << insertTabs(1) << '}' << std::endl;
            } else {
                os << generateDeserialise(map, 1, map.getName());
            }
            
            os << std::endl;
        }
    }
    
    os.seekp(-1, os.cur);
    os << insertTabs() << '}' << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::NamedPrimitive&,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;   
    
    os << generateSerialiseElement(parentName, numTabs);

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::NamedPrimitive&,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;   
    
    os << generateDeserialiseElement(parentName, numTabs);

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::NamedEnumeration&,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;   
    
    os << generateSerialiseElement(parentName, numTabs);

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::NamedEnumeration&,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;   
    
    os << generateDeserialiseElement(parentName, numTabs);

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::String&,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;   
    std::string sensibleName = parentName;
    std::erase(sensibleName, '.');
    std::erase(sensibleName, ')');
    std::erase(sensibleName, '(');
    
    os << insertTabs(numTabs) << "// Serialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size = " << parentName << ".size();" << std::endl;    
    os << generateSerialiseElement(sensibleName + "Size", numTabs);
    os << std::endl;
    os << insertTabs(numTabs) << "// Serialise " << parentName << " data" << std::endl;
    os << generateSerialiseElementWithSize(parentName + "[0]", parentName + ".size()", numTabs);

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::String&,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;  
    std::string sensibleName = parentName;
    std::erase(sensibleName, '.');
    std::erase(sensibleName, ')');
    std::erase(sensibleName, '(');
    
    os << insertTabs(numTabs) << "// Deserialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size;" << std::endl;
    os << generateDeserialiseElement(sensibleName + "Size", numTabs);    
    os << insertTabs(numTabs) << parentName << ".resize(" << sensibleName << "Size);" << std::endl;
    os << std::endl;
    os << insertTabs(numTabs) << "// Deserialise " << parentName << " data" << std::endl;
    os << generateDeserialiseElementWithSize(parentName + "[0]", parentName + ".size()", numTabs);

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::NamedStructure&,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;   
    
    os << insertTabs(numTabs) << parentName << ".serialise(data, offset);" << std::endl;

    return os.str();
}

std::string CppGenerator::generateDeserialise(const Messaging::NamedStructure&,
                                              const uint8_t numTabs,
                                              const std::string& parentName) {
    std::ostringstream os;   
    
    os << insertTabs(numTabs) << parentName << ".deserialise(data, offset);" << std::endl;

    return os.str();
}

std::string CppGenerator::generateSerialise(const Messaging::Array& array,
                                            const uint8_t numTabs,
                                            const std::string& parentName) {
    std::ostringstream os;
    
    if (array.getType().getPrimitive().present()) {
        os << generateSerialiseElementWithSize(parentName + "[0]", parentName + ".size()", numTabs);
    } else if (array.getType().getEnumeration().present()) {
        os << generateSerialiseElementWithSize(parentName + "[0]", parentName + ".size()", numTabs);
    } else if (array.getType().getString().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialise(array.getType().getString().get(), numTabs + 1, "e"  + std::to_string(numTabs));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getStructure().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".serialise(data, offset);" << std::endl;
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
        os << generateDeserialiseElementWithSize(parentName + "[0]", parentName + ".size()", numTabs);
    } else if (array.getType().getEnumeration().present()) {
        os << generateDeserialiseElementWithSize(parentName + "[0]", parentName + ".size()", numTabs);
    } else if (array.getType().getString().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialise(array.getType().getString().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (array.getType().getStructure().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".deserialise(data, offset);" << std::endl;
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
    std::erase(sensibleName, '.');
    std::erase(sensibleName, ')');
    std::erase(sensibleName, '(');
    
    os << insertTabs(numTabs) << "// Serialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size = " << parentName << ".size();" << std::endl;    
    os << generateSerialiseElement(sensibleName + "Size", numTabs);
    os << std::endl;
    os << insertTabs(numTabs) << "// Serialise " << parentName << " data" << std::endl;
    
    if (sequence.getType().getPrimitive().present()) {
        os << generateSerialiseElementWithSize(parentName + "[0]", sensibleName + "Size", numTabs);
    } else if (sequence.getType().getEnumeration().present()) {
        os << generateSerialiseElementWithSize(parentName + "[0]", sensibleName + "Size", numTabs);
    } else if (sequence.getType().getString().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateSerialise(sequence.getType().getString().get(), numTabs + 1, "e"  + std::to_string(numTabs));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getStructure().present()) {
        os << insertTabs(numTabs) << "for (const auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".serialise(data, offset);" << std::endl;
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
    std::erase(sensibleName, '.');
    std::erase(sensibleName, ')');
    std::erase(sensibleName, '(');
    
    os << insertTabs(numTabs) << "// Deserialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size;" << std::endl;
    os << generateDeserialiseElement(sensibleName + "Size", numTabs);
    os << std::endl;
    os << insertTabs(numTabs) << "// Deserialise " << parentName << " data" << std::endl;
    os << insertTabs(numTabs) << parentName << ".resize(" << sensibleName << "Size);" << std::endl;
    
    if (sequence.getType().getPrimitive().present()) {
        os << generateDeserialiseElementWithSize(parentName + "[0]", sensibleName + "Size", numTabs);
    } else if (sequence.getType().getEnumeration().present()) {
        os << generateDeserialiseElementWithSize(parentName + "[0]", sensibleName + "Size", numTabs);
    } else if (sequence.getType().getString().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << generateDeserialise(sequence.getType().getString().get(), numTabs + 1, ("e" + std::to_string(numTabs)));
        os << insertTabs(numTabs) << '}' << std::endl;
    } else if (sequence.getType().getStructure().present()) {
        os << insertTabs(numTabs) << "for (auto& e" << +numTabs << " : " << parentName << ") {" << std::endl;
        os << insertTabs(numTabs + 1) << 'e' << +numTabs << ".deserialise(data, offset);" << std::endl;
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
    std::erase(sensibleName, '.');
    std::erase(sensibleName, ')');
    std::erase(sensibleName, '(');
    
    os << insertTabs(numTabs) << "// Serialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size = " << parentName << ".size();" << std::endl;    
    os << generateSerialiseElement(sensibleName + "Size", numTabs);
    os << std::endl;
    os << insertTabs(numTabs) << "for (const auto& e" << +(numTabs - 1) << " : " << parentName << ") {" << std::endl;
    os << insertTabs(numTabs + 1) << "// Serialise " << sensibleName << " key data" << std::endl;
    
    if (map.getKeyType().getPrimitive().present()) {    
        os << generateSerialiseElement("e" + std::to_string(numTabs - 1) + ".first", numTabs + 1);
    } else if (map.getKeyType().getEnumeration().present()) {
        os << generateSerialiseElement("e" + std::to_string(numTabs - 1) + ".first", numTabs + 1);
    } else if (map.getKeyType().getString().present()) {
        os << generateSerialise(map.getKeyType().getString().get(), numTabs + 1, "e"  + std::to_string(numTabs - 1) + ".first");
    } 

    os << std::endl;
    os << insertTabs(numTabs + 1) << "// Serialise " << sensibleName << " value data" << std::endl;
    
    if (map.getValueType().getPrimitive().present()) {
        os << generateSerialiseElement("e" + std::to_string(numTabs - 1) + ".second", numTabs + 1);
    } else if (map.getValueType().getEnumeration().present()) {
        os << generateSerialiseElement("e" + std::to_string(numTabs - 1) + ".second", numTabs + 1);
    } else if (map.getValueType().getString().present()) {
        os << generateSerialise(map.getValueType().getString().get(), numTabs + 1, "e"  + std::to_string(numTabs - 1) + ".second");
    } else if (map.getValueType().getStructure().present()) {
        os << insertTabs(numTabs + 1) << 'e' << +(numTabs - 1) << ".second.serialise(data, offset);" << std::endl;
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
    std::erase(sensibleName, '.');
    std::erase(sensibleName, ')');
    std::erase(sensibleName, '(');
    
    os << insertTabs(numTabs) << "// Deserialise size of " << parentName << std::endl;
    os << insertTabs(numTabs) << SEQ_SIZE_TYPE << ' ' << sensibleName << "Size;" << std::endl;
    os << generateDeserialiseElement(sensibleName + "Size", numTabs);
    os << std::endl;
    os << insertTabs(numTabs) << "// Deserialise " << parentName << " data" << std::endl;
    os << insertTabs(numTabs) << "for (" << SEQ_SIZE_TYPE << " index = 0; index < " << sensibleName << "Size; ++index) {" << std::endl;
    os << insertTabs(numTabs + 1) << "// Deserialise " << sensibleName << " key data" << std::endl;
    
    if (map.getKeyType().getPrimitive().present()) {
        os << insertTabs(numTabs + 1) << convertPrimitiveTypeToCppType(map.getKeyType().getPrimitive().get()) << " first" << +(numTabs - 1) << ';' << std::endl;
        os << generateDeserialiseElement("first" + std::to_string(numTabs - 1), numTabs + 1);
    } else if (map.getKeyType().getEnumeration().present()) {
        os << insertTabs(numTabs + 1) << map.getKeyType().getEnumeration().get() << " first" << +(numTabs - 1) << ';' << std::endl;
        os << generateDeserialiseElement("first" + std::to_string(numTabs - 1), numTabs + 1);
    } else if (map.getKeyType().getString().present()) {
        os << insertTabs(numTabs + 1) << getCxxType(map.getKeyType().getString().get()) << " first" << +(numTabs - 1) << ';' << std::endl;
        os << generateDeserialise(map.getKeyType().getString().get(), numTabs + 1, "first" + std::to_string(numTabs - 1));
    } 

    os << std::endl;
    os << insertTabs(numTabs + 1) << "// Deserialise " << sensibleName << " value data" << std::endl;
    
    if (map.getValueType().getPrimitive().present()) {
        os << insertTabs(numTabs + 1) << convertPrimitiveTypeToCppType(map.getValueType().getPrimitive().get()) << " second" << +(numTabs - 1) << ';' << std::endl;
        os << generateDeserialiseElement("second" + std::to_string(numTabs - 1), numTabs + 1);
    } else if (map.getValueType().getEnumeration().present()) {
        os << insertTabs(numTabs + 1) << map.getValueType().getEnumeration().get() << " second" << +(numTabs - 1) << ";" << std::endl;
        os << generateDeserialiseElement("second" + std::to_string(numTabs - 1), numTabs + 1);
    }  else if (map.getValueType().getString().present()) {
        os << insertTabs(numTabs + 1) << getCxxType(map.getValueType().getString().get()) << " second" << +(numTabs - 1) << ';' << std::endl;
        os << generateDeserialise(map.getValueType().getString().get(), numTabs + 1, "second" + std::to_string(numTabs - 1));
    } else if (map.getValueType().getStructure().present()) {
        os << insertTabs(numTabs + 1) << map.getValueType().getStructure().get() << " second" << +(numTabs - 1) << ';' << std::endl;
        os << insertTabs(numTabs + 1) << "second" << +(numTabs - 1) << ".deserialise(data, offset);" << std::endl;
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
    } else if (array.getType().getString().present()) {
        os << getCxxType(array.getType().getString().get());
    } else if (array.getType().getStructure().present()) {
        os << array.getType().getStructure().get();
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

std::string CppGenerator::getCxxType(const Messaging::String& string) {
    std::ostringstream os;
    
    if (string.getType() == "8") {
        os << "std::string";
    } else if (string.getType() == "16") {
        os << "std::u16string";
    } else if (string.getType() == "32") {
        os << "std::u32string";
    }
    
    return os.str();
}

std::string CppGenerator::getCxxType(const Messaging::Sequence& sequence) {
    std::ostringstream os;
    
    os << "std::vector<";
    
    if (sequence.getType().getPrimitive().present()) {
        os << convertPrimitiveTypeToCppType(sequence.getType().getPrimitive().get());
    } else if (sequence.getType().getEnumeration().present()) {
        os << sequence.getType().getEnumeration().get();
    } else if (sequence.getType().getString().present()) {
        os << getCxxType(sequence.getType().getString().get());
    } else if (sequence.getType().getStructure().present()) {
        os << sequence.getType().getStructure().get();
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
    } else if (map.getKeyType().getString().present()) {
        os << getCxxType(map.getKeyType().getString().get());
    }
    
    os << ", ";
    
    if (map.getValueType().getPrimitive().present()) {
        os << convertPrimitiveTypeToCppType(map.getValueType().getPrimitive().get());
    } else if (map.getValueType().getEnumeration().present()) {
        os << map.getValueType().getEnumeration().get();
    } else if (map.getValueType().getString().present()) {
        os << getCxxType(map.getValueType().getString().get());
    } else if (map.getValueType().getStructure().present()) {
        os << map.getValueType().getStructure().get();
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

bool CppGenerator::isOptional(const auto& element) {
    bool returnVal = false;

    if (element.getOptional().present()) {
        returnVal = element.getOptional().get();
    }
    
    return returnVal;
}

std::string CppGenerator::generateSerialiseElement(const std::string& element,
                                                   const uint8_t numTabs) {
    std::ostringstream os;
    
    os << insertTabs(numTabs) << "memcpy(data + offset, &" << element << ", sizeof(" << element << "));" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << element << ");" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateSerialiseElementWithSize(const std::string& element,
                                                           const std::string& size,
                                                           const uint8_t numTabs) {
    std::ostringstream os;
    
    os << insertTabs(numTabs) << "memcpy(data + offset, &" << element << ", sizeof(" << element << ") * " << size << ");" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << element << ") * " << size << ';' << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateDeserialiseElement(const std::string& element,
                                                     const uint8_t numTabs) {
    std::ostringstream os;
    
    os << insertTabs(numTabs) << "memcpy(&" << element << ", data + offset, sizeof(" << element << "));" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << element << ");" << std::endl;
    
    return os.str();
}

std::string CppGenerator::generateDeserialiseElementWithSize(const std::string& element,
                                                             const std::string& size,
                                                             const uint8_t numTabs) {
    std::ostringstream os;
    
    os << insertTabs(numTabs) << "memcpy(&" << element << ", data + offset, sizeof(" << element << ") * " << size << ");" << std::endl;
    os << insertTabs(numTabs) << "offset += sizeof(" << element << ") * " << size << ';' << std::endl;
    
    return os.str();
}
