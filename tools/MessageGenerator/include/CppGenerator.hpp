#pragma once

#include "Generator.hpp"

class CppGenerator : Generator {
    public:
        bool generate(const Definitions& definitions,
                      const std::string& outputFolder);

    private:        
        bool generateHeaderFile(const Structure& structure,
                                const std::string& outputFolder,
                                const Definitions::NamespaceOptional& namespaceOptional);
        
        bool generateHeaderFile(const Enumeration& enumeration,
                                const std::string& outputFolder,
                                const Definitions::NamespaceOptional& namespaceOptional);
        
        bool generateSourceFile(const Structure& structure,
                                const std::string& outputFolder,
                                const Definitions::NamespaceOptional& namespaceOptional);

        std::string generateConstructorHxx(const Structure& structure);

        std::string generateDestructorHxx(const Structure& structure);

        std::string generateSerialiseHxx();

        std::string generateDeserialiseHxx();

        std::string generateGetSizeInBytesHxx();

        std::string generateGettersHxx(const Structure& structure);

        std::string generateSettersHxx(const Structure& structure);

        std::string generateGetterHxx(const PrimitiveElement& element);

        std::string generateSetterHxx(const PrimitiveElement& element);

        std::string generateGetterHxx(const IncludedEnumeration& element);

        std::string generateSetterHxx(const IncludedEnumeration& element);

        std::string generateMembersHxx(const Structure& structure);

        std::string generateIncludesHxx(const Structure& structure);

        std::string generateConstructorCxx(const Structure& structure);

        std::string generateDestructorCxx(const Structure& structure);

        std::string generateGetSizeInBytesCxx(const Structure& structure);

        std::string generateSerialiseCxx(const Structure& structure);

        std::string generateDeserialiseCxx(const Structure& structure);

        std::string generateGettersCxx(const Structure& structure);

        std::string generateSettersCxx(const Structure& structure);
        
        uint64_t sizeOfPrimitiveElement(const PrimitiveElement& element);
        
        std::string convertEnumToCppBaseType(const Enumeration& enumeration);

        std::string convertPrimitiveElementToCppType(const PrimitiveElement& element);
};