#pragma once

#include "Structure.hxx"

class CppGenerator {
    public:
        bool generate(const Structure& structure,
                      const std::string& outputFolder);

    private:
        bool generateHeaderFile(const Structure& structure,
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

        std::string generateMemberSerialisation(const Element& element,
                                                const std::string& nameToUse);

        std::string generateMemberDeserialisation(const Element& element,
                                                  const std::string& nameToUse);
};