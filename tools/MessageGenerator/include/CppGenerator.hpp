#pragma once

#include "Generator.hpp"

class CppGenerator : Generator {
    public:
        CppGenerator(const Definitions& definitions);
        
        bool generate(const std::string& outputFolder);

    private:     
        void sanityCheck();
        
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

        std::string generateGetterHxx(const EnumerationElement& element);

        std::string generateSetterHxx(const EnumerationElement& element);

        std::string generateGetterHxx(const StructureElement& structure);

        std::string generateGetterHxx(const Array& array);

        std::string generateMembersHxx(const Structure& structure);

        std::string generateIncludesHxx(const Structure& structure);

        std::string generateConstructorCxx(const Structure& structure);

        std::string generateDestructorCxx(const Structure& structure);

        std::string generateGetSizeInBytesCxx(const Structure& structure);

        std::string generateSerialiseCxx(const Structure& structure);

        std::string generateDeserialiseCxx(const Structure& structure);

        std::string generateSerialisePrimitiveElement(const PrimitiveElement& element);

        std::string generateDeserialisePrimitiveElement(const PrimitiveElement& element);

        std::string generateSerialiseEnumeration(const EnumerationElement& enumeration);

        std::string generateDeserialiseEnumeration(const EnumerationElement& enumeration);

        std::string generateSerialiseStructure(const StructureElement& structure);

        std::string generateDeserialiseStructure(const StructureElement& structure);

        std::string generateSerialiseArray(const Array& array);

        std::string generateDeserialiseArray(const Array& array);

        std::string generateGettersCxx(const Structure& structure);

        std::string generateSettersCxx(const Structure& structure);

        std::string generateGetterCxx(const Structure& structure,
                                      const PrimitiveElement& element);

        std::string generateSetterCxx(const Structure& structure,
                                      const PrimitiveElement& element);

        std::string generateGetterCxx(const Structure& structure,
                                      const EnumerationElement& enumeration);

        std::string generateSetterCxx(const Structure& structure,
                                      const EnumerationElement& enumeration);

        std::string generateGetterCxx(const Structure& structure,
                                      const StructureElement& subStructure);

        std::string generateGetterCxx(const Structure& structure,
                                      const Array& array);
        
        uint64_t sizeOfPrimitiveType(const PrimitiveElement::TypeType& type);
        
        uint64_t sizeOfEnumeration(const std::string& enumString);
        
        std::string getArrayType(const Array& array);
        
        std::string convertEnumBaseTypeToCppType(const Enumeration::BaseTypeType& enumeration);

        std::string convertPrimitiveTypeToCppType(const PrimitiveElement::TypeType& element);
};