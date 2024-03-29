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

        std::string generateSetterHxx(const auto& element,
                                      const std::string& type,
                                      const bool passAsReference = false);

        std::string generateGetterHxx(const auto& map,
                                      const std::string& type,
                                      const bool returnReference = true);
        
        std::string generateGetterCxx(const Messaging::Structure& structure,
                                      const auto& element,
                                      const std::string& type,
                                      const bool returnReference = true);

        std::string generateMembersHxx(const Messaging::Structure& structure);

        std::string generateIncludesHxx(const Messaging::Structure& structure);

        std::string generateConstructorCxx(const Messaging::Structure& structure);

        std::string generateDestructorCxx(const Messaging::Structure& structure);

        std::string generateGetSizeInBytesCxx(const Messaging::Structure& structure);

        std::string generateSerialiseCxx(const Messaging::Structure& structure);

        std::string generateDeserialiseCxx(const Messaging::Structure& structure);

        std::string generateSerialise(const Messaging::NamedPrimitive& primitive,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::NamedPrimitive& primitive,
                                        const uint8_t numTabs,
                                        const std::string& parentName);

        std::string generateSerialise(const Messaging::NamedEnumeration& enumeration,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::NamedEnumeration& enumeration,
                                        const uint8_t numTabs,
                                        const std::string& parentName);

        std::string generateSerialise(const Messaging::String& string,
                                      const uint8_t numTabs,
                                      const std::string& parentName);

        std::string generateDeserialise(const Messaging::String& string,
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

        std::string generateSerialiseElement(const std::string& element,
                                             const uint8_t numTabs);

        std::string generateSerialiseElementWithSize(const std::string& element,
                                                     const std::string& size,
                                                     const uint8_t numTabs);

        std::string generateDeserialiseElement(const std::string& element,
                                               const uint8_t numTabs);

        std::string generateDeserialiseElementWithSize(const std::string& element,
                                                       const std::string& size,
                                                       const uint8_t numTabs);

        std::string generateGettersCxx(const Messaging::Structure& structure);

        std::string generateSettersCxx(const Messaging::Structure& structure);

        std::string generateSetterCxx(const Messaging::Structure& structure,
                                      const auto& element,
                                      const std::string& type,
                                      const bool passAsReference = false);
        
        std::string getCxxType(const Messaging::String& string);
        
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

        std::string convertPrimitiveTypeToCppType(const Messaging::NamedPrimitive::TypeType& primitive);
        
        bool isOptional(const auto& element);

        void addRequiredHeaders(const Messaging::Array array,
                                std::set<std::string>& headers);

        void addRequiredHeaders(const Messaging::Sequence sequence,
                                std::set<std::string>& headers);

        void addRequiredHeaders(const Messaging::Map map,
                                std::set<std::string>& headers);
        
        const static std::string SEQ_SIZE_TYPE;
};