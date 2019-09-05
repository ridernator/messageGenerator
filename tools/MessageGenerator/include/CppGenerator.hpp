#pragma once

#include "Definitions.hxx"

class CppGenerator {
    public:
        bool generate(const Definitions& definitions,
                      const std::string& outputFolder);

    private:        
        bool generateHeaderFile(const Structure& structure,
                                const std::string& outputFolder);
        
        bool generateHeaderFile(const Enumeration& enumeration,
                                const std::string& outputFolder);
        
        bool generateSourceFile(const Structure& structure,
                                const std::string& outputFolder);

        std::string generateConstructorHxx(const Structure& structure);

        std::string generateConstructorCxx(const Structure& structure);

        std::string generateDestructorHxx(const Structure& structure);

        std::string generateDestructorCxx(const Structure& structure);

        std::set<std::string> getListOfStructIncludes(const Structure& structure);

        std::set<std::string> getListOfStructIncludes(const Element& element);

        std::string generateConstants(const Structure& structure);

        std::string generateGetSizeInBytesHxx();

        std::string generateGetSizeInBytesCxx(const Structure& structure);

        std::string generateSerialiseHxx();

        std::string generateSerialiseCxx(const Structure& structure);

        std::string generateDeserialiseHxx();

        std::string generateDeserialiseCxx(const Structure& structure);

        std::string generateGetterHxx(const Element& element);

        std::string generateGettersHxx(const Structure& structure);
        
        std::string generateSetterHxx(const Element& element);

        std::string generateSettersHxx(const Structure& structure);

        std::string generateIncludesHxx(const Structure& structure);

        std::string generateMembersHxx(const Structure& structure);

        std::string convertElementToCppType(const Element& element);

        bool isPrimitiveType(const Element& element);

        std::string convertConstantToCppConstant(const Constant& constant);
        
        std::string convertBaseTypeToCppType(const Enumeration::BaseTypeType& type);

        std::string generateMemberSerialisation(const Element& element,
                                                const std::string& nameToUse,
                                                const uint8_t numTabs = 1);

        std::string generateMemberDeserialisation(const Element& element,
                                                  const std::string& nameToUse,
                                                  const uint8_t numTabs = 1);
        
        uint64_t sizeOfPrimitiveType(const Element::TypeType type);

        std::string generateSizeStatement(const Element& element,
                                          const std::string& nameToUse,
                                          const uint8_t numTabs = 1);
        
        std::string insertTabs(const uint8_t numTabs);
};