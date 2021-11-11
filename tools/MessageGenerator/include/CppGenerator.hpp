#pragma once

#include "Generator.hpp"

class CppGenerator : Generator {
    public:
        CppGenerator(const Messaging::Definitions& definitions);
        
        bool generate(const std::string& outputFolder);

    private:     
        void sanityCheck();
        
        bool generateHeaderFile(const Messaging::Structure& structure,
                                const std::string& outputFolder,
                                const Messaging::Definitions::NamespaceOptional& namespaceOptional);
        
        bool generateHeaderFile(const Messaging::Enumeration& enumeration,
                                const std::string& outputFolder,
                                const Messaging::Definitions::NamespaceOptional& namespaceOptional);
        
        bool generateSourceFile(const Messaging::Structure& structure,
                                const std::string& outputFolder,
                                const Messaging::Definitions::NamespaceOptional& namespaceOptional);

        std::string generateConstructorHxx(const Messaging::Structure& structure);

        std::string generateDestructorHxx(const Messaging::Structure& structure);

        std::string generateSerialiseHxx();

        std::string generateDeserialiseHxx();

        std::string generateGetSizeInBytesHxx();

        std::string generateGettersHxx(const Messaging::Structure& structure);

        std::string generateSettersHxx(const Messaging::Structure& structure);

        std::string generateGetterHxx(const Messaging::NamedPrimitive& element);

        std::string generateSetterHxx(const Messaging::NamedPrimitive& element);

        std::string generateGetterHxx(const Messaging::NamedEnumeration& element);

        std::string generateSetterHxx(const Messaging::NamedEnumeration& element);

        std::string generateGetterHxx(const Messaging::NamedStructure& structure);

        std::string generateGetterHxx(const Messaging::NamedSequence& sequence);

        std::string generateGetterHxx(const Messaging::NamedArray& array);

        std::string generateGetterHxx(const Messaging::NamedMap& map);

        std::string generateMembersHxx(const Messaging::Structure& structure);

        std::string generateIncludesHxx(const Messaging::Structure& structure);

        std::string generateConstructorCxx(const Messaging::Structure& structure);

        std::string generateDestructorCxx(const Messaging::Structure& structure);

        std::string generateGetSizeInBytesCxx(const Messaging::Structure& structure);

        std::string generateSerialiseCxx(const Messaging::Structure& structure);

        std::string generateDeserialiseCxx(const Messaging::Structure& structure);

        std::string generateSerialise(const Messaging::NamedPrimitive& element,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::NamedPrimitive& element,
                                        const uint8_t numTabs,
                                        const std::string& parentName);

        std::string generateSerialise(const Messaging::NamedEnumeration& enumeration,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::NamedEnumeration& enumeration,
                                        const uint8_t numTabs,
                                        const std::string& parentName);

        std::string generateSerialise(const Messaging::NamedStructure& structure,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::NamedStructure& structure,
                                        const uint8_t numTabs,
                                        const std::string& parentName);

        std::string generateSerialise(const Messaging::Array& array,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::Array& array,
                                        const uint8_t numTabs,
                                        const std::string& parentName);

        std::string generateSerialise(const Messaging::Sequence& sequence,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::Sequence& sequence,
                                        const uint8_t numTabs,
                                        const std::string& parentName);

        std::string generateSerialise(const Messaging::Map& map,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::Map& map,
                                        const uint8_t numTabs,
                                        const std::string& parentName);

        std::string generateGettersCxx(const Messaging::Structure& structure);

        std::string generateSettersCxx(const Messaging::Structure& structure);

        std::string generateGetterCxx(const Messaging::Structure& structure,
                                      const Messaging::NamedPrimitive& element);

        std::string generateSetterCxx(const Messaging::Structure& structure,
                                      const Messaging::NamedPrimitive& element);

        std::string generateGetterCxx(const Messaging::Structure& structure,
                                      const Messaging::NamedEnumeration& enumeration);

        std::string generateSetterCxx(const Messaging::Structure& structure,
                                      const Messaging::NamedEnumeration& enumeration);

        std::string generateGetterCxx(const Messaging::Structure& structure,
                                      const Messaging::NamedStructure& subStructure);

        std::string generateGetterCxx(const Messaging::Structure& structure,
                                      const Messaging::NamedArray& array);

        std::string generateGetterCxx(const Messaging::Structure& structure,
                                      const Messaging::NamedSequence& sequence);

        std::string generateGetterCxx(const Messaging::Structure& structure,
                                      const Messaging::NamedMap& map);
        
        uint64_t sizeOfPrimitiveType(const Messaging::NamedPrimitive::TypeType& type);
        
        uint64_t sizeOfEnumeration(const std::string& enumString);
        
        std::string getCxxType(const Messaging::Array& array);
        
        std::string getCxxType(const Messaging::Sequence& sequence);
        
        std::string getCxxType(const Messaging::Map& map);
        
        std::string generateSizeOfArray(const Messaging::Array& array,
                                        const uint8_t numTabs,
                                        const std::string& parentName);
        
        std::string generateSizeOfSequence(const Messaging::Sequence& sequence,
                                           const uint8_t numTabs,
                                           const std::string& parentName);
        
        std::string generateSizeOfMap(const Messaging::Map& map,
                                      const uint8_t numTabs,
                                      const std::string& parentName);
        
        std::string convertEnumBaseTypeToCppType(const Messaging::Enumeration::BaseTypeType& enumeration);

        std::string convertPrimitiveTypeToCppType(const Messaging::NamedPrimitive::TypeType& element);
        
        const static std::string SEQ_SIZE_TYPE;
};